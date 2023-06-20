#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int BUFFERSIZE = 512;

int main(int argc, char** argv) {
    char command[BUFFERSIZE+1];
    snprintf(command, BUFFERSIZE, "ping -c 4 %s", argv[1]);

    if(setuid(0) == -1) {
        perror("setUID ERROR");
    }

    
    printf("Executing: '%s'\n", command);
    system(command);

    return 0;
}

