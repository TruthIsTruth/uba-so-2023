#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdbool.h>
/*
void share_mem(int *ptr);
void share_mem(int *ptr, int size);
void bsend(pid_t dst, int msg);
int breceive(pid_t src);
*/
#define PIPE_READ 0
#define PIPE_WRITE 1

#define PIPE_PtoH1 0
#define PIPE_H1toH2 1
#define PIPE_H2toP 2

int pipes[3][2]; //Usamos 3 pipes

void hijo1(){
    close(pipes[PIPE_H2toP][PIPE_READ]);
    close(pipes[PIPE_H2toP][PIPE_WRITE]);
    close(pipes[PIPE_PtoH1][PIPE_WRITE]);
    close(pipes[PIPE_H1toH2][PIPE_READ]);

    int num;
    while (true){
        read(pipes[PIPE_PtoH1][PIPE_READ], &num, sizeof(num));
        printf("Hijo 1 lee %d de Padre.\n", num);
        if (num >= 50) break;
        num = num + 1;
        write(pipes[PIPE_H1toH2][PIPE_WRITE], &num, sizeof(num));
        printf("Hijo 1 envía %d a Hijo 2.\n", num);
    }
    exit(EXIT_SUCCESS);    
}

void hijo2(){
    close(pipes[PIPE_PtoH1][PIPE_READ]);
    close(pipes[PIPE_PtoH1][PIPE_WRITE]);
    close(pipes[PIPE_H1toH2][PIPE_WRITE]);
    close(pipes[PIPE_H2toP][PIPE_READ]);

    int num;
    while (true) {
        read(pipes[PIPE_H1toH2][PIPE_READ], &num, sizeof(num));
        printf("Hijo 2 lee %d de Hijo 1.\n", num);
        if (num >= 50) break;
        num = num + 1;
        write(pipes[PIPE_H2toP][PIPE_WRITE], &num, sizeof(num));
        printf("Hijo 2 envía %d a Padre.\n", num);
    }
    exit(EXIT_SUCCESS);
}

void padre(){
    close(pipes[PIPE_H1toH2][PIPE_READ]);
    close(pipes[PIPE_H1toH2][PIPE_WRITE]);
    close(pipes[PIPE_H2toP][PIPE_WRITE]);
    close(pipes[PIPE_PtoH1][PIPE_READ]);

    int num = 0;
    write(pipes[PIPE_PtoH1][PIPE_WRITE], &num, sizeof(num));
    printf("Padre envía %d a Hijo 1.\n", num);
    while (true) {
        read(pipes[PIPE_H2toP][PIPE_READ], &num, sizeof(num));
        printf("Padre lee %d de Hijo 2.\n", num);
        if (num >= 50) break;
        num = num + 1;
        write(pipes[PIPE_PtoH1][PIPE_WRITE], &num, sizeof(num));
        printf("Padre envía %d a Hijo 1.\n", num);
    }
}

int main() {
    for (int i = 0; i < 3; i++) {
        pipe(pipes[i]);
    }

    if (fork() == 0) {
        hijo1();
    }
    if (fork() == 0) {
        hijo2();
    }

    padre();
    
    return 0;
}