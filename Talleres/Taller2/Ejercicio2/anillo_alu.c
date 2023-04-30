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

void cerrarPipesHijo(int pipeDesdePadre[2], int pipeHaciaPadre[2], int pipesEntreHijos[][2],
					 int indexHijo, int start, int n) 
{
	if (indexHijo != start)
	{
		close(pipeDesdePadre[PIPE_READ]);
		close(pipeDesdePadre[PIPE_WRITE]);
		close(pipeHaciaPadre[PIPE_READ]);
		close(pipeHaciaPadre[PIPE_WRITE]);
	}
	//Hacemos modulo manual porque C hace cosas raras con numero negativo y modulo
	int pipeAnt = (indexHijo - 1);
	if (pipeAnt < 0) pipeAnt += n;
	//Cerramos los pipes, salvo el que comunica al hijo actual con su sucesor y antecesor
	for(int i = 0; i < n; i++) {
		if (i == indexHijo) { //Actual -> Sucesor
			close(pipesEntreHijos[i][PIPE_READ]);
		} else if (i == pipeAnt) {//Antecesor -> Actual
			close(pipesEntreHijos[i][PIPE_WRITE]);
		} else {
			close(pipesEntreHijos[i][PIPE_READ]);
			close(pipesEntreHijos[i][PIPE_WRITE]);
		}
	}
}

void cerrarPipesPadre(int pipeDesdePadre[2], int pipeHaciaPadre[2], int pipesEntreHijos[][2], int n){
	close(pipeDesdePadre[PIPE_READ]);
	close(pipeHaciaPadre[PIPE_WRITE]);
	for (int i = 0; i < n; i++)
	{
		close(pipesEntreHijos[i][PIPE_READ]);
		close(pipesEntreHijos[i][PIPE_WRITE]);
	}
}

void ejecutarHijo(int pipeDesdePadre[2], int pipeHaciaPadre[2], int pipesEntreHijos[][2], int indexHijo, int start, int n) {
	cerrarPipesHijo(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, indexHijo, start, n);

	/* COMPLETAR */

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
    
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer, start);
    
    /* Definimos los pipes */
    int pipeDesdePadre[2];
	int pipeHaciaPadre[2];
	int pipesEntreHijos[n][2];
	/* Creamos pipes */
	if(pipe(pipeDesdePadre) == -1) {
		printf("Error al crear pipe desde padre.");
		exit(EXIT_FAILURE);
	}
	if(pipe(pipeHaciaPadre) == -1) {
		printf("Error al crear pipe hacia padre.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < n; i++)
	{
		if(pipe(pipesEntreHijos[i]) == -1){
			printf("Error al crear pipes entre hijo %i y %i.", i, (i+1)%n);
			exit(EXIT_FAILURE);
		}
	}
	
	/* Creamos hijos */
	for (int i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == 0) {
			ejecutarHijo(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, i, start, n);
		} else if (pid < 0) {
			printf("Error al crear hijo %i", i);
			exit(EXIT_FAILURE);
		}
	}

	/* Cerramos los pipes que no usa el padre */
	cerrarPipesPadre(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, n);

	/* COMPLETAR */
	
	return 0;
}
