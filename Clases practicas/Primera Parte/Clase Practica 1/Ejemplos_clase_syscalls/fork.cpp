#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void padre(){
    sleep(1);
    char mensaje[] = "Soy el proceso padre\n";
    write(1, mensaje, sizeof(mensaje));    
}

void hijo(){
  sleep(1);
  char mensaje[] = "Soy el proceso hijo\n";
  write(1, mensaje, sizeof(mensaje));
  
}

int main(int argc, char const *argv[]){
  pid_t pid = fork();
  if(pid == 0){
    hijo();
  } else {
    padre();
  }
  exit(EXIT_SUCCESS); //cada uno finaliza su proceso

}
