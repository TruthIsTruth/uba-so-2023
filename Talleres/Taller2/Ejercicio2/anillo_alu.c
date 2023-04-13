#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"
#include <stdbool.h>

int generate_random_number(){
	return (rand() % 50);
}

void cerrarPipes(int pipePadre[2], int pipesEntreHijos[][2], int indexHijo, int n) {
	dup2(pipesEntreHijos[indexHijo][PIPE_WRITE], STD_OUTPUT);
	dup2(pipesEntreHijos[(indexHijo-1)%n][PIPE_READ], STD_INPUT);
	for(int i = 0; i < n; i++) {
		close(pipesEntreHijos[i][PIPE_READ]);
		close(pipesEntreHijos[i][PIPE_WRITE]);
	}
}

void ejecutarHijo(int pipePadre[2], int pipesEntreHijos[][2], int indexHijo, int n) {
	cerrarPipes(pipePadre, pipesEntreHijos, indexHijo, n);
}


int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, n, start, buffer;
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	start = atoi(argv[3]);

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
    /* COMPLETAR */
    int pipePadre[2];
	int pipesEntreHijos[n][2];

	if(pipe(pipePadre) == -1) {
		printf("Error al crear pipe hacia el padre.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < n; i++)
	{
		if(pipe(pipesEntreHijos[i]) == -1){
			printf("Error al crear pipes entre hijos.");
			exit(EXIT_FAILURE);
		}
	}
	

	for (int i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == 0) {
			ejecutarHijo(pipePadre, pipesEntreHijos, i, n);
		} else if (pid < 0) {
			printf("Error al crear hijo %i", i);
			exit(EXIT_FAILURE);
		}
	}
	
	return 0;
}
