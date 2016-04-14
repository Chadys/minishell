#include "minishell.h"

void ctrl(int sig){
	list temp;

	if(enfant){
		if(sig == SIGTSTP){
			temp=malloc(sizeof(cell));
			if(temp==NULL){
				fprintf(stderr, "Too many processes in memory, please kill some\n");
				return;
			}
			if(!enpause && !enbg)
				temp->num=1;
			else{
				if(enpause && (!enbg || enpause->num > enbg->num))
					temp->num=enpause->num+1;
				if(enbg && (!enpause || enbg->num > enpause->num))
					temp->num=enbg->num+1;
			}
			temp->pid=enfant;
			temp->next=enpause;
			enpause=temp;
			printf("\n[%d]+	Stopped\n",temp->num);
		}
		if(sig == SIGQUIT)
			printf("\nQuit (core dumped)\n");
		else
			kill(enfant,SIGQUIT);
		kill(enfant,sig);
	}
}

void bg(){
	list temp;

	if(enpause==NULL)
		printf("bg: No stopped process\n");
	else{
		temp=enpause->next;
		enpause->next=enbg;
		enbg=enpause;
		enpause=temp;
		kill(enbg->pid,SIGCONT);
		printf("\n[%d]+	&\n",enbg->num);
    	waitpid(enbg->pid,NULL,WNOHANG);
	}
}

void fg(){
	list temp;
	
	if(!enpause && !enbg){
		printf("fg: No current process\n");
		return;
	}

	if(enpause){
		if(!enbg || enbg->num < enpause->num){
			temp=enpause;
			enpause=enpause->next;
		}
	}
	if(enbg){
		if (!enpause || enbg->num > enpause->num){
			temp=enbg;
			enbg=enbg->next;
		}
	}

	kill(temp->pid,SIGCONT);
	printf("\n[%d]\n",temp->num);
	enfant=temp->pid;
    waitpid(temp->pid,NULL,WUNTRACED);
    enfant=0;
    free(temp);
}