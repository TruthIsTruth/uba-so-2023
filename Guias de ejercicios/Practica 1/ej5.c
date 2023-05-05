#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

void wait_for_child(pid_t child) {
    waitpid(child, NULL, 0);
}

void Homer() {    
    int pid = fork();
    if (pid == 0)
    {
        printf("Soy Bart\n");
    } else {
        pid = fork();
        if (pid == 0) {
            printf("Soy Lisa\n");
        } else {
            pid = fork();
            if (pid == 0) {
                printf("Soy Maggie\n");
            } else {
                wait_for_child(pid);
                printf("Soy Homer\n");
            }
        }
        
    }
}

int main() {
    int pid = fork();
    if (pid == 0) {
        Homer();
    } else {
        wait_for_child(pid);
        printf("Soy Abraham\n");
    }
    return 0;
}