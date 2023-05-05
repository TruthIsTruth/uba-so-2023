#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

int pipes[2];

void hijo1(){
    close(pipes[PIPE_READ]);
    dup2(pipes[PIPE_WRITE], STDOUT_FILENO);
    execlp("ls", "ls", "-al", NULL);
}

void hijo2(){
    close(pipes[PIPE_WRITE]);
    dup2(pipes[PIPE_READ], STDIN_FILENO);
    execlp("wc", "wc", "-l", NULL);
}

int main(){
    if(pipe(pipes) < 0){
        printf("Pipe creation failed.");
        return 1;
    }
    int pid = fork();
    if (pid == 0){
        hijo1();
    }
    pid = fork();
    if (pid == 0){
        hijo2();
    }
    return 0;
}