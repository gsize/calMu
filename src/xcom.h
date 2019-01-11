#ifndef XCOM_H
#define XCOM_H 1

#include <string>

using namespace std;

#define ME 1500
#define MEA 600
#define MEB 108
#define AVOG 0.60221367
#define EPAIR1 1.022007E+06
#define EPAIR2 2.044014E+06

// read MDATX3 file par
typedef struct MDATX3{
	int IDG[14],KMX[14];
	float EDGEN[14];
	string ADG[14];
	float ENG[14][35],PHC[14][35];
	float E[MEB],SCATCO[MEB],SCATIN[MEB];
	float PHOT[MEB],PAIRAT[MEB],PAIREL[MEB];
	float ATWT;
	int IZ,MAXEDG,MAXE,LAX;
}MDATX3;

// read MDATX3 datebase
int ReadMDATX3(char *file,MDATX3 *data);

// Sorts into monotonically increasing order 
// sort energy list
// input parameter:
//    NMAX: size of E
//    E   : pointer to  list of energy
template <class T>
void SORT(int NMAX, T *E);

// merges energy lists
template <class T>
int MERGE(T* E1,int *K1,int *L1,int MMAX,T*  E2,int *K2,int *L2,int NMAX);

// Reverses the order of lists
template <class T>
void REV(int NMAX,T *X);

//fits F as a function of X, and calculates cubic spline coefficients A,B,C and D
template <class T>
int SCOF(T *X,T *F,const int NMAX,T *A,T *B,T *C,T *D);

// Evaluates cubic spline as function of S, to obtain fitted result G.
template <class T>
int BSPOL(const T S,T *X,T *A,T *B,T *C,T *D,const int N,T &G);

// Linear interpolation routine
template <class T>
int BLIN(const T S,T *X,T *Y,const int N,T &TY);

//////////////////////////////////////////////////////////////
// Reads element symbols or chemical formulas.
// input parameter:
//     W  :  chemical formulas
//     JZ :  list of element shell number
//     WT :  list of weight of element
int ParseFormulas(const char *W,int *JZ,float *WT);

// Inititialize energy list 
// input parameter:
//   KMAX : number of elements
//   NZ   : list of atomic number of element
//   NEGO : flag of output status in energy list (1: default energy list(1 keV -- 100 GeV ) ; 2:default and add energy list; 3: only input energy list,)
//   JENG : number of add energy 
//   JZ   : list of add energy flag
//   JM   : list of add energy flag
//   EAD  : list of add energy
//   EN   : list of energy wanted to output
//   KZ   : list of energy wanted to output flag
//   KM   : list of energy wanted to output flag
// return:  the number of output energy
int InitEnergyList(int KMAX,int *NZ,int NEGO,int JENG,int *JZ,int *JM,float *EAD,float *EN,int *KZ,int *KM);

// calculate the cross sections for the interactions of photons with element, compound or mixture
// input parameter:
//   KMAX  : number of element
//   NZ    : list of atomic number of element
//   WEIGHT: list of weight of element
//   NF    :
//   NEGO  : flag of output status in energy list (1: default energy list(1 keV -- 100 GeV ) ; 2:default and add energy list; 3: only input energy list,)
//   NENG  : number of output energy
//   EN    : list of energy
//   KZ    : list of shell flag of element
//   KM    : list
//   SCTCO : scattering with coherent
//   SCTIN : scattering with incoherent
//   PHT   : photo elctric
//   PRAT  : pair proudction in nuclear field
//   PREL  : pair proudction in electron field
void Calculation(int KMAX,int *NZ,float *WEIGHT, int NF,int NEGO,int NENG,float *EN,int *KZ,int *KM,float *SCTCO,float *SCTIN,float *PHT,float *PRAT,float *PREL);

#endif
