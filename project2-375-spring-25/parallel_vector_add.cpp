#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: ./parallel_vector_addition <n> <threads>" << endl;
        return -1;
    }

    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    vector<int> A(n), B(n), C(n);

    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
        B[i] = n - i;
    }

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        int start = thread_id * n / total_threads;
        int end = (thread_id + 1) * n / total_threads - 1;
        if (thread_id == total_threads - 1) {
            end = n - 1;
        }

        #pragma omp critical
        {
            cout << "Thread " << thread_id << " is working on elements " << start << " to " << end << endl;
        }

        for (int i = start; i <= end; i++) {
            C[i] = A[i] + B[i];
        }
    }

    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;
    
    bool is_correct = true;
    #pragma omp parallel for num_threads(num_threads) reduction(&& : is_correct)
    for (int i = 0; i < n; i++) {
        if (C[i] != 10001) {
            is_correct = false;
        }
    }

    if (is_correct) {
        cout << "Vector addition successful! All values in C are 10001." << endl;
    } else {
        cout << "Error: Not all values in C are 10001!" << endl;
    }

    cout << "Parallel version took " << execution_time << " seconds." << endl;

    return 0;
}
