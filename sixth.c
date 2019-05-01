/*
Author: IRLynx
Description:
	This program calculates the volume of a hemisphere
	using Monte-Carlo method. Pseudorandom numbers are
	generated using LCG.
*/

#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdint.h"
#include "math.h"
#include "limits.h"
#include "omp.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/mman.h"

/*
Macro _RAND_NEXT(NUM)

Generates a pseudorandom unsigned int number based on seed NUM
Implements the LCG
*/
#define _RAND_NEXT(NUM) ( ( ((unsigned int)(NUM)) * 1103515245 ) + 12345)

//Helper; must be ONLY statement on a line.
#define _RAND_SEED(VAR) VAR = _RAND_NEXT(VAR)

#define _SQ(EXPR) ( (EXPR)*(EXPR) )

/*
Macro _VEC_SQ(X,Y,Z)

Calculates the squared norm of vector (x,y,z)
*/
#define _VEC_SQ(X,Y,Z) ( _SQ(X) + _SQ(Y) + _SQ(Z))

/*
Macro RAND_NEXT(TYPE,NUM,LIM)

Generates a number of type TYPE in range [0,LIM] 
using expression NUM as seed
*/
#define RAND_NEXT(TYPE,NUM,LIM)  (((TYPE) _RAND_NEXT(NUM))/((TYPE)((long)UINT_MAX + 1)))*((TYPE) LIM)

#define R 2.0
#define NUM_POINTS 200000000

//Comment this to disable parallelism
#define OPTION_PARALLEL

//Comment this to use slow method for determining
//if a point is within a hemisphere
//#define OPTION_SLOW

extern int main(int argc,char *argv[]);

#ifdef OPTION_SLOW
double f(double x, double y);
#endif

int main(int argc,char *argv[])
{
	double x, y, z;
	int rand = 0;
	int point_count = 0;
	
	#ifdef OPTION_PARALLEL
	#pragma omp parallel private(x,y,z,rand)
	#endif
	{
		//Initial value for the LCG
		
		#ifdef OPTION_PARALLEL
		rand = omp_get_thread_num();
		#endif
		
		//Calculate the number of random points inside the hemisphere of radius R
		#ifdef OPTION_PARALLEL
		#pragma omp for reduction(+:point_count)
		#endif
		for( unsigned long i = 0; i < NUM_POINTS; i++ )	//size of long is exactly one machine word in linux
		{
			x = -((double)R) + 2*RAND_NEXT(double, rand, R);
			_RAND_SEED(rand);
			y = -((double)R) + 2*RAND_NEXT(double, rand, R);
			_RAND_SEED(rand);
			z = RAND_NEXT(double, rand, R);
			_RAND_SEED(rand);
			
			#ifndef OPTION_SLOW
			if( _VEC_SQ(x,y,z) <= _SQ(R) )	//Checks if a point (x,y,z) is inside the hemisphere
			{
				point_count++;
			}
			#else
			//Checks if a point (x,y,z) is inside the hemisphere.
			//Very slow method, use only if the teacher asks you to do so.
			if( fabs(f(x,y)) <= fabs(z) ) 
			{
				point_count++;
			}
			#endif
			
		}
	}
	
	printf("The volume of hemisphere of radius %lf is %lf.\n", 
	       (double)R, ((double)point_count)/NUM_POINTS*R*R*R*4);
	
	return 0;
}

#ifdef OPTION_SLOW
double f(double x, double y)
{
	return pow(pow(x,2.0)+pow(y,2.0),0.5);
}
#endif
