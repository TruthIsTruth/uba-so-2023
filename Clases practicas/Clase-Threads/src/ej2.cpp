//Semaforo "viejito" segun la profe
//  #include<semaphore.h>
//  sem_init(&sem,0,0)
//Binary semaphore
//  #include<semaphore>
//  binary_semaphore sem1(0);
//  sem1.release();
//  sem1.acquire();

#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <semaphore.h>

using namespace std;

#define MSG_COUNT 5

sem_t sem_a;
sem_t sem_b;

void f1_a() {
    for (int i = 0; i < MSG_COUNT; ++i) {
        cout << "Ejecutando F1 (A)\n";
        this_thread::sleep_for(100ms);
    }
}
void f1_b() {
    for (int i = 0; i < MSG_COUNT; ++i) {
        cout << "Ejecutando F1 (B)\n";
        this_thread::sleep_for(200ms);
    }
}
void f2_a() {
    for (int i = 0; i < MSG_COUNT; ++i) {
        cout << "Ejecutando F2 (A)\n";
        this_thread::sleep_for(500ms);
    }
}
void f2_b() {
    for (int i = 0; i < MSG_COUNT; ++i) {
        cout << "Ejecutando F2 (B)\n";
        this_thread::sleep_for(10ms);
    }
}

void f1(){
    f1_a();
    sem_post(&sem_a);
    sem_wait(&sem_b);
    f1_b();
}
void f2(){
    f2_a();
    sem_post(&sem_b);
    sem_wait(&sem_a);
    f2_b();
}

int main(){
    sem_init(&sem_a, 0, 0);
    sem_init(&sem_b, 0, 0);

    thread t1 = thread(f1);
    thread t2 = thread(f2);
    
    t1.join();
    t2.join();

    return 0;
}