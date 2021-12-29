library(tidyverse)
library(tibble)


result<-read.csv("result.csv", header = FALSE,sep=";") 
result%>%rename(Size=V1, Rank=V2, walltime=V3)-> result

result_Non<-read.csv("resultNon.csv",header = FALSE, sep=";")
result_Non%>%rename(Size=V1, Rank=V2, walltime=V3)-> result_Non

result$Size<- as.numeric(result$Size)
result_Non$Size<- as.numeric(result_Non$Size)
result$walltime<- as.numeric(result$walltime)
result_Non$walltime<- as.numeric(result_Non$walltime)

result <- result[rowSums(is.na(result)) == 0,] # remove empty row
result_Non <- result_Non[rowSums(is.na(result_Non)) == 0,] # remove empty row

sum(is.na(result))
sum(is.na(result_Non))
################
result<-tibble(result)

result %>%group_by(Size)%>% 
  summarize(Walltime = mean(walltime, ra.rm=TRUE)*1e3, sd=sd(walltime,na.rm = TRUE)) -> resultf

result_Non<-tibble(result_Non)
result_Non %>%group_by(Size)%>% 
  summarize(Walltime = mean(walltime, ra.rm=TRUE)*1e3, std1=sd(walltime,na.rm = TRUE)) -> resultf_Non

#resultf_Non<-resultf_Non[-24,] run this if there are more than 23 rows

##### Let's mutate the data from blocking and non blocking ring

result<- mutate(resultf,Walltime_Non = resultf_Non$Walltime, sd_non=resultf_Non$std1)

view(result)

ggplot(result)+
  geom_point(mapping = aes(x=Size,y=Walltime_Non, color="Isend/Irecv"))+
  geom_smooth(mapping = aes(x=Size,y=Walltime_Non,color="Isend/Irecv"),se=FALSE) +
  labs(x="Number of processors (P)", y= "Runtime(ms)", title = "Runtime as a function of processors")+
  theme(plot.title = element_text(hjust = 0.5))+
  geom_errorbar(aes(x=Size, ymin=Walltime_Non-sd_non, ymax=Walltime_Non+sd_non,color="Isend/Irecv"), width=0.15,show.legend = TRUE)+
  scale_color_manual(name = "Rutine",
                     breaks = c( "Isend/Irecv"),
                     values = c( "Isend/Irecv" = "blue"),drop=F )
ggplot(result)+
  geom_point(mapping = aes(x=Size,y=Walltime, color="Send/Recv"))+
  geom_smooth(mapping = aes(x=Size,y=Walltime,color="Send/Recv"),se=FALSE) +
  labs(x="Number of processors (P)", y= "Runtime (ms)", title = "Runtime as a function of processors")+
  theme(plot.title = element_text(hjust = 0.5))+
  geom_errorbar(aes(x=Size, ymin=Walltime-sd, ymax=Walltime+sd,color="Send/Recv"), width=0.15,show.legend = TRUE)+
  scale_color_manual(name = "Rutine",
                     breaks = c( "Send/Recv"),
                     values = c( "Send/Recv" = "blue"),drop=F )

#write.csv2(result, "result_1500_iteration.csv") to save the result 

ggplot(resultf_Non)+
  geom_point(mapping = aes(x=Size,y=Walltime))+
  geom_smooth(mapping = aes(x=Size,y=Walltime,color="Send/Recv"),se=FALSE)+
  labs(x="Number of processors (P)", y= "Runtime (ms)", title = "Runtime as a function of processors")+
  theme(plot.title = element_text(hjust = 0.5))

ggplot(resultf)+
  geom_point(mapping = aes(x=Size,y=Walltime))+
  geom_smooth(mapping = aes(x=Size,y=Walltime,color="Send/Recv"),se=FALSE)+
  labs(x="Number of processors (P)", y= "Runtime in ns", title = "Runtime as a function of processors")+
  theme(plot.title = element_text(hjust = 0.5))

