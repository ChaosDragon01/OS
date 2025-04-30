#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int readers = 0, writers = 0;
bool readProcesscount[3] = {false, false, false};
bool writeProcesscount[2] = {false, false};

int getreadercount() {
    return readers;
}

int getwritercount() {
    return writers;
}

void setreadercount() {
    readers = 0;
    for (int i = 0; i < 3; i++) {
        if (readProcesscount[i]) readers++;
    }
}

void setwritercount() {
    writers = 0;
    for (int i = 0; i < 2; i++) {
        if (writeProcesscount[i]) writers++;
    }
}

void readerentry(int readerid) {
    if (getwritercount() == 0) {
        readProcesscount[readerid] = true;
        setreadercount();
        cout << "Reader " << ++readerid<< " entered critical section.\n";
    } else {
        cout << "Reader " << ++readerid<< " is waiting to enter.\n";
    }
}

void readerexit(int readerid) {
    readProcesscount[readerid] = false;
    setreadercount();
    cout << "Reader " << ++readerid << " exited critical section.\n";
}

void writerentry(int writerid) {
    if (getreadercount() == 0 && getwritercount() == 0) {
        writeProcesscount[writerid] = true;
        setwritercount();
        cout << "Writer " << ++writerid << " entered critical section.\n";
    } else {
        cout << "Writer " << ++writerid << " is waiting to enter.\n";
    }
}

void writerexit(int writerid) {
    writeProcesscount[writerid] = false;
    setwritercount();
    cout << "Writer " << ++writerid << " exited critical section.\n";
}

void checkcriticalsection() {
    if ((getreadercount() > 2) || (getreadercount() > 0 && getwritercount() > 0)) {
        cout << "🚨 Unstable critical section detected!\n";
    } else {
        cout << "✅ Critical section is safe.\n";
    }
}

void readprocess() {
    int readerid = rand() % 3;
    readerentry(readerid);
    cout << "Reader " << ++readerid<< " is reading...\n";
    readerexit(readerid);
}

void writeprocess() {
    int writerid = rand() % 2;
    writerentry(writerid);
    cout << "Writer " << ++writerid<< " is writing...\n";
    writerexit(writerid);
}

void process() {
    int processID = rand() % 5;
    if (processID < 3)
        readprocess();
    else
        writeprocess();
}

int main() {
    srand(time(0));
    for (int i = 0; i < 20; i++) {
        process();
        checkcriticalsection();
    }
    return 0;
}
