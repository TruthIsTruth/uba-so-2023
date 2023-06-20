#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void login_ok() {
    printf("Login granted.\n");
    system("/bin/sh");
}

void login_fail() {
    printf("Login failed, password was not valid\n");
}

bool validate_password() {
    char password[32];

    printf("Insert your password: ");
    scanf("%s", password);

    printf("Password is: ");
    printf(password);
    printf("\n");

    return strcmp(password, "porfis")==0;
}

int main(int argc, char const *argv[]) {
    if(setuid(0) == -1) {
        perror("setUID ERROR\n");
    }

    if(validate_password()) {
        login_ok();
    } else {
        login_fail();
    }
    return 0;
}

