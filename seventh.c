#include "stdio.h"
#include "stddef.h"
#include "stdint.h"
#include "limits.h"
#include "stdlib.h"
#include "math.h"
#include "unistd.h"
#include "omp.h"

#define elem(arr,N,i,j) (arr)[(i)*(N) + (j)]
#define a(i,j) elem(a,(matrix_dimension) + 1,(i),(j))

//Comment this to read matrix from file
//instead of generating it using
//the dimension of matrix read from file
#define OPTION_DRY

int main(int argc, char *argv[]);
int solve(int matrix_dimension, double* matrix, double* x);

int main(int argc, char *argv[])
{
	FILE* matrix_file;
	double* a;
	double* x;
	int element_count = 0;
	int matrix_dimension = 0;
	double tmp;
	
	matrix_file = fopen("matrix.dat","r");
	
	if( matrix_file == NULL )
	{
		puts("Couldn't open matrix file.");
		return 1;
	}
	
	#ifndef OPTION_DRY
	
	a = malloc(sizeof(double));
	while( ( fscanf(matrix_file, "%lf",a) > 0 ) && ( element_count++ >= 0 ) );
	free(a);
	
	matrix_dimension = (-1 + (long long) sqrtl(1 + 4*((long double) element_count)))/2;
	element_count = matrix_dimension * ( matrix_dimension + 1 );
	
	if( element_count < 2 )
	{
		puts("Invalid file.");
		fclose(matrix_file);
		return 3;
	}
	
	a = (double*) calloc(element_count,sizeof(double));
	
	if( a == NULL )
	{
		puts("Couldn't allocate memory");
		fclose(matrix_file);
		return 2;
	}
	
	rewind(matrix_file);
	for( int i = 0 ; ( (fscanf(matrix_file, "%lf", &a[i]) > 0) && i < element_count ) ; i++ );
	fclose(matrix_file);
	
	#else
	
	if( fscanf(matrix_file, "%d", &matrix_dimension) <= 0 )
	{
		puts("Invalid file.");
		fclose(matrix_file);
		return 3;
	}
	
	fclose(matrix_file);
	
	a = (double*) calloc(matrix_dimension*(matrix_dimension+1),sizeof(double));
	
	for( int i = 0 ; i < matrix_dimension ; i++)
	{
		for( int j = 0 ; j <= matrix_dimension ; j++)
		{
			a(i,j) = ( i == j || j == matrix_dimension ) ? 1 : 6 ;
		}
	}
	
	#endif
	
	x = calloc(matrix_dimension,sizeof(double));
	
	if( x == NULL )
	{
		puts("Couldn't allocate memory.");
		return 2;
	}
	
	if( !solve(matrix_dimension, a, x) )
	{
		free(a);
		free(x);
		return 4;
	}
	free(a);
	
	for( int i = 0 ; (printf("x[%d] = %lf\n",i,x[i]) > 0) && (i < matrix_dimension - 1) ; i++ );
	free(x);
	
	return 0;
}

#undef a

#define a(i,j) elem(a,matrix_dimension+1,(i),(j))

int solve(int matrix_dimension, double* a, double* x)
{
	if( (matrix_dimension < 1) || (a == NULL) || (x == NULL) )
		return 0;
	
	for( int i = 0; i < matrix_dimension - 1 ; i++ )
	{
		#pragma omp parallel for
		for( int k = i + 1 ; k <= matrix_dimension - 1; k++ )
		{
			for( int j = i + 1 ; j <= matrix_dimension; j++ )
			{
				a(k, j) -= a(k, i) * a(i, j) / a(i, i);
			}
		}
	}
	
	for( int j = matrix_dimension - 1 ; j >= 0 ; j-- )
	{
		x[j] = a(j,matrix_dimension) / a(j,j);
		#pragma omp parallel for
		for( int i = 0; i <= j ; i++ )
		{
			a(i,matrix_dimension) -= a(i,j) * x[j];
		}
	}
	
	return 1;
}

#undef a
