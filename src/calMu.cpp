#include "xcom.h"
#include "GateMaterialsDB.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Analysis chemical formulas
void AnalysisChemicalFormulas(char* formulas)
{

    string w(formulas);
    w +=' ';

    int NZ[100];
    float WT[100];
    //return number of element
    int MMAX =ParseFormulas(w.c_str(),NZ,WT);
    if(MMAX<0)
    {
        printf("FORM status %d\n",MMAX);
        return ;
    }
    for(int i=0;i<MMAX;i++)
        printf("ele:%s MMAX: %d JZ[%d]: %d WT[%d]: %f\n",w.c_str(),MMAX,i,NZ[i],i,WT[i]);

    printf("-------------------------------------------------\n");

    float EAD[600];
    int JENG =1;

    float EN[1200];
    int KZ[1200],KM[1200];
    int lenth= 0;

    int NEGO =3; //3:only input energy list 
    int NF = 3;  //3: cm2/g
    int KMAX = MMAX;
    EAD[0]=511000.0;

    NEGO =2; //2: show all energy list (default and adding) 

    lenth = InitEnergyList(KMAX,NZ,NEGO,JENG,EAD,EN,KZ,KM);

    float SCTCO[ME],SCTIN[ME],PHT[ME],PRAT[ME],PREL[ME],PHDIF[ME];
    Calculation(KMAX,NZ,WT, NF,NEGO,lenth,EN,KZ,KM,SCTCO,SCTIN,PHT,PRAT,PREL,PHDIF);

    printf("          Photon      Scattering        Photo-     Pair production    Total attenuation\n");
    printf("          energy   coherent  incoher.  electric     in        in       with     without\n");
    printf("                                       absorption  nuclear electron  coherent  coherent\n");
    printf("                                                    field    field     scatt.    scatt.\n");
    printf("          (MeV)    (cm2/g)    (cm2/g)   (cm2/g)    (cm2/g)  (cm2/g)   (cm2/g)   (cm2/g)\n");
    for(int j=0;j<lenth; j++)
    {
        float crossAll =SCTCO[j]+SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
        double ca_noco=SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
        if(KZ[j]>0)
        {
            printf("       %10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j]-PHDIF[j],PRAT[j],PREL[j],crossAll-PHDIF[j],ca_noco-PHDIF[j]);
            printf("%7d%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",KZ[j],EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j],PRAT[j],PREL[j],crossAll,ca_noco);
        }else
            printf("       %10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j]-PHDIF[j],PRAT[j],PREL[j],crossAll-PHDIF[j],ca_noco-PHDIF[j]);
    }
}

void AnalysisGateMaterials()
{
    //read Gate MaterialsDB
    string gateMaterialsDB("../data/GateMaterials.db");
    MaterialDB matDB;
    matDB.ReadDB(gateMaterialsDB);

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
        int JENG =1;
        EAD[0]=511000.0;

        float EN[1200];
        int KZ[1200],KM[1200];
        int lenth= 0;

        int NEGO =2; //3:output all input energy list and default
        int NF = 3;
        int KMAX = MMAX;

        lenth = InitEnergyList(KMAX,NZ,NEGO,JENG,EAD,EN,KZ,KM);

        float SCTCO[ME],SCTIN[ME],PHT[ME],PRAT[ME],PREL[ME],PHDIF[ME];
        Calculation(KMAX,NZ,WT, NF,NEGO,lenth,EN,KZ,KM,SCTCO,SCTIN,PHT,PRAT,PREL,PHDIF);

		FILE *fp=NULL;
		string outfile(matDB.m_matList[i].GetName());
		outfile+="_cross.txt";
		fp=fopen(outfile.c_str(),"w");
		fprintf(fp,"          Photon      Scattering        Photo-     Pair production    Total attenuation\n");
    	fprintf(fp,"          energy   coherent  incoher.  electric     in        in       with     without\n");
    	fprintf(fp,"                                       absorption  nuclear electron  coherent  coherent\n");
    	fprintf(fp,"                                                    field    field     scatt.    scatt.\n");
    	fprintf(fp,"          (MeV)    (cm2/g)    (cm2/g)   (cm2/g)    (cm2/g)  (cm2/g)   (cm2/g)   (cm2/g)\n");
    	for(int j=0;j<lenth; j++)
    	{
    	    float crossAll =SCTCO[j]+SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
    	    double ca_noco=SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
    	    if(KZ[j]>0)
    	    {
    	        fprintf(fp,"       %10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j]-PHDIF[j],PRAT[j],PREL[j],crossAll-PHDIF[j],ca_noco-PHDIF[j]);
    	        fprintf(fp,"%7d%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",KZ[j],EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j],PRAT[j],PREL[j],crossAll,ca_noco);
    	    }else
    	        fprintf(fp,"       %10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j]-PHDIF[j],PRAT[j],PREL[j],crossAll-PHDIF[j],ca_noco-PHDIF[j]);
    	}
		fclose(fp);
    }
}

void AnalysisGateMaterials_511keV()
{
    //read Gate MaterialsDB
    string gateMaterialsDB("../data/GateMaterials.db");
    string muFile("../data/mumap.txt");
    MaterialDB matDB;
    matDB.ReadDB(gateMaterialsDB);
    //	matDB.PrintElements();
    //	matDB.PrintMaterials(1);
    //matDB.ReadMu(muFile);

    printf("-------------------------------------------------\n");

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
        int JENG =1;
        EAD[0]=511000.0;

        float EN[1200];
        int KZ[1200],KM[1200];
        int lenth= 0;

        int NEGO =3; //3:only input energy list 
        int NF = 3;
        int KMAX = MMAX;

        //if(matDB.m_matList[i].GetName()!="Water")
        //   continue;
        //NEGO =2; //2: show all energy list

        // print material information
        //matDB.m_matList[i].Print(1);

        lenth = InitEnergyList(KMAX,NZ,NEGO,JENG,EAD,EN,KZ,KM);

        float SCTCO[ME],SCTIN[ME],PHT[ME],PRAT[ME],PREL[ME],PHDIF[ME];
        Calculation(KMAX,NZ,WT, NF,NEGO,lenth,EN,KZ,KM,SCTCO,SCTIN,PHT,PRAT,PREL,PHDIF);
        if(i==0)
            printf("material  density(g/cm3) gamma(keV)  mu_mass(cm2/g)  mu(cm-1)\n");
        float crossAll=0.;
        for(int j=0;j<lenth; j++)
        {
            crossAll =SCTCO[j]+SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
            double ca_noco=SCTIN[j]+PHT[j]+PRAT[j]+PREL[j];
            //printf("%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E%10.3E\n",EN[j]*1.0e-6,SCTCO[j],SCTIN[j],PHT[j],PRAT[j],PREL[j],crossAll,ca_noco);
            printf("%15s  %10.3E  %10.3lf  %10.6E  %10.6E\n",matDB.m_matList[i].GetName().c_str(),matDB.m_matList[i].GetDensity(),EN[j]/1000.,crossAll,crossAll*matDB.m_matList[i].GetDensity());
        }
    }
}

int main(int argc, char** argv)
{
    if(argc>1)
        AnalysisChemicalFormulas(argv[1]);
    else
        AnalysisGateMaterials();
        //AnalysisGateMaterials_511keV();

    return 0;
}
