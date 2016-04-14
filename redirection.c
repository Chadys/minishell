#include "minishell.h"

size_t tailledest(char *);

void redir(char* mot[]){
  	int i,tmp;
  	size_t reste;
  	short separ;
  	char *signe, *dest, *destcut, *temp;

  	separ=0;
    for(i=0;mot[i] != NULL;i++){

      	signe=strstr(mot[i],"<"); //cas "<"
      	if(signe!=NULL){
        	if(strlen(signe)==1){ //"<" et le fichier à rediriger sont séparés par un espace
          		separ=1; //indicateur pour retirer les redirection de la commande
          		dest=mot[i+1]; //fichier servant de redirection
        	}
        	else {//"<" et le fichier à rediriger n'était pas séparés
        	  	separ=2;
        		dest=signe+1;
        	}
    	}

    	temp=strstr(mot[i],">>"); //cas ">>"
    	if(temp!=NULL && (signe==NULL || strlen(temp)>strlen(signe))){
    		signe=temp;
        	if(strlen(signe)==2){
    	    	separ=1;
            	dest=mot[i+1];
        	}
        	else {
            	separ=2;
            	dest=signe+2;
        	}

    	}
    	temp=strstr(mot[i],"2>"); //cas "2>"
        if(temp!=NULL && (signe==NULL || strlen(temp)>strlen(signe))){
    		signe=temp;
            if(strlen(signe)==2){
              	separ=1;
              	dest=mot[i+1];
            }
            else {
              	separ=2;
              	dest=signe+2;
            }
        }
        temp=strstr(mot[i],">"); //cas ">"
    	if(temp!=NULL && (signe==NULL || strlen(temp)>strlen(signe))){
    		signe=temp;
            if(strlen(signe)==2){
                separ=1;
                dest=mot[i+1];
            }
            else {
                separ=2;
                dest=signe+1;
            }
      
        }

      	if(separ){
      		if(*signe=='<') tmp=close(0);
      		if(*signe=='>') tmp=close(1);
      		if(*signe=='2') tmp=close(2);
        	if (tmp < 0){
            	perror("close");
            	exit(1);
        	}
        	reste=tailledest(dest); //signes pour voir s'il ne reste pas d'autre redirection non séparées par des espaces
        	if(reste<strlen(dest)){
         	 	destcut=malloc((reste+1)*sizeof(char));
          		if(destcut==NULL){
            		perror("malloc");
            		exit(1);
          		}
          		strncpy(destcut,dest,reste);
          		*(destcut+reste)='\0';
          		if((*signe=='>' && *(signe+1)!='>') || *signe=='2')
            		tmp=open(destcut,O_WRONLY | O_CREAT | O_TRUNC,0666);
          		if(*signe=='>' && *(signe+1)=='>')
            		tmp=open(destcut,O_WRONLY | O_CREAT | O_APPEND,0666);
          		if(*signe=='<')
            		tmp=open(destcut,O_RDONLY);
          		free(destcut);
          		if (tmp < 0){
            		perror("open");
            		exit(1);
          		}
        	}
        	else{
          		if((*signe=='>' && *(signe+1)!='>') || *signe=='2')
            		tmp=open(dest,O_WRONLY | O_CREAT | O_TRUNC,0666);
          		if(*signe=='>' && *(signe+1)=='>')
            		tmp=open(dest,O_WRONLY | O_CREAT | O_APPEND,0666);
          		if(*signe=='<')
            		tmp=open(dest,O_RDONLY);
          		if (tmp < 0){
            		perror("open");
            		exit(1);
          		}
        	}

        	if (strlen(mot[i])>strlen(signe)){ //si la redirection est collée au mot d'avant
          		if(separ==2){ //si elle est aussi collée au mot d'après
            		if(strlen(dest)>reste)
              			i--;
            		strncpy(signe,dest+reste,strlen(signe));
            		separ=0;
            		continue;
          		}
          		memset(signe,'\0',strlen(signe));
          		if(strlen(dest)>reste){
            		strncpy(dest,dest+reste,strlen(dest));
            		continue;
          		}
          		reste=strlen(dest);
          		i++;
          		separ=2; //permettra d'effacer le mot d'après
        	}
        	if(separ==1){
          		if(strlen(dest)>reste){
            		strncpy(dest,dest+reste,strlen(dest));
            		reste=strlen(dest);
            		separ=2;
          		}
          		else{
            		for(tmp=i;mot[tmp]!=NULL;tmp+=2){
              			if(tmp+2>=MaxMot)
                			mot[tmp]=NULL;
              			else
                			mot[tmp]=mot[tmp+2];
              			if(tmp+3>=MaxMot)
                			mot[tmp+1]=NULL;
              			else
                			mot[tmp+1]=mot[tmp+3];
            		}
          		}
          		i--;
        	}
        	if(separ==2){
          		if(strlen(dest)>reste)
            		strncpy(signe,dest+reste,strlen(signe));
          		else{
            		for(tmp=i;mot[tmp]!=NULL;tmp++){
             	 		if(tmp+1>=MaxMot)
                			mot[tmp]=NULL;
              			else
                			mot[tmp]=mot[tmp+1];
            		}
          		}
          		i--;
        	}
        	separ=0;
      	}
    }
}

size_t tailledest(char * suite){
  	size_t reste;
  	char* temp;

  	reste=strlen(suite);
  	temp=strstr(suite,"2>");
  	if(temp!=NULL)
  		reste-=strlen(temp);
  	temp=strstr(suite,">");
  	if(temp!=NULL && (strlen(suite)-strlen(temp)<reste))
    	reste=strlen(suite)-strlen(temp);
  	temp=strstr(suite,"<");
  	if(temp!=NULL && (strlen(suite)-strlen(temp)<reste))
    	reste=strlen(suite)-strlen(temp);
  	temp=strstr(suite,"|");
  	if(temp!=NULL && (strlen(suite)-strlen(temp)<reste))
    	reste=strlen(suite)-strlen(temp);
  	return reste;
}