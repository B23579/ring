# Process to run all the program we provided for ring topology

Our MPI programs were written in `C++`.  On the login node, compile each program using the following syntax  

```
module load openmpi-4.1.1+gnu-9.3.0
mpic++ blocking_ring.cpp -o block 
mpic++ non_blocking_ring.cpp -o non_block
```
To submit the job on the computer node, run the following syntax

```
qsub job.pbs
```
Where `job.pbs` is the PBS bash script. 

Once you have the output in csv file, run the R script `plot_performance.R` on Rstudio, make sure to install `tidyverse ` library in order to use `ggplot2`. The script and the csv file should be in the folder.  

# Process  to compute 3d matrix-matrix addition in parallel using a 1D toplogy

On the login node compiled the program `sum3Dmatrix.cpp` using the following syntax  

```
module load openmpi-4.1.1+gnu-9.3.0
mpic++ sum3Dmatrix.cpp -o matr
```
Creat a PBS script `sum.pbs` and past the following line of code 

```
#!/bin/bash

#PBS -N ring 
#PBS -q dssc
#PBS -l mem=100mb 
#PBS -l nodes=1:ppn=24

#PBS -l walltime=00:60:00

module load openmpi-4.1.1+gnu-9.3.0

cd $PBS_O_WORKDIR

for i in {2..5}
do
	mpirun -np 24 matr >> add.csv
done 
exit
```
Use the following syntax to submit the job to the computer node 

```
qsub sum.pbs
```



