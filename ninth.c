#include "stdio.h"
#include "stdlib.h"
#include "limits.h"
#include "string.h"
#include "mpi.h"

int main(int argc,char* argv[])
{
	int rank,size;
	char num;
	char minmax[2];
	
	minmax[0] = CHAR_MAX;
	minmax[1] = CHAR_MIN;
	
	char* sbuf;
	char* rbuf;
	
	MPI_Init(&argc,&argv);
	
	MPI_Status status;
	
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	num = (char)((rank * 44 + 50) % 100);
	
	sbuf = (char*) malloc(size * sizeof(char));
	rbuf = (char*) malloc(size * sizeof(char));
	memset(sbuf, (int)num, size * sizeof(char));
	
	MPI_Alltoall((void*)sbuf, 1, MPI_CHAR, (void*)rbuf, 1, MPI_CHAR, MPI_COMM_WORLD);
	
	free(sbuf);
	
	for( int i = 0 ; i < size ; i++)
	{
		minmax[0] = (rbuf[i] < minmax[0]) ? rbuf[i] : minmax[0];
		minmax[1] = (rbuf[i] > minmax[1]) ? rbuf[i] : minmax[1];
	}
	
	printf("Process %d/%d received num=%d\n",rank,size,num);
	printf("Process %d/%d found min = %d max = %d\n", rank, size, (int)minmax[0], (int)minmax[1]);
	
	MPI_Finalize();
	return 0;
}
