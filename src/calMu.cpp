#include "xcom.h"
#include "GateMaterialsDB.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	/*
	// Test read MDATX3 database
	MDATX3 data;
	char dataPath[30];
	sprintf(dataPath,"%s",argv[1]);
	ReadMDATX3(dataPath, &data);

	for(int i=0;i<data.LAX;i++)
	{
	int IMAX = data.KMX[i];
	for(int I=0;I<IMAX;I++)
	{
	float fd;
	printf("%f\t",data.PHC[i][I]);
	}
	printf("\n");
	printf("%6d",data.KMX[i]);
	printf("\n");
	if(i== data.LAX-1)
	printf("\n");
	}
	*/
	//read Gate MaterialsDB
	string gateMaterialsDB("../data/GateMaterials.db");
	string muFile("../data/mumap.txt");
	MaterialDB matDB;
	matDB.ReadDB(gateMaterialsDB);
	//	matDB.PrintElements();
	//	matDB.PrintMaterials(1);
	matDB.ReadMu(muFile);

	printf("-------------------------------------------------\n");
	int MMAX;
	int NZ[100];
	float WT[100];
	/*
	   string w(argv[1]);
	   w +=' ';
	   MMAX =ParseFormulas(w.c_str(),NZ,WT);
	   if(MMAX<0)
	   {
	   printf("FORM status %d\n",MMAX);
	   return -1;
	   }
	   for(int i=0;i<MMAX;i++)
	   printf("ele:%s MMAX: %d JZ[%d]: %d WT[%d]: %f\n",w.c_str(),MMAX,i,NZ[i],i,WT[i]);

	   printf("-------------------------------------------------\n");
	   */
	for(int i=0;i<matDB.m_matList.size();i++)
	{
		int MMAX;
		int NZ[100];
		float WT[100];

		Element *el = NULL;
		MMAX = matDB.m_matList[i].GetNum();
		for(int j=0;j<MMAX;j++)
		{
			el =matDB.m_matList[i].GetElement(j);
			NZ[j] = el->m_Z;
			WT[j] = matDB.m_matList[i].Getf(j);;
		}
		float wtSun=0.;
		for(int i=0;i<MMAX;i++)
		{
			wtSun +=WT[i];
		}
		if(wtSun<0.999999)
		{
			printf("weigth <1.");
			for(int i=0;i<MMAX;i++)
			{
				WT[i]/=wtSun;
			}
		}

		float EAD[600];
		int JZ[600],JM[600];
		int JENG =1;
		EAD[0]=511000.0;

		float EN[1200];
		int KZ[1200],KM[1200];
		int lenth= 0;

		int NEGO =3; //3:only input energy list 
		int NF = 3;
		int KMAX = MMAX;
		lenth = InitEnergyList(KMAX,NZ,NEGO,JENG,JZ,JM,EAD,EN,KZ,KM);

		float SCTCO[ME],SCTIN[ME],PHT[ME],PRAT[ME],PREL[ME];
		Calculation(KMAX,NZ,WT, NF,NEGO,lenth,EN,KZ,KM,SCTCO,SCTIN,PHT,PRAT,PREL);
		if(i==0)
			printf("material  density(g/cm3) gamma(keV)  mu_mass(cm2/g)  mu(cm-1)\n");
		float crossAll=0.;
		for(int j=0;j<lenth; j++)
		{
			crossAll =SCTCO[j]+SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
			printf("%15s  %10.3E  %10.3lf  %10.6E  %10.6E\n",matDB.m_matList[i].GetName().c_str(),matDB.m_matList[i].GetDensity(),EN[j]/1000.,crossAll,crossAll*matDB.m_matList[i].GetDensity());
		}
	}
	return 0;
}
