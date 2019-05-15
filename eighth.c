#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "limits.h"

#define matr(i, j) (matr[(i)*(matr_width2) + (j)])
#define matr1(i, j) (matr1[(i)*(matr_width1) + (j)])
#define matr2(i, j) (matr2[(i)*(matr_width2) + (j)])

//Function matmul(matr,matr1,matr2,height1,width1,height2,width2)
//Multiplies matrix matr1 by matr2, storing result in matr.
int matmul(double* matr, double* matr1, double* matr2, int matr_height1, int matr_width1, int matr_height2, int matr_width2);

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	FILE* matrix_file_in1;
	FILE* matrix_file_in2;
	FILE* matrix_file_out;
	
	int matr_width1 = 0;
	int matr_width2 = 0;
	int matr_height1 = 0;
	int matr_height2 = 0;
	
	double* matr1;
	double* matr2;
	double* matr;
	
	int status = 0;
	
	matrix_file_in1 = fopen("a.dat","r");
	if( matrix_file_in1 == NULL )
	{
		puts("Couldn't open matrix file (a.dat).");
		return 1;
	}
	
	matrix_file_in2 = fopen("b.dat","r");
	if( matrix_file_in2 == NULL )
	{
		puts("Couldn't open matrix file (b.dat).");
		return 2;
	}
	
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			if ( fscanf(matrix_file_in1, "matrix %dx%d", &(matr_height1), &(matr_width1)) < 2 )
			{
				puts("Invalid file");
				fclose(matrix_file_in1);
				fclose(matrix_file_in2);
				#pragma omp atomic
				status += 1;
			}
		}
		#pragma omp section
		{
			if( fscanf(matrix_file_in2, "matrix %dx%d", &(matr_height2), &(matr_width2)) < 2 )
			{
				puts("Invalid file");
				fclose(matrix_file_in1);
				fclose(matrix_file_in2);
				#pragma omp atomic
				status += 1;
			}
		}
	}
	
	if( status )
		return 5;
	
	if( matr_width1 != matr_height2 )
	{
		puts("Matrix dimensions mismatch.");
		fclose(matrix_file_in1);
		fclose(matrix_file_in2);
		return 4;
	}
	
	#pragma omp parallel sections shared(status)
	{
		#pragma omp section
		{
			matr1 = malloc( matr_height1 * matr_width1 * sizeof(double) );
			
			if( matr1 == NULL )
			{
				puts("Couldn't allocate memory.");
				#pragma omp atomic
				status += 1;
			}
			for( int i = 0 ; i < matr_height1 ; i++ )
			{
				for( int j = 0 ; j < matr_width1 ; j++ )
				{
					if( fscanf(matrix_file_in1, "%lf", &matr1(i,j)) <= 0 )
					{
						#pragma omp atomic
						status += 1;
					}
					if( status )
					{
						break;
					}
				}
			}
		}
		#pragma omp section
		{
			matr2 = malloc( matr_height2 * matr_width2 * sizeof(double) );
			
			if( matr2 == NULL )
			{
				puts("Couldn't allocate memory.");
				#pragma omp atomic
				status += 1;
			}
			for( int i = 0 ; i < matr_height2 ; i++ )
			{
				for( int j = 0 ; j < matr_width2 ; j++ )
				{
					if( fscanf(matrix_file_in2, "%lf", &matr2(i,j)) <= 0 )
					{
						#pragma omp atomic
						status += 1;
					}
					if( status )
					{
						break;
					}
				}
			}
		}
	}
	
	fclose(matrix_file_in1);
	fclose(matrix_file_in2);
	
	if( status )
	{
		free((void*)matr1);
		free((void*)matr2);
		return 8;
	}
	
	matr = (double*) malloc( matr_height1 * matr_width2 * sizeof(double) );
	
	if( matr == NULL )
	{
		free((void*)matr1);
		free((void*)matr2);
		return 9;
	}
	
	if( !matmul(matr,matr1,matr2,matr_height1,matr_width1,matr_height2,matr_width2) )
	{
		puts("Weird...");
		return 127;
	}
	
	matrix_file_out = fopen("c.dat","w");
	
	if( matrix_file_out == NULL )
	{
		puts("Couldn't open output file(c.dat)");
		free((void*)matr);
		free((void*)matr1);
		free((void*)matr2);
		return 10;
	}
	
	fprintf(matrix_file_out, "matrix %dx%d\n", matr_height1, matr_width2);
	for( int i = 0 ; i < matr_height1 ; i++ )
	{
		for( int j = 0 ; j < matr_width2 ; j++ )
		{
			fprintf(matrix_file_out, "%lf\t", matr(i,j));
		}
		fputs("\n", matrix_file_out);
	}
	
	free((void*)matr);
	free((void*)matr1);
	free((void*)matr2);
	fclose(matrix_file_out);
	
	return 0;
}

int matmul(double* matr, double* matr1, double* matr2, int matr_height1, int matr_width1, int matr_height2, int matr_width2)
{
	if( matr == NULL || matr1 == NULL || matr2 == NULL || matr_width1 != matr_height2)
		return 0;
	
	#pragma omp parallel for shared(matr1,matr2)
	for( int i = 0 ; i < matr_height1 ; i++ )
	{
		//#pragma omp parallel for shared(matr1,matr2)
		for( int j = 0 ; j < matr_width2 ; j++ )
		{
			//#pragma omp parallel for shared(matr1,matr2)
			for( int k = 0; k < matr_width1 ; k++ )
			{
				matr(i,j) += matr1(i,k) * matr2(k,j);
			}
		}
	}
	return -1;
}
