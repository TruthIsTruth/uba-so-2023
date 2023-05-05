#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_RANDOM 100
#define N 10

bool ganador = false;
int pipes[10][2];
int pidHijos[10];
int hijo;

int dameRandom() {
    return rand()%MAX_RANDOM;
}

void crearPipes() {
    for (int i = 0; i < N; ++i) {
        pipe(pipes[i]);
    }
}

void cerrarPipesParaPadre() {
    for (int i = 0; i < N; ++i){
        close(pipes[i][1]);
    }
}

void cerrarPipesParaHijo() {
    for (int i = 0; i < N; ++i){
        if (i != hijo) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }
    close(pipes[hijo][0]);
}

void handler() {
    ganador = true;
}

void ejecutarHijo() {
    signal(SIGTERM, handler);
    srand(getpid());
    cerrarPipesParaHijo();

    printf("[Hijo %d] Hola! Voy a jugar!! 😊\n", hijo);
    while (!ganador) {
        int numero = dameRandom();
        printf("[Hijo %d] Enviaré el número %d a mi padre...\n", hijo, numero);
        write(pipes[hijo][1], &numero, sizeof(numero));
        sleep(1);
    }

    printf("\033[1;93m[Hijo %d] 🎉 Gané!! 🥳 \033[0;39m\n", hijo);
    exit(EXIT_SUCCESS);
}

void crearHijos() {
    for (hijo = 0; hijo < N; hijo++) {
        pidHijos[hijo] = fork();
        if (pidHijos[hijo] == 0) {
            ejecutarHijo();
        }
    }
}

void jugarConHijos(int numeroSecreto) {
    int hijosVivos = N;
    int i = 0;

    while (hijosVivos > 1) {
        if (pidHijos[i] != 0) {
            printf("[Padre] Leyendo el número del hijo %d...\n", i);
            int numero;
            read(pipes[i][0], &numero, sizeof(numero));
            if(numero <= numeroSecreto) {
                printf("\033[1;92m[Padre] 👍 El hijo %d informó el número %d, así que sigue jugando! 👍 \033[0m\n", i, numero);
            } else {
                printf("\033[1;91m[Padre] 💀 El hijo %d informó el número %d, queda fuera del juego! 💀\033[0m\n", i, numero);
                hijosVivos--;
                kill(pidHijos[i], SIGKILL);
                //waitpid(pidHijos[i], NULL, 0);
                pidHijos[i] = 0;
                printf("[Padre] Quedan %d hijos jugando!\n", hijosVivos);
            }
        }
        i = (i+1)%N;
    }

    while(pidHijos[i] == 0) {
        i = (i+1)%N;
    }

    printf("\033[1;93m[Padre] 🏆 El hijo ganador es: %d!! 🏆\033[0;39m\n", i);
    kill(pidHijos[i], SIGTERM);
}

int crearNumeroSecreto() {
    int numeroSecreto = dameRandom()/2;
    printf("[Padre] El número secreto es %d\n", numeroSecreto);
    return numeroSecreto;
}

int main(int argc, char const *argv[]) {
    srand(time(NULL));

    crearPipes();
    crearHijos();
    cerrarPipesParaPadre();
    jugarConHijos(crearNumeroSecreto());
    return 0;
 }

    return 0;
}
