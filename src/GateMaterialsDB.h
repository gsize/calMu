#ifndef GATEMATERIALSDB_H
#define  GATEMATERIALSDB_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Element
{
	public:
		Element();
		Element& operator=(const Element&);
		virtual ~Element(){};
		void Print();

//	private:
	string m_fullName;
	string m_shortName;
	int m_Z;
	float m_A;
};

class Material
{
	public:
		Material();
		Material& operator=(const Material&);
		virtual ~Material(){};

		string GetName(){return m_name;};
		void SetName(string name){m_name=name;};
		float GetDensity(){return m_density;};
		void SetDensity(float density){m_density=density;};
		int GetNum(){return m_n;};
		void SetNum(int n){m_n=n;};
		void AddElement(Element );
		Element *GetElement(int i);
		void GetElements(vector<Element> &elements){elements = m_elements;};
		void Addf(float);
		float Getf(int n);
		void GetVectorf(vector<float> &f){f = m_f;};
		float GetM(){return m_M;};
		void SetM(float f){m_M=f;};
		void Print(int flag =0);
		void Clear();

	private:
	string m_name;
	float m_density;
	int m_n;
	vector<Element> m_elements;
	vector<float> m_f;
	float m_M;
};

class MaterialDB
{
	public:
		MaterialDB(){};

		int ReadDB(string dbFile);
		Element *FindElementByFullName(string name);
		void PrintMaterials(int flag =0);
		void PrintElements();

	vector<Element> m_elemList;
	vector<Material> m_matList;
};


#endif 
