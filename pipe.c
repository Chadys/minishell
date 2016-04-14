#include "minishell.h"

void transfert(char* mot[]){
	int i,j,tmp;
	char *signe;
    int t[2];

    for(i=0;mot[i] != NULL;i++){
		for(signe=strstr(mot[i],"|");signe!=NULL;signe=strstr(mot[i],"|")){
	      	
	      	if(mot[0][1]=='|' || (strlen(signe)==1 && mot[i+1]==NULL)){ //pas d'entrée ou de sortie pour le pipe
        	  	fprintf(stderr, "|: syntax error\n");
          		exit(1);
        	}

        	tmp=pipe(t);
       		if (tmp < 0){
       			perror("pipe");
       			exit(1);
   			}
        	tmp=fork();
        	if (tmp < 0){
       			perror("fork");
       			exit(1);
   			}

      		if(tmp==0){ //enfant : entrée (écriture) du pipe (cet ordre a été choisi afin que le wait du parent ne s'achève pas avant que toutes les commandes du pipe ne soient effectuées)
      			tmp=close(t[0]); //ferme la sortie du pipe
       			if (tmp < 0){
           			perror("close");
           			exit(1);
       			}
   				tmp=dup2(t[1],1); //remplace stdout par l'entrée du pipe
           		if (tmp < 0){
           			perror("dup2");
           			exit(1);
       			}
       			tmp=close(t[1]);
       			if (tmp < 0){
           			perror("close");
           			exit(1);
       			}
          		j=i;
          		if(*mot[i]!='|'){ //efface "|" et tout ce qui suit
	       			strncpy(signe,"\0",strlen(signe));
         			j++;
       			}
          		while(mot[j]!=NULL){
          			mot[j]=NULL;
          			j++;
       			}
       			return;
   			}

      		else{ //parent : sortie (lecture) du pipe
      			tmp=close(t[1]); //ferme l'entrée du pipe
       			if (tmp < 0){
           			perror("close");
           			exit(1);
       			}
   				tmp=dup2(t[0],0); //remplace stdin par la sortie du pipe
           		if (tmp < 0){
            		perror("dup2");
           			exit(1);
        		}
       			tmp=close(t[0]);
       			if (tmp < 0){
           			perror("close");
           			exit(1);
       			}
       			j=0;
        		if(strlen(signe)==1)//efface "|" et tout ce qui précède
        			i++;
        		else
       				strncpy(mot[i],signe+1,strlen(mot[i]));
       			while(mot[j]!=NULL){
       				if(j>0 && mot[j-1]==NULL)
       					mot[j]=NULL;
       				else
    					mot[j]=mot[i+j];
       				j++;
       			}
        		i=0;
      		}
       	}
   	}
}