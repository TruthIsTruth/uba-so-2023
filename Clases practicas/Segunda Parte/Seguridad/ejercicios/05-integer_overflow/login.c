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

bool validate_password(const char *input) {
    char password[128];
    char input_len = strlen(input);

    if(input_len<128) {
    	strcpy(password, input);
    	printf("Password is: %s\n", password);
    } else {
    	printf("Error: password lenght should be less than 32 chars.");
    }

    return strcmp(password, "porfis")==0;
}

int main(int argc, char const *argv[]) {
	char password2[999];
    if(setuid(0) == -1) {
        printf("setUID ERROR\n");
        return -1;
    }

    if(argc < 2) {
    	printf("Use: ./login password\n");
    	return 1;
    }

    if(validate_password(argv[1])) {
        login_ok();
    } else {
        login_fail();
    }
    return 0;
}

