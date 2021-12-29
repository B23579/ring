# Process tho run all the program we provided

Our MPI programs were written in `C++`.  On the login node compiled each program using the following syntax  

```
module load openmpi-4.1.1+gnu-9.3.0
mpic++ blocking_ring.cpp -o block 
mpic++ non_blocking_ring.cpp -o non_block
```
To submit the job the program on the computer node, run the following syntax on the 

```
qsub job.pbs
```
Once you have the output in csv file, run the R script `plot_performance.R` make sure to install `tidyverse ` library in order to use `ggplot2`. 

