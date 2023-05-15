#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <semaphore.h>
#include <numeric> //Para la función iota
#include <atomic>

using namespace std;

int N = 100;
int THREAD_COUNT = 5;
vector<int> p_por_thread(THREAD_COUNT, 0);
atomic_int pos(0);

void f(vector<int> &v1, vector<int> &v2, vector<int> &v3, int t_i){
    int i;
    while ((i = pos++) < N){
        v3[i] = v1[i] * v2[i];
        p_por_thread[t_i]++;
        this_thread::sleep_for(100ms * (t_i + 1));
    }
}

int main() {
    vector<int> v1(N);
    vector<int> v2(N);
    vector<int> v3(N);
    iota(v1.begin(), v1.end(), 1);
    iota(v1.begin(), v1.end(), N + 1);

    vector<thread> threads;
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        threads.emplace_back(f, ref(v1), ref(v2), ref(v3), i);
    }
    
    for (auto &t : threads) {
        t.join();
    }

    return 0;
}