#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

//Duda: Cambia en algo si cierro TODOS los pipes luego de hacer
//		el dup2 correspondiente?
//Otra duda: Me tira que un proceso no temrina correctamente cuando hago mas de 3
//			 Tambien, el comando 'awk '{print $2}'' no funciona

void ejecutarPrimerPrograma(char ***prog, int pipe_count, int pipes[][2]){
	//Cambiamos std_out al write del pipe 
	dup2(pipes[0][PIPE_WRITE], STD_OUTPUT);
	//Y cerramos los pipes.
	for (int i = 0; i < pipe_count; i++)
	{
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}
	execvp(prog[0][0], prog[0]);
}

void ejecutarUltimoPrograma(char ***prog, int pipe_count, int pipes[][2], int index_prog){
	//Cambiamos std_in al write del pipe correspondiente al prog anterior (el último pipe)
	dup2(pipes[pipe_count - 1][PIPE_READ], STD_INPUT);
	//Y cerramos los pipes.
	for (int i = 0; i < pipe_count; i++)
	{
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}
	execvp(prog[index_prog][0], prog[index_prog]);
}

void ejecutarHijoMedio(char ***prog, int prog_count, int pipe_count, int pipes[][2], int index_prog) {
	//Remapeamos STD_INPUT al WRITE del pipe anterior 
	//y STD_OUTPUT al WRITE del su pipe
	dup2(pipes[index_prog-1][PIPE_READ], STD_INPUT);
	dup2(pipes[index_prog][PIPE_WRITE], STD_OUTPUT);
	//Y cerramos los pipes.
	for (int i = 0; i < pipe_count; i++)
	{
		close(pipes[i][PIPE_READ]);
		close(pipes[i][PIPE_WRITE]);
	}
	execvp(prog[index_prog][0], prog[index_prog]);
}

static int run(char ***progs, size_t count)
{	
	if (count == 0) return 0;
	
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
			if (pipe(pipes[i]) < 0) {
				printf("Failed to create pipes.");
				return EXIT_FAILURE;
			}
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
				//TODO: Para cada proceso hijo:
					//1. Redireccionar los file descriptors adecuados al proceso
					//2. Ejecutar el programa correspondiente
				if (i == 0)	{
					ejecutarPrimerPrograma(progs, pipe_count, pipes);
				} else if (i == count - 1) {
					ejecutarUltimoPrograma(progs, pipe_count, pipes, i);
				} else {
					ejecutarHijoMedio(progs, count, pipe_count, pipes, i);
				}
			}
		}
		/*IMPORTANTE
		Hay que cerrar los pipes del padre tambien porque el padre va a esperar
		a que terminen los hijos mas abajo en el codigo. Es decir que el proceso
		padre no va a morir y por ende va a mantener abiertos los pipes.

		En el ejercicio 2 de la clase practica no cierra los pipes del padre, pero
		funciona porque el proceso padre termina, con lo cual en ese momento se cierran
		automaticamente los fd.
		*/
		for (int i = 0; i < pipe_count; i++)
		{
			close(pipes[i][PIPE_READ]);
			close(pipes[i][PIPE_WRITE]);
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
/*
	for (size_t i = 0; i < programs_count; i++)
	{
		int j = 0;		
		while (programs_with_parameters[i][j] != NULL)
		{
			printf("%s ", programs_with_parameters[i][j]);
			j++;
		}
		printf("\n");		
	}*/
	

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
