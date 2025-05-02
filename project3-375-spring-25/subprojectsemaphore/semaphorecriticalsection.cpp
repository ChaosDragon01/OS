#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

/* I am not using any form of implementation-specific libraries for this. 
Initially this part of the project was created with separate implementations.
File paths:
- main.cpp
- main.exe
- makefile
- Semaphore.cpp
- Semaphore.h
- semaphorecriticalsection.cpp

All from: project3-375-spring-25/subprojectsemaphore/

I’ve now combined everything into one file for simplicity and to avoid submission issues.
*/




class Semaphore {  
/*
This is the semaphore class I initialized for the binary/counting semaphore part.
It allows mutual exclusion and synchronization between processes.
It has methods for waiting and signaling, and for getting the current value.
*/
private:
    int value;
public:
    Semaphore(int init = 1) : value(init) {}

    void wait() {
        if (value > 0) {
            value--;
        } else {
            cout << "[PANIC] Wait failed: Semaphore already at 0" << endl;
        }
    }

    void signal() {
        value++; // Increment the semaphore value
    }

    int getVal() const {
        return value; // Return the current value of the semaphore
    }
};


/*
These are the global variables and semaphores used in the program.
They are used to keep track of the number of readers and writers,
as well as their program counters (to simulate one instruction per cycle).
*/

Semaphore mutex(1);       // Binary semaphore for mutual exclusion
Semaphore write(1);         // Binary semaphore for writer lock
Semaphore readCount(2);   // Counting semaphore to allow up to 2 readers

int readercount = 0;
int writercount = 0;
bool readProcesscount[3] = {false, false, false};
bool writeProcesscount[2] = {false, false};
int pcReader[3] = {0};
int pcWriter[2] = {0};


/*
This function checks for violations of the critical section rules.
- Panic if both reader(s) and writer(s) are inside simultaneously.
- Panic if there are more than 2 readers in the critical section.
*/
void checkCriticalSectionState() { 
    if (readercount > 2 || (readercount > 0 && write.getVal() < 1)) {
        cout << "[PANIC] Critical section violation: ";
        if (readercount > 2) cout << "More than 2 readers detected. ";
        if (readercount > 0 && write.getVal() < 1) cout << "Reader and Writer co-existing.";
        cout << endl;
    } else {
        cout << "Critical section is stable." << endl;
    }
}


/*
This is the reader function that simulates a reader process.
It uses a switch to simulate one instruction per cycle.
It handles entry, reading, and exit.
*/
void reader(int id) {
    if (!readProcesscount[id] && writercount == 0 && readCount.getVal() > 0) {
        readCount.wait();
        readercount++;
        readProcesscount[id] = true;
        cout << "Reader " << id + 1 << " entered the critical section." << endl;
    } else if (readProcesscount[id]) {
        cout << "Reader " << id + 1 << " is reading..." << endl;
        readCount.signal();
        readercount--;
        readProcesscount[id] = false;
        cout << "Reader " << id + 1 << " exited the critical section." << endl;
    } else {
        cout << "Reader " << id + 1 << " is waiting..." << endl;
    }
}



/*
This is the writer function that simulates a writer process.
It uses a switch to simulate one instruction per cycle.
It handles entry, writing, and exit.
*/
void writer(int id) {
    if (!writeProcesscount[id] && readercount == 0 && writercount == 0 && write.getVal() > 0) {
        write.wait();
        writercount++;
        writeProcesscount[id] = true;
        cout << "Writer " << id + 1 << " entered the critical section." << endl;
    } else if (writeProcesscount[id]) {
        cout << "Writer " << id + 1 << " is writing..." << endl;
        write.signal();
        writercount--;
        writeProcesscount[id] = false;
        cout << "Writer " << id + 1 << " exited the critical section." << endl;
    } else {
        cout << "Writer " << id + 1 << " is waiting..." << endl;
    }
}


int main() {
    srand(time(0));  // Seed for random number generation

    for (int i = 0; i < 20; i++) {
        int proc = rand() % 5; // 0-2: readers, 3-4: writers, 5: randomized reader or writer

        switch (proc) {
            case 0:
                if (pcReader[0] < 3) reader(0);
                break;
            case 1:
                if (pcReader[1] < 3) reader(1);
                break;
            case 2:
                if (pcReader[2] < 3) reader(2);
                break;
            case 3:
                if (pcWriter[0] < 3) writer(0);
                break;
            case 4:
                if (pcWriter[1] < 3) writer(1);
                break;
            case 5: {
                int r_or_w = rand() % 2; // 0 = reader, 1 = writer
                if (r_or_w == 0) {
                    int r_id = rand() % 3;
                    if (pcReader[r_id] < 3) reader(r_id);
                } else {
                    int w_id = rand() % 2;
                    if (pcWriter[w_id] < 3) writer(w_id);
                }
                break;
            }
        }

        checkCriticalSectionState();
    }

    return 0;
}


