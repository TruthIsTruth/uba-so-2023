#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <semaphore.h>
#include <numeric> //Para la función iota

using namespace std;

int N = 100;
int THREAD_COUNT = 5;
vector<int> v1(N);
vector<int> v2(N);
vector<int> v3(N);

void f(int start, int end){
    for (int i = start; i < end; i++)
    {
        v3[i] = v1[i] * v2[i];
    }
}


int main() {
    iota(v1.begin(), v1.end(), 1);
    iota(v1.begin(), v1.end(), N + 1);

    vector<thread> threads;
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int step = N / THREAD_COUNT;
        int start = i * step;
        int end = start + step;

        threads.emplace_back(f, start, end);
    }
    
    for (auto &t : threads) {
        t.join();
    }

    return 0;
}