#include <thread>
#include <iostream>
#include <vector>
#include <numeric>
#include <iterator>
#include <atomic>

#define N 100
#define THREAD_COUNT 5

using namespace std;

atomic_int pos(0);

void producto_interno_concurrente(vector<int> &v1, vector<int> &v2, atomic_int &res) {
    int i;
    while ((i = pos++) < N) {
        res += v1[i] * v2[i];
    }
}

int main() {
    vector<int> v1(N);
    vector<int> v2(N);
    iota(v1.begin(), v1.end(), 1);
    iota(v2.begin(), v2.end(), N + 1);

    atomic_int res(0);
    vector<thread> threads;
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back(producto_interno_concurrente, ref(v1), ref(v2), ref(res));
    }

    for (auto &t: threads) {
        t.join();
    }

    cout << "El resultado es " << res << endl;

    return 0;
}


