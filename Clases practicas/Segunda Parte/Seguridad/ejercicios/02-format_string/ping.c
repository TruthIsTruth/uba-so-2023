#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int BUFFERSIZE = 512;

int main(int argc, char** argv) {
    char command[BUFFERSIZE+1];
    char *ipaddr_sanitized = strtok(argv[1], " ;&|()");
    snprintf(command, BUFFERSIZE, "ping -c 4 %s", ipaddr_sanitized);

    if(setuid(0) == -1) {
        perror("setUID ERROR");
    }


    printf("Executing: '%s'\n", command);
    system(command);
    return 0;
}