#include<iostream>
#include<mpi.h>
#include<fstream>


int main(int argc,char *argv[]){

			//" This program use a blocking communication \n" ;

	int numtasks,rank,next, prev, buf[2]={100,100}, itag_rank=1, itag_rec_prev=2, itag_rec_next=2;
	
//	std::fstream myFile;

	MPI_Status stats[4]; // required variable for non-blocking call
	MPI_Request reqs[4]; // require variable for waitall  routine 

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
//	myFile.open("result/resultNon.csv",std::ios::app); // open in append mode

//	if(myFile.is_open()){
	
	double starttime, endtime, mean=0;
	//determine the right and left neighbors

	prev = rank -1;
	next = rank + 1;

	int msgleft = rank, msgright = -rank,sum=0, count=0; // sum collect the  the sum of the
		// rank and the receiving message at each step of the process.compt will compt the number of messages receive
		// for each process.   

	if(rank==0) prev= numtasks-1;
	if(rank== (numtasks-1)) next = 0;
		

	int N = 3000;// number of iteration to get a runtime mean 

	for(int i = 0; i< N; i++)
	{

	starttime = MPI_Wtime();

	int cond=1000; // in the cluster, we are allow to use 24 processes, using this number, we are sure 
		// that any process will have this rank.  So it help to solve deadlock at the first step 
		// of the program 
	
	itag_rank = rank*10;
	itag_rec_prev= prev*10;
	itag_rec_next = next*10;


	
	if(rank %2==0){ // this condition is used to avoid deadlock. at the begining 
		

		while(cond !=rank){

			MPI_Irecv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[0]); // buf[0] -> msgleft

			MPI_Isend(&msgleft,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[1]);
		
		// post non blocking receives and sends for neighbord

			MPI_Irecv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[2]); //buf[1] -> msgright
	
			MPI_Isend(&msgright ,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[3]);
		
		// wait for non-blocking operation to complete
			MPI_Waitall(4,reqs,stats);
	// The message receive from the next process become the message to send  to the previous process,inversely. 
			msgleft =  buf[0];
			msgright = buf[1];	
			
			sum = rank + msgleft + msgright;
			count= count+ 2 ; // because at each step, process p receive message from right and left.  

			cond =msgleft;
		}
		
//	std::cout<<" I am process " << rank <<" and I have received " << count << " messages. " << " My final messages have tag  " << itag_rank;
//	std::cout <<" .My final msgleft and msgright are "<< msgleft << " and " << msgright <<std::endl;
	
//	std::cout << std::endl;

	}
	else
	{	
		while( cond!= rank){
			MPI_Isend(&msgleft,1,MPI_INT,prev,itag_rank,MPI_COMM_WORLD,&reqs[0]);
			MPI_Irecv(&buf[0],1,MPI_INT,next,itag_rec_next,MPI_COMM_WORLD,&reqs[1]); // buf[0] -> msgleft

			
		// post non blocking receives and sends for neighbord
	
				 
			MPI_Isend(&msgright,1,MPI_INT,next,itag_rank,MPI_COMM_WORLD,&reqs[2]);
		
			MPI_Irecv(&buf[1],1,MPI_INT,prev, itag_rec_prev,MPI_COMM_WORLD, &reqs[3]); // buef[1]-> msgright

		// wait for all non-blocking operations to complete
		MPI_Waitall(4,reqs,stats);

			msgleft=buf[0];
			msgright= buf[1];
			cond= msgleft;
			sum = msgleft + msgright + rank;
			count= count+ 2;
		}
	}

	endtime = MPI_Wtime();

	mean = mean + (endtime-starttime);
	
	std::cout<<"\n"<< numtasks<<";"<<rank<<";"<<mean/N<<std::endl; //write on the file 
	
	}// iteration


	//std::cout<<" I am process " << rank <<" and I have received " << count << " messages. " << " My final messages have tag " << itag_rank;
	//std::cout <<" .My final msgleft and msgright are "<< msgleft<< " and "<< msgright <<std::endl;
	//std::cout << std::endl;
	
//	myFile.close();
	MPI_Finalize();	
	

	return 0;

	
//		}
//	else{
//		std:: cout<< "File not open. Please make sure you run the fille run.sh first \n";
//		MPI_Finalize();
//		return 1;
//	}

}
