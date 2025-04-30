#include <windows.h>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_THREADS 2

// Prototypes for the Producer and Consumer functions
// The Producer function is responsible for adding items to the buffer
// The Consumer function is responsible for removing items from the buffer
DWORD WINAPI producer(LPVOID);
DWORD WINAPI consumer(LPVOID);

// Global buffer to store items
// Global variable to store the buffer size
queue<int> buffer;

// Global lock to protect the buffer
volatile bool bufferLock = false; // Volatile to prevent compiler optimization

// Structure to pass parameters to the Producer and Consumer functions
// The structure contains the buffer size, counter limit, and pointers to the producer and consumer counters
struct ThreadParams {
    int bufferSize;
    int counterLimit;
    int* producerCounter;
    int* consumerCounter;
};

// Main function to create Producer and Consumer threads
int main() {
    // This is to allow the user to input buffer size and counter limit
    int bufferSize, counterLimit;
    cout << "Enter buffer size: ";
    cin >> bufferSize;
    cout << "Enter counter limit: ";
    cin >> counterLimit;

    // Initialize the counters to 0
    int producerCounter = 0;
    int consumerCounter = 0;

    // Create a ThreadParams structure to pass parameters to the Producer and Consumer functions
    ThreadParams params = { bufferSize, counterLimit, &producerCounter, &consumerCounter };

    // Array to store thread handles and thread IDs
    HANDLE hThreads[MAX_THREADS];
    DWORD id[MAX_THREADS];

    // Create producer and consumer threads and store their handles
    hThreads[0] = CreateThread(NULL, 0, producer, &params, 0, &id[0]);
    hThreads[1] = CreateThread(NULL, 0, consumer, &params, 0, &id[1]);

    // Wait for both threads to finish
    WaitForMultipleObjects(MAX_THREADS, hThreads, TRUE, INFINITE);

    // Closes thread handles
    for (int i = 0; i < MAX_THREADS; i++) {
        CloseHandle(hThreads[i]);
    }

    // end main function 
    return 0;
}

// Producer function
DWORD WINAPI producer(LPVOID lpParam) {
    // Cast the lpParam to ThreadParams
    ThreadParams* params = (ThreadParams*)lpParam;
    // Seed the random number generator
    srand(time(NULL));

    // Producer loop
    while (true) {
        // Busy-wait for lock
        while (bufferLock);

        bufferLock = true; // Add buffer lock

        // Check if the total count of produced and consumed items has reached the counter limit
        if (*(params->producerCounter) + *(params->consumerCounter) >= params->counterLimit) {
            bufferLock = false; // Release lock
            break; // Exit loop
        }

        // Check if the buffer is not full
        if (buffer.size() < params->bufferSize) {
            // Generate a random number and add it to the buffer
            int num = rand() % 100;
            // Add the number to the buffer
            buffer.push(num);
            cout << "Produced: " << num << endl; // Print the number
            // Increment the producer counter
            (*(params->producerCounter))++;
        }

        bufferLock = false; // Release lock
        Sleep(100); // Simulate work
    }
    return 0;
}

// Consumer function
DWORD WINAPI consumer(LPVOID lpParam) {
    // Cast the lpParam to ThreadParams
    ThreadParams* params = (ThreadParams*)lpParam;
    // Consumer loop
    while (true) {
        // Busy-wait for lock
        while (bufferLock);

        bufferLock = true; // Add buffer lock

        // Check if the total count of produced and consumed items has reached the counter limit and the buffer is empty
        if (*(params->producerCounter) + *(params->consumerCounter) >= params->counterLimit && buffer.empty()) {
            bufferLock = false; // Release lock
            break; // ends the loop
        }

        // Check if the buffer is not empty
        if (!buffer.empty()) {
            // Remove a number from the buffer
            int num = buffer.front();
            buffer.pop();
            cout << "Consumed: " << num << endl; // Print the number
            // Increment the consumer counter
            (*(params->consumerCounter))++;
        }

        bufferLock = false; // Release lock
        Sleep(150); // Simulate work
    }
    return 0;
}