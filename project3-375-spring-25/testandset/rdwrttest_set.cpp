#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


bool lockFlag = false;
/*
This function simulates a test-and-set operation on a boolean variable.
It takes a reference to a boolean variable as input and sets it to true.
*/

/* Please ignore the atrovious amounts of comments, I remember in one class session you mentioned "I better see alot of comments"*/


/*
This function simulates a test-and-set operation on a boolean variable.
It takes a reference to a boolean variable as input and sets it to true.
It returns the old value of the variable before it was set.
This is a simple implementation of the test-and-set operation, which is often used in synchronization algorithms.
*/
bool testAndSet(bool& target) {
    bool old = target;
    target = true;
    return old;
}



int readers = 0, writers = 0; // Global variables to keep track of the number of readers and writers in the critical section
bool readProcesscount[3] = {false, false, false}; // Process count for readers
bool writeProcesscount[2] = {false, false}; // Process count for writers

int pcReader[3] = {0};  // Process count for readers
int pcWriter[2] = {0};  // Process count for writers




/*
This is the function that will be called by the processes to enter the critical section.
It will check if the process can enter the critical section or not.
If the process can enter, it will set the lockFlag to true and increment the process count.
*/
void readerentry(int id) {
    if (!writeProcesscount[0] && !writeProcesscount[1] && readers < 2 && !testAndSet(lockFlag)) {
        readProcesscount[id] = true;
        readers++;
        cout << "Reader " << id++ << " entered the critical section."<< endl ;
        lockFlag = false;
        pcReader[id]++;
    } else {
        cout << "Reader " << id++ << " is waiting..." << endl ;
    }
}



/*
This function will be called by the reader process after it enters the critical section.
It will print a message to the console that the reader is reading the data.
It will also increment the process counter for the reader.
*/
void readerread(int id) {
    cout << "Reader " << id++<< " is reading..."<< endl ;
    pcReader[id]++;
}

/*
this is the function that will be called by the processes to exit the critical section.
It will check if the process can exit the critical section or not.
If the process can exit, it will set the lockFlag to true and decrement the process count.
*/
void readerexit(int id) {
    if (!testAndSet(lockFlag)) {
        readProcesscount[id] = false;
        readers--;
        cout << "Reader " << id++ << " exited the critical section." << endl ;
        lockFlag = false;
        pcReader[id] = 0;
    } else {
        cout << "Reader " << id++ << " is waiting to exit..." << endl ;
    }
}

void reader(int id) {
    switch (pcReader[id]) {
        case 0: readerentry(id); break;
        case 1: readerread(id); break;
        case 2: readerexit(id); break;
    }
}

/*
This is the function that will be called by the processes to enter the critical section.
It will check if the process can enter the critical section or not.
If the process can enter, it will set the lockFlag to true and increment the process count.
*/
void writerentry(int id) {
    if (readers == 0 && writers == 0 && !testAndSet(lockFlag)) { // Check if no readers or writers are in the critical section and lockFlag is false
        writeProcesscount[id] = true; // Set the process count for the writer
        writers++; // Increment the writer count
        cout << "Writer " << id++<< " entered the critical section." << endl ; // Print message to console
        lockFlag = false; // Set the lockFlag to false to allow other processes to enter
        pcWriter[id]++; // Increment the process count for the writer
    } else {
        cout << "Writer " << id++<< " is waiting..." << endl ; // Print message to console if the writer cannot enter the critical section
    }
}

void writerwrite(int id) { // This function will be called by the writer process after it enters the critical section
    cout << "Writer " << id++<< " is writing..." << endl ; // Print message to console that the writer is writing data
    pcWriter[id]++; // Increment the process count for the writer
}


/*
this is the function that will be called by the processes to exit the critical section.
It will check if the process can exit the critical section or not.
If the process can exit, it will set the lockFlag to true and decrement the process count.
*/
void writerexit(int id) { // This function will be called by the writer process after it exits the critical section
    if (!testAndSet(lockFlag)) { // Check if the lockFlag is false
        writeProcesscount[id] = false; // Set the process count for the writer to false
        writers--; // Decrement the writer count
        cout << "Writer " << id++<< " exited the critical section." << endl ;
        lockFlag = false; // Set the lockFlag to false to allow other processes to enter
        pcWriter[id] = 0; // Reset the process count for the writer
    } else { // Print message to console if the writer cannot exit the critical section
        cout << "Writer " << id++<< " is waiting to exit..." << endl ; // Print message to console if the writer cannot exit the critical section
    }
}




/*
This function implements a writer process, which enters the critical section,
writes data, and then exits the critical section. The function takes one parameter,
the id of the writer process, and uses a switch to simulate one instruction per cycle.
It handles entry, writing, and exit.
*/
void writer(int id) {
    switch (pcWriter[id]) {
        case 0: writerentry(id); break;
        case 1: writerwrite(id); break;
        case 2: writerexit(id); break;
    }
}

/*
This function checks the critical section for any instability.
It checks if there are any readers or writers in the critical section.
If there are more than 2 readers or if there is a reader and a writer in the critical section, it will print a panic message.
*/
void checkcriticalsection() {
    if ((readers > 0 && writers > 0) || readers > 2) {
        cout << "[PANIC] Unstable Critical Section: too many readers or reader+writer collision!" << endl;
    } else {
        cout << "Critical section is stable." << endl;
    }
}

int main() {
    srand(time(0)); // Seed Random Number Generator
    
    for (int i = 0; i < 10; i++) {
        int proc = rand() % 5; // 0–2 = readers, 3–4 = writers

        switch (proc) {
            case 0: // Randomly select a reader process
                reader(0); 
                break;
            case 1:
                reader(1);
                break;
            case 2:
                reader(2);
                break;
            case 3:
                writer(0);
                break;
            case 4:
                writer(1);
                break;
            default:
                cout << "[PANIC] Invalid process ID generated: " << proc << endl; // This should never happen
                break;
        }

        checkcriticalsection(); // panic checker here
    }

    return 0;
}
