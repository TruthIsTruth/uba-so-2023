#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>

#define CANT_THREADS 10

using namespace std;

void f() {
    this_thread::sleep_for(500ms);
    puts("Hola! Soy un thread");
}

int main(){
    vector<thread> threads;

    for (int i = 0; i < CANT_THREADS; i++)
        threads.emplace_back(f);

    for (auto &t: threads){
        //Opcion 1
        //t.join();
        //Opcion 2
        //t.detach();
    }

    return 0;
}