#include<iostream> 
#include<mpi.h>
#include<random>

int main( int arg, char **argv)
{
	int rank, size;

	MPI_Init(&arg, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	
	int root_rank =0;

	// 3D matrix  ///
	
	double starttime = MPI_Wtime();
	
	int n=100, m=100, k=2400;

	double **A= new double*[k];
	double 	**B= new double*[k];
 	double **C= new double*[k];

	int end = n*m;
	srand(23);	
	
	for(int i=0; i<k; i++){
	
		A[i] = new double[end];
		B[i] = new double[end];
		C[i] = new double[end];
		
		for(int j=0;j<n*m;j++){

		A[i][j] = double( rand())/double(RAND_MAX);
		B[i][j] = double(rand())/double(RAND_MAX);
		C[i][j] =0;
		}
	}

	//  A+B 	

		// Let's scatter the table
	int count = int(end/size); // size of matrix to sum 
	int start= count*rank;
	int stop = start+ count ;
	int len = count*size; // size of the array which will gather process operations.
		// row iteration//
		
		double  *my_value = new double[len];
		double *sum = new double[count];
		
	for(int l=0;l<k;l++)
	{

		int j=0; 

		for(int i=start;i<stop; i++)
		{
			sum[j]=A[l][i]+B[l][i];
			j++; 
		}


		MPI_Gather(sum,count, MPI_DOUBLE,my_value,count,MPI_DOUBLE,root_rank,MPI_COMM_WORLD);
		


		if(rank==root_rank)
		{

		for(int  i=0;i<len;++i){
			C[l][i]=my_value[i];
			}

		// it is time to check whehter there not operation leave right
		if(end>len){
			for(int i=len;i<end;i++) C[l][i] = A[l][i] + B[l][i];
			}		
		}

		// wait for all the process 

		MPI_Barrier(MPI_COMM_WORLD); //  blocks untill all the processes in the communicator have reached this routine 
	} // close for l

	delete[] sum;

	// free space //

	for(int i=0; i<k;i++){
		delete A[i];
		delete C[i];
		delete B[i];
	}

	delete[] A;
	delete[] B;
	delete[] C;

	if(rank ==0) std::cout<<" Runtime is "<< MPI_Wtime()-starttime<<std::endl;
	
	MPI_Finalize();

	return 0;

}
