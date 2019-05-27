#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "limits.h"

int h1 = 1000;
int w1 = 1000;
int w2 = 1000;

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	FILE* matrix_file_out1;
	FILE* matrix_file_out2;
	
	char* endptr[3];
	
	if( argc == 2 ) 
	{
		h1 = atoi(argv[1]);
		w1 = h1;
		w2 = w1;
		if( h1 < 1 )
		{
			puts("A matrix dimension must be a positive number.");
			return 2;
		}
	}
	else if( argc >=4 )
	{
		h1 = atoi(argv[1]);
		w1 = atoi(argv[2]);
		w2 = atoi(argv[3]);
		if( h1 < 1 || w1 < 1 || w2 < 1)
		{
			puts("Matrix dimensions must be positive numbers.");
			return 4;
		}
	}
	else
	{
		printf("Usage: %s matrix_height [matrix_width_1] [matrix_width_2]\
		\n\tGenerates two files containing matrices.\n",argv[0]);
		return 1;
	}
	
	matrix_file_out1 = fopen("a.dat","w");
	matrix_file_out2 = fopen("b.dat","w");
	
	fprintf(matrix_file_out1, "matrix %dx%d\n", h1, w1);
	fprintf(matrix_file_out2, "matrix %dx%d\n", w1, w2);
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			srand(1);
			for( int i = 0 ; i < h1 ; i++ )
			{
				for( int j = 0 ; j < w1 ; j++ )
				{
					fprintf(matrix_file_out1, "%d ", i*j);
				}
				fputs("\n", matrix_file_out1);
			}
		}
		#pragma omp section
		{
			srand(2);
			for( int i = 0 ; i < w1 ; i++ )
			{
				for( int j = 0 ; j < w2 ; j++ )
				{
					fprintf(matrix_file_out2, "%d ", i*j);
				}
				fputs("\n", matrix_file_out2);
			}
		}
	}
	fclose(matrix_file_out1);
	fclose(matrix_file_out2);
}
