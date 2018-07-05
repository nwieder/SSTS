/**
 * Random_Generator.cpp
 */

#include "Uni_Rnd.h"
#include <iostream>

using namespace std;
using namespace nw;

// define Values for the random generator
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

double Uni_Rnd::uni_Rnd(long* idum){

	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	double temp;
//	Initialize.
	if (*idum <= 0 || !iy){
// 		Make sure to prevent idum = 0.
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
//		Load the shuffle table (after 8 warm-ups).
		for (j=NTAB+7;j>=0;j--){
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
	   iy=iv[0];
	}
// 	Start here when not initializing.
	k=(*idum)/IQ;
// 	Compute idum=(IA*idum) % IM without over
	*idum=IA*(*idum-k*IQ)-IR*k;

// 	flows by Schrage's method.
	if (*idum < 0) *idum += IM;

// 	Will be in the range 0..NTAB-1.
	j=iy/NDIV;
//  Output previously stored value and refill the
	iy=iv[j];
//	shuffle table.
	iv[j] = *idum;

// 	Because users don't expect endpoint values.
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}

double Uni_Rnd::get_Uni_Rnd(){
	double r = uni_Rnd(&seed);
	return r;
}
