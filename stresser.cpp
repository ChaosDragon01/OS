#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

// Function to stress the CPU
void stress_cpu(atomic<bool>& running) {
    while (running) {
        // Perform heavy computation to maximize CPU usage
        volatile double x = 0;
        for (int i = 0; i < 100000000; i++) { // Increased iterations
            x += i * 0.0001;
        }
    }
}

int main() {
    int num_threads;
    cout << "Enter the number of threads to stress the CPU (recommend using all cores): ";
    cin >> num_threads;

    // Atomic flag to control the threads
    atomic<bool> running(true);

    // Create threads
    vector<thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(stress_cpu, ref(running));
    }

    cout << "Press Enter to stop the stress test..." << endl;
    cin.ignore(); // Clear the input buffer
    cin.get();    // Wait for Enter key

    // Stop the threads
    running = false;

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    cout << "Stress test stopped." << endl;
    return 0;
}