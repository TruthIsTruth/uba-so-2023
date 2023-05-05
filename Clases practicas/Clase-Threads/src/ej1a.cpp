#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

void f() {
    this_thread::sleep_for(500ms);
    puts("Hola! Soy un thread");
}

int main(){
    for (int i = 0; i < 10; i++)
        new thread(f);

    sleep(1);
    return 0;
}