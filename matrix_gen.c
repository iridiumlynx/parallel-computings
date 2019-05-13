#include "stdio.h"
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "limits.h"

#define h1 1000
#define w1 1000
#define w2 1000

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	FILE* matrix_file_out1;
	FILE* matrix_file_out2;
	
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
					fprintf(matrix_file_out1, "%d ", rand());
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
					fprintf(matrix_file_out2, "%d ", rand());
				}
				fputs("\n", matrix_file_out2);
			}
		}
	}
	fclose(matrix_file_out1);
	fclose(matrix_file_out2);
}
