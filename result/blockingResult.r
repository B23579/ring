library(tidyverse)
library(tibble)
result<-read.csv("result.csv", sep=";") 
result_Non<-read.csv("resultNon.csv", sep=";")
result$Size<- as.integer(result$Size)
result_Non$Size<- as.integer(result_Non$Size)

################
result<-tibble(result)

#result$Size<-as.factor(result$Size)
#levels(result$Size)
result %>%group_by(Size)%>% 
  summarize(Walltime = mean(walltime, ra.rm=TRUE)/1e-6, sd=sd(walltime,na.rm = TRUE)/1e-6) -> resultf

resultf$Size <- as.integer(resultf$Size)

#view(result)
#########
result_Non<-tibble(result_Non)
#result_Non<-select(result_Non, Size : walltime) 
names(result_Non)
########

##########
#result_Non$Size<-as.factor(result_Non$Size)
#levels(result_Non$Size)
result_Non %>%group_by(Size)%>% 
  summarize(Walltime = mean(walltime, ra.rm=TRUE)/1e-6, std1=sd(walltime,na.rm = TRUE)/1e-6) -> resultf_Non

resultf_Non$Size <- as.integer(resultf_Non$Size)

##### Let's mutate the data from blocking and non blocking ring

result<- mutate(resultf,Walltime_Non = resultf_Non$Walltime, sd_non=resultf_Non$std1)

#view(result)


ggplot(result)+
  geom_point(mapping = aes(x=Size,y=Walltime))+
  geom_point(mapping = aes(x=Size,y=Walltime_Non, color="Isend/Irecv"))+
  geom_smooth(mapping = aes(x=Size,y=Walltime,color="Send/Recv"),se=FALSE)+
  geom_smooth(mapping = aes(x=Size,y=Walltime_Non,color="Isend/Irecv"),se=FALSE) +
  labs(x="Number of processors (P)", y= "Runtime in ns", title = "Runtime as a function of processors")+
  theme(plot.title = element_text(hjust = 0.5))+
  geom_errorbar(aes(x=Size, ymin=Walltime-sd, ymax=Walltime+sd,color="Send/Recv"), width=0.15,show.legend = TRUE)+
  geom_errorbar(aes(x=Size, ymin=Walltime_Non-sd_non, ymax=Walltime_Non+sd_non,color="Isend/Irecv"), width=0.15,show.legend = TRUE)+
  scale_color_manual(name = "Rutine",
                     breaks = c("Send/Recv", "Isend/Irecv"),
                     values = c("Send/Recv" = "blue", "Isend/Irecv" = "red"),drop=F )




