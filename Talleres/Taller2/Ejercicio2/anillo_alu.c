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

void ejecutarPrimerHijo(int pipeDesdePadre[2], int pipeHaciaPadre[2], int pipesEntreHijos[][2],
				int start, int n)
{
	//Recibir numero entero de padre
	int nroRecibido;
	read(pipeDesdePadre[PIPE_READ], &nroRecibido, sizeof(int));
	// printf("DEBUG: Recibí el nro %i de padre. Soy el proceso %i.\n", nroRecibido, start);
	//Generar numero secreto
	int nroSecreto = nroRecibido; //Inicializo en el mismo de padre para entrar en el while
	while (nroSecreto <= nroRecibido) {
		nroSecreto = generate_random_number();
	}
	// printf("DEBUG: El numero secreto es %i.\n", nroSecreto);
	//Iniciamos la ronda
	int sucesor = start;
	int antecesor = start - 1;
	if (antecesor < 0) antecesor += n;

	nroRecibido++;
	write(pipesEntreHijos[sucesor][PIPE_WRITE], &nroRecibido, sizeof(int));
	// printf("DEBUG: Envié el nro %i al proceso %i. Soy el proceso %i.\n", nroRecibido, (start+1)%n, start);
	while (true) { 
		read(pipesEntreHijos[antecesor][PIPE_READ], &nroRecibido, sizeof(int));
		// printf("DEBUG: Recibí el nro %i del proceso %i. Soy el proceso %i.\n", nroRecibido, antecesor,start);
		if (nroRecibido >= nroSecreto) break;
		nroRecibido++;
		write(pipesEntreHijos[sucesor][PIPE_WRITE], &nroRecibido, sizeof(int));
		// printf("DEBUG: Envié el nro %i al proceso %i. Soy el proceso %i.\n", nroRecibido, (start+1)%n,start);
	}

	//Enviamos el último valor obtenido a padre
	write(pipeHaciaPadre[PIPE_WRITE], &nroRecibido, sizeof(int));
	// printf("DEBUG: Envío al padre el número %i, que fue el último valor recibido.\n", nroRecibido);
	exit(EXIT_SUCCESS);
}

void ejecutarRestoDeHijos(int pipesEntreHijos[][2], int indexHijo, int n){
	//Inicializamos variables
	int nroRecibido;
	int sucesor = indexHijo;
	int antecesor = indexHijo - 1;
	if (antecesor < 0) antecesor += n;

	//Ejecutamos la ronda hasta que el pipe del antecesor se cierre
	while (0 != read(pipesEntreHijos[antecesor][PIPE_READ], &nroRecibido, sizeof(int)))
	{
		// printf("DEBUG: Recibí el nro %i del proceso %i. Soy el proceso %i.\n", nroRecibido, antecesor, indexHijo);
		nroRecibido++;
		write(pipesEntreHijos[sucesor][PIPE_WRITE], &nroRecibido, sizeof(int));
		// printf("DEBUG: Envié el nro %i al proceso %i. Soy el proceso %i.\n", nroRecibido, (sucesor+1)%n, indexHijo);
	}
	// printf("DEBUG: Proceso %i salió del while con éxito.\n", indexHijo);
	exit(EXIT_SUCCESS);	
}

void ejecutarHijos(int pipeDesdePadre[2], int pipeHaciaPadre[2], int pipesEntreHijos[][2],
				 int indexHijo, int start, int n) 
{
	cerrarPipesHijo(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, indexHijo, start, n);
	if (start == indexHijo) {
		ejecutarPrimerHijo(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, start, n);
	} else {
		ejecutarRestoDeHijos(pipesEntreHijos, indexHijo, n);
	}
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
		printf("Error al crear pipe desde padre.\n");
		exit(EXIT_FAILURE);
	}
	if(pipe(pipeHaciaPadre) == -1) {
		printf("Error al crear pipe hacia padre.\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < n; i++)
	{
		if(pipe(pipesEntreHijos[i]) == -1){
			printf("Error al crear pipes entre hijo %i y %i.\n", i, (i+1)%n);
			exit(EXIT_FAILURE);
		}
	}
	
	/* Creamos hijos */
	for (int i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == 0) {
			ejecutarHijos(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, i, start, n);
		} else if (pid < 0) {
			printf("Error al crear hijo %i\n", i);
			exit(EXIT_FAILURE);
		}
	}

	/* Cerramos los pipes que no usa el padre */
	cerrarPipesPadre(pipeDesdePadre, pipeHaciaPadre, pipesEntreHijos, n);

	/* Enviamos al proceso indicado el numero que esta en buffer*/
	write(pipeDesdePadre[PIPE_WRITE], &buffer, sizeof(int));
	/* Esperamos el mensaje final*/
	read(pipeHaciaPadre[PIPE_READ], &buffer, sizeof(int));
	printf("El número final es: %i\n", buffer);
	
	return 0;
}
