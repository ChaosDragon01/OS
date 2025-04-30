#include <windows.h>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_THREADS 2

// Function prototypes
DWORD WINAPI producer(LPVOID);
DWORD WINAPI consumer(LPVOID);

// Global variables
queue<int> buffer;
int bufferSize;
int counterLimit;
int counter = 0;
HANDLE hMutex;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <buffer size> <counter limit>" << endl;
        return 1;
    }

    bufferSize = atoi(argv[1]);
    counterLimit = atoi(argv[2]);

    HANDLE hThreads[MAX_THREADS];
    DWORD id[MAX_THREADS];

    // Initialize the mutex
    hMutex = CreateMutex(NULL, FALSE, NULL);

    // Create producer and consumer threads
    hThreads[0] = CreateThread(NULL, 0, producer, NULL, 0, &id[0]);
    hThreads[1] = CreateThread(NULL, 0, consumer, NULL, 0, &id[1]);

    // Wait for both threads to finish
    WaitForMultipleObjects(MAX_THREADS, hThreads, TRUE, INFINITE);

    // Close thread handles and mutex
    for (int i = 0; i < MAX_THREADS; i++) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(hMutex);

    return 0;
}

DWORD WINAPI producer(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        WaitForSingleObject(hMutex, INFINITE);

        if (counter >= counterLimit) {
            ReleaseMutex(hMutex);
            break;
        }

        if (buffer.size() < bufferSize) {
            int num = rand() % 100;
            buffer.push(num);
            cout << "Produced: " << num << endl;
            counter++;
        }

        ReleaseMutex(hMutex);
        Sleep(100); // Simulate work
    }
    return 0;
}

DWORD WINAPI consumer(LPVOID lpParam) {
    while (true) {
        WaitForSingleObject(hMutex, INFINITE);

        if (counter >= counterLimit && buffer.empty()) {
            ReleaseMutex(hMutex);
            break;
        }

        if (!buffer.empty()) {
            int num = buffer.front();
            buffer.pop();
            cout << "Consumed: " << num << endl;
            counter++;
        }

        ReleaseMutex(hMutex);
        Sleep(150); // Simulate work
    }
    return 0;
}