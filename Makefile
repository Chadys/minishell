OBJ=cd.o decouper.o pipe.o plusieurs.o redirection.o signaux.o main.o
CC=gcc -Wall -W -g

minishell:	$(OBJ) minishell.h
			$(CC) $(OBJ) -o minishell

%.o:		%.c
			$(CC) -c $<

clean:
	rm *.o *~ core