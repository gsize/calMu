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

// Reads element symbols or chemical formulas.
int ParseFormulas(const char *W,int *JZ,float *WT);

// Sorts into monotonically increasing order 
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

int InitEnergyList(int KMAX,int *NZ,int NEGO,int JENG,int *JZ,int *JM,float *EAD,float *EN,int *KZ,int *KM);

//void Calculation(int KMAX,int *NZ, int NF,int NEGO,int NENG,float *EN,int *KZ,int *KM);
void Calculation(int KMAX,int *NZ,float *WEIGHT, int NF,int NEGO,int NENG,float *EN,int *KZ,int *KM,float *SCTCO,float *SCTIN,float *PHT,float *PRAT,float *PREL);

#endif
