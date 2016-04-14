# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <assert.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,		// nbre max de repertoire dans PATH
  MaxPathLength = 512,		// longueur max d'un nom de fichier
};

struct cell
{
  int num;
  int pid;
  struct cell* next;
};
typedef struct cell cell;
typedef cell* list;

extern int enfant; //si enfant en cours, utile pour SIGINT et SIGSTOP
extern list enpause; //pile des enfants stoppé, utile pour fg et bg
extern list enbg; //pile des enfants en arrière-plan, utile pour fg

/* decouper une chaine en mots */
int decouper(char *, char *, char *[], int);
/* gestion des redirections */
void redir(char*[]);
/* gestion du changement de répertoire courant */
void cd(char*[],short);
/* gestion de commandes multiples */
short plusieurs(char *);
/* gestion des pipes */
void transfert(char*[]);
/* gestion des signaux */
void ctrl(int);
/* gestion des bg/fg */
void bg();
void fg();