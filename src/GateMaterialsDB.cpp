#include "GateMaterialsDB.h"

#include <stdlib.h>
#include <stdio.h>


Element::Element():
	m_Z(0)
	,m_A(0.0)
{
}
Element& Element::operator=(const Element &el)
{
	if(&el ==this)return *this;
	this->m_fullName = el.m_fullName;
	this->m_shortName = el.m_shortName;
	this->m_Z =el.m_Z;
	this->m_A = el.m_A;
}

void Element::Print()
{
	if(m_Z>0)
		printf("el:%s  Z:%d  A:%f\n",m_shortName.c_str(),m_Z,m_A);
	else
		printf("no element\n");
}

Material::Material():
	m_n(0)
	,m_density(0.0)
	,m_M(0.0)
{
}
void Material::Clear()
{
	m_n =0;
	m_density =0.0;
	m_M =0.0;
	m_elements.clear();
	m_f.clear();
}

Material& Material::operator=(const Material& mat)
{
	if(&mat ==this)return *this;
	this->m_name = mat.m_name;
	this->m_n= mat.m_n;
	this->m_density= mat.m_density;
	this->m_f = mat.m_f;
	this->m_elements = mat.m_elements;
	this->m_M = mat.m_M;
}

void Material::AddElement(Element ele)
{
	m_elements.push_back(ele);
}

Element *Material::GetElement(int i)
{
	if(i<m_n &&i>=0)
		return &(m_elements[i]);
	else
		return NULL;
}
void Material::Addf(float f)
{
	m_f.push_back(f);
}
float Material::Getf(int i)
{
	if(i<m_n &&i>=0)
		return m_f[i];
	else
		return 0.0;
}
void Material::Print(int flag)
{
	if(m_n>0)
	{
		printf("%s d=%f M=%f  n=%d\n",m_name.c_str(),m_density,m_M,m_n);
		if(flag)
		{
			for(int i=0;i<m_elements.size();i++)
				printf("+el:%8s%10.6f\n",m_elements[i].m_shortName.c_str(),m_f[i]);
		}
	}
	else
		printf("no Material\n");
}

Element *MaterialDB::FindElementByFullName(string name)
{
	if(m_elemList.size()>0)
	{
		Element el;
		for(int i=0;i<m_elemList.size();i++)
		{
			if(name == m_elemList[i].m_fullName)
				return &(m_elemList[i]);
		}
	}
	return  NULL;
}

Material *MaterialDB::FindMaterialByName(string name)
{
	if(m_matList.size()>0)
	{
		Material *el;
		for(int i=0;i<m_matList.size();i++)
		{
			if(name == m_matList[i].GetName())
				return &(m_matList[i]);
		}
	}
	return  NULL;
}

int MaterialDB::ReadDB(string dbFile)
{
	FILE *pf = fopen(dbFile.c_str(),"r");
	if(!pf)
	{
		printf("Read GateMaterialsDB failure!\n");
		return -1;
	}
	int status=0;
	char strBuf[256];
	int flag =0;
	Material mat;
	vector<int> nList;
	//while((!feof(pf)) && (!ferror(pf)))
	while(fgets(strBuf,256,pf))
	{
		//strBuf[0]='\0';
		//fgets(strBuf,256,pf);
		string str(strBuf);
		if(str.find("Elements")!=string::npos && flag !=1)
		{
			flag =1;
			continue;
		}
		if(str.find("Materials")!=string::npos && flag !=2)
		{
			flag =2;
			continue;
		}
		if(flag ==1 && str.find("S=")!=string::npos)
		{
			char fname[30],sname[5];
			int z;
			float A;
			sscanf(strBuf,"%[^:]:%*[^=]=%s%*[^=]=%d%*[^=]=%f",fname,sname,&z,&A);
			if(A>0.)
			{
				Element el;
				el.m_fullName = fname;
				el.m_shortName= sname;
				el.m_Z =int(z);
				el.m_A = A;
				m_elemList.push_back(el);
			}
		}
		if(flag ==2 && str.find("d=")!=string::npos)
		{
			if(mat.GetNum()>0)
			{
				if(nList.size()>0)
				{
					for(int i=0;i<nList.size();i++)	
					{
						Element *el_tmp = mat.GetElement(i);
						float f = el_tmp->m_A*nList[i]/mat.GetM();
						mat.Addf(f);
					}
					nList.clear();
				}
				m_matList.push_back(mat);
				mat.Clear();
			}
			char fname[30];
			float d;
			int n;
			sscanf(strBuf,"%[^:]:%*[^=]=%f%*[^=]=%d",fname,&d,&n);
			string fn(fname);
			mat.SetName(fn);

			if(str.find("mg/cm3")!=string::npos)
				mat.SetDensity(0.001*d);
			else
				mat.SetDensity(d);
			mat.SetNum(n);
		}
		if(flag ==2 && str.find("+el")!=string::npos)
		{
			char elname[30];
			if(str.find("n=")!=string::npos)
			{
				int n;
				sscanf(strBuf,"%*[^=]=%s%*[^=]=%d",elname,&n);
				string fn(elname);
				if(fn.find("auto")!=string::npos)
					fn = mat.GetName();
				Element *el_tmp = FindElementByFullName(fn);
				mat.AddElement(*el_tmp);
				mat.SetM(mat.GetM()+n*el_tmp->m_A);
				nList.push_back(n);
			}
			else if(str.find("f=")!=string::npos)
			{
				float f;
				sscanf(strBuf,"%*[^=]=%s%*[^=]=%f",elname,&f);
				string fn(elname);
				Element *el_tmp = FindElementByFullName(fn);
				if(f>0.00001 && el_tmp)
				{
					mat.AddElement(*el_tmp);
					mat.Addf(f);
					//	mat.SetM(mat.GetM()+f*el_tmp->m_A);
				}
				else
					mat.SetNum(mat.GetNum()-1);
			}else
				printf("error: %s don't have element\n", mat.GetName().c_str());
		}
	}
	fclose(pf);

	// add last material
	if(mat.GetNum()>0)
	{
		if(nList.size()>0)
		{
			for(int i=0;i<nList.size();i++)	
			{
				Element *el_tmp = mat.GetElement(i);
				float f = el_tmp->m_A*nList[i]/mat.GetM();
				mat.Addf(f);
			}
			nList.clear();
		}
		m_matList.push_back(mat);
		//	mat.Print();
		mat.Clear();
	}
	return status;
}

int MaterialDB::ReadMu(string muFile)
{
	FILE *pf=NULL;
	pf = fopen(muFile.c_str(),"r");
	if(!pf)
	{
		printf("Read mumap failure!\n");
		return -1;
	}
	char strBuf[256];
	while(fgets(strBuf,256,pf))
	{
		string str(strBuf);
		if(str.find("#")==string::npos)
		{
			char mat[50];
			float density,energy,muMass,mu;
			Material *pMat=NULL;
			sscanf(strBuf,"%s%f%f%f%f",mat,&density,&energy,&muMass,&mu);
			string str(mat);
			pMat= FindMaterialByName(str);
			if(pMat)
				pMat->SetMu(mu);
			else
				printf("MaterialDB don't have material %s\n",mat);
		}
	}
}

void MaterialDB::PrintElements()
{
	for(int i=0;i<m_elemList.size();i++)	
	{
		printf("%d:",i);
		m_elemList[i].Print();
	}
}
void MaterialDB::PrintMaterials(int flag)
{
	for(int i=0;i<m_matList.size();i++)	
	{
		printf("%d:",i);
		m_matList[i].Print(flag);
	}
}

