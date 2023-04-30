#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador = 0;

void signalURG_handler1(int sig){
	printf("ya va!\n");
	contador++;
}

void signalINT_handler1(int sig){
	wait(NULL);
}

int main(int argc, char* argv[]) {
	printf("%s", argv[0]);
	printf("%s", argv[1]);
	printf("%s", argv[2]);

  	pid_t pid = fork();
	if (pid != 0) { //Proceso padre
		signal(SIGINT, signalINT_handler1);
		for (int i = 0; i < 5; i++)
		{
			sleep(1);
			printf("sup!\n");
			kill(pid, SIGURG);
		}
		
	} else { //Proceso hijo
		signal(SIGURG, signalURG_handler1);
		while(contador < 5);
		kill(getppid(), SIGINT);
		
		execvp(argv[1], argv + 1);
	}



	return 0;
}
