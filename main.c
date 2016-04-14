#include "minishell.h"

# define PROMPT "? "

int enfant=0;
list enpause=NULL;
list enbg = NULL;

int main(){
  char ligne[MaxLigne];
  char pathname[MaxPathLength];
  char * mot[MaxMot];
  char * dirs[MaxDirs];
  char * wd;
  int i, tmp;
  short plsr;

  plsr=0; //cette commande comporte-elle plusieurs instructions?

  /* Decouper UNE COPIE de PATH en repertoires */
  decouper(strdup(getenv("PATH")), ":", dirs, MaxDirs);

  /* Lire et traiter chaque ligne de commande */
  for(wd=getcwd(NULL,0),printf("%s%s",wd,PROMPT); fgets(ligne, sizeof ligne, stdin) != NULL; wd=getcwd(NULL,0),printf("%s%s",wd,PROMPT)){
    free(wd);

    /* Traitement des ^C et ^Z */
    if (signal(SIGINT,SIG_IGN) == SIG_ERR)
      perror("signal");
    if (signal(SIGTSTP,ctrl) == SIG_ERR)
      perror("signal");
    if (signal(SIGQUIT,ctrl) == SIG_ERR)
      perror("signal");

    /* Traitement des "&" et des ";" */
    plsr=plusieurs(ligne);


    decouper(ligne, " \t\n", mot, MaxMot);
    if (mot[0] == NULL){            // ligne vide
      if(!plsr)
        continue;
      exit(0);
    }

    /*Traitement de la commande cd*/
    if (!strcmp(mot[0],"cd")){
      cd(mot,plsr);
      continue;
    }

    /*Traitement des commandes fg et bg*/
    if (!strcmp(mot[0],"fg")){
      fg();
      if(!plsr)
        continue;
      exit(0);
    }
    if (!strcmp(mot[0],"bg")){
      bg();
      if(!plsr)
        continue;
      exit(0);
    }


    if(!plsr){
      tmp = fork();               // lancer le processus enfant uniquement si ce n'est pas déjà un enfant à cause d'une multiplicité de commande
      if (tmp < 0){
        perror("fork");
        continue;
      }

      if (tmp != 0){             // parent : attendre la fin de l'enfant
        enfant=tmp;
        waitpid(tmp,NULL,WUNTRACED);
        enfant=0;
        continue;
      }
    }

				// enfant : exec du programme
    /* Traitement des "<", ">" ">>" et "2>" */
    redir(mot);


    /* Traitement des "|" */
    transfert(mot);

    
    for(i = 0; dirs[i] != NULL; i++){
      snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
      execv(pathname, mot);
    }
                                // aucun exec n'a fonctionne
    fprintf(stderr, "%s: not found\n", mot[0]);
    exit(1);
  }

  printf("Bye\n");
  return 0;

}
  