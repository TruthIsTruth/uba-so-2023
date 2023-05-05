#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>


int main(){
    pid_t pid = fork();
    if (pid == 0) { //Hijo
        printf("Soy Julieta\n\0");
        sleep(1);
        pid = fork();
        if (pid == 0) {
            printf("Soy Jennifer\n\0");
            sleep(1);
            exit(EXIT_SUCCESS);
        } else {
            exit(EXIT_SUCCESS);
        }

    } else { //Padre
        printf("Soy Juan\n\0");
        sleep(1);
        wait(NULL);
        pid = fork();
        if (pid == 0)
        {
            printf("Soy Jorge\n");
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        else
        {
            exit(EXIT_SUCCESS);
        }
        
    }
}