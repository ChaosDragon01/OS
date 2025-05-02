// main.cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Semaphore.h"

using namespace std;

Semaphore mutex(1);
Semaphore wrt(1);
Semaphore readCount(2);

int rc = 0; // reader count
int pcReader[3] = {0};
int pcWriter[2] = {0};

void reader(int id) {
    switch (pcReader[id]) {
        case 0:
            if (wrt.getVal() > 0 && readCount.getVal() > 0) {
                readCount.wait();
                rc++;
                cout << "Reader " << ++id << " entered\n";
                pcReader[id]++;
            } else {
                cout << "Reader " << ++id << " waiting...\n";
            }
            break;
        case 1:
            cout << "Reader " << id++<< " is reading...\n";
            pcReader[id]++;
            break;
        case 2:
            rc--;
            readCount.signal();
            cout << "Reader " << id++<< " exited\n";
            pcReader[id] = 0;
            break;
    }
}

void writer(int id) {
    switch (pcWriter[id]) {
        case 0:
            if (rc == 0 && wrt.getVal() > 0) {
                wrt.wait();
                cout << "Writer " << id + 1 << " entered\n";
                pcWriter[id]++;
            } else {
                cout << "Writer " << id + 1 << " waiting...\n";
            }
            break;
        case 1:
            cout << "Writer " << id + 1 << " is writing...\n";
            pcWriter[id]++;
            break;
        case 2:
            wrt.signal();
            cout << "Writer " << id + 1 << " exited\n";
            pcWriter[id] = 0;
            break;
    }
}

int main() {
    srand(time(0));
    for (int i = 0; i < 20; i++) {
        int proc = rand() % 5; // 0-2 for readers, 3-4 for writers
        if (proc <= 2) reader(proc);
        else writer(proc - 3);
    }
    return 0;
}
