#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

void ejecutarHijo(char ***prog, int prog_count, int pipe_count, int pipes[][2], int index_prog) {
	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente

//Chequeamos si es el primer programa
	if (index_prog == 0)
	{
		//Cambiamos std_out al write del pipe 
		dup2(pipes[0][PIPE_WRITE], STD_OUTPUT);
		//Cerramos el READ
		close(pipes[0][PIPE_READ]);

		//Y cerramos los demas pipes que no usa el proceso.
		for (int i = 1; i < pipe_count; i++)
		{
			close(pipes[i][PIPE_READ]);
			close(pipes[i][PIPE_WRITE]);
		}
		execvp(prog[index_prog][0], prog[index_prog]);
	}

//Chequeamos si es el último programa
	if (index_prog == prog_count - 1)
	{
		//Cambiamos std_in al write del pipe correspondiente al prog anterior (el último pipe)
		dup2(pipes[pipe_count - 1][PIPE_READ], STD_INPUT);
		//Cerramos el WRITE
		close(pipes[pipe_count - 1][PIPE_WRITE]);
		//Y cerramos los demas pipes que no usa el proceso.
		for (int i = 0; i < pipe_count - 1; i++)
		{
			close(pipes[i][PIPE_READ]);
			close(pipes[i][PIPE_WRITE]);
		}
		execvp(prog[index_prog][0], prog[index_prog]);
	}

//Si no
	//Remapeamos STD_INPUT al WRITE del pipe anterior 
	//y STD_OUTPUT al WRITE del su pipe
	dup2(pipes[index_prog-1][PIPE_READ], STD_INPUT);
	dup2(pipes[index_prog][PIPE_WRITE], STD_OUTPUT);
	//Cerramos pipes que no usamos
	close(pipes[index_prog-1][PIPE_WRITE]);
	close(pipes[index_prog][PIPE_READ]);
	for (int i = 0; i < pipe_count; i++)
	{
		if (i != index_prog && i != index_prog - 1) {
			close(pipes[i][PIPE_READ]);
			close(pipes[i][PIPE_WRITE]);
		}
	}
	execvp(prog[index_prog][0], prog[index_prog]);
}



static int run(char ***progs, size_t count)
{	
	int r, status;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);

	//TODO: Pensar cuantos pipes necesito.
	int pipe_count = count - 1; //Creamos 'count - 1' pipes
								//Ya que el ultimo programa usa stdout y el pipe del programa anterior
	if (pipe_count > 0){
		int pipes[pipe_count][2];
		for (int i = 0; i < pipe_count; i++)
		{
			pipe(pipes[i]);
		}
	
		//TODO: Pensar cuantos procesos necesito
		//A cada proceso hijo le doy uno de los programas
		for (int i = 0; i < count; i++)
		{
			pid_t pid = fork();
			if (pid != 0)
			{
				children[i] = pid;
			} else {
				ejecutarHijo(progs, count, pipe_count, pipes, i);
			}
		}
	} else {
		pid_t pid = fork();
			if (pid != 0) {
				children[0] = pid;
			} else {
				execvp(progs[0][0], progs[0]);
			}
	}
	
	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}

	r = 0;
	free(children);

	return r;
}


int main(int argc, char **argv)
{
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
