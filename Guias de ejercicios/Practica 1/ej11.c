#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<stdbool.h>

void share_mem(int *ptr);
void share_mem(int *ptr, int size);
void bsend(pid_t dst, int msg);
int breceive(pid_t src);

int num = 0;

void hijo1(pid_t pid_p){
    int pid_h2 = breceive(pid_p);
    while (true){
        breceive(pid_p);
        if (num > 50) break;
        num = num + 1;
        bsend(pid_h2, 0);
    }
    exit(EXIT_SUCCESS);
}

void hijo2(pid_t pid_p){
    int pid_h1 = breceive(pid_p);
    while (true) {
        breceive(pid_h1);
        if (num > 50) break;
        num = num + 1;
        bsend(pid_p, 0);
    }
    exit(EXIT_SUCCESS);
}

void padre(pid_t pid_h1, pid_t pid_h2){
    bsend(pid_h1, pid_h2);
    bsend(pid_h2, pid_h1);
    bsend(pid_h1, 0);
    while (true) {
        breceive(pid_h2);
        if (num > 50) break;
        num = num + 1;
        bsend(pid_h1, 0);
    }
}

int main() {
    share_mem(&num);
    pid_t pid_p = get_current_pid();
    pid_t pid_h1 = fork();
    if (pid_h1 == 0)
    {
        hijo1(pid_p);
    }
    pid_t pid_h2 = fork();
    if (pid_h2 == 0)
    {
        hijo2(pid_p);
    }
    padre(pid_h1, pid_h2);
    
    return 0;
}