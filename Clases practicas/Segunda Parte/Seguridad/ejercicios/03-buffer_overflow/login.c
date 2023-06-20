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

struct login_data_t {
    char password[100];
    bool valid;
} login_data;

void validate_password() {
    login_data.valid = false;

    printf("Insert your password: ");
    scanf("%s", login_data.password);

    printf("Password is: ");
    printf(login_data.password);
    printf("\n");

    if(strcmp(login_data.password, "porfis")==0) {
        login_data.valid = 1;
    }
}

int main(int argc, char const *argv[]) {
    if(setuid(0) == -1) {
        perror("setUID ERROR");
    }

    validate_password();

    if(login_data.valid) {
        login_ok();
    } else {
        login_fail();
    }
    return 0;
}