#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string> 
#include <random>
#include <windows.h>
using namespace std;

// global variables are going to be stored here

string numbersletters[62] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                            "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
                            "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
                            "U", "V", "W", "X", "Y", "Z","a", "b", "c", "d", 
                            "e", "f", "g", "h", "i", "j","k", "l", "m", "n",
                             "o", "p", "q", "r", "s", "t","u", "v", "w", "x", "y", "z"}; // array of numbers and letters



string criticalsection = ""; 


void updatecriticalsection() {
    int length = 10; // Length of the string you want
    string randomStr = "";

    for (int i = 0; i < length; ++i) {
        int index = rand() % 62; // Random index from 0 to 61
        randomStr += numbersletters[index]; // Append random character
    }
    criticalsection = randomStr; // update the critical section with the new data
    cout << "Critical section updated by writer: " << criticalsection << endl; 
}


int readers = 0, writers = 0; 

bool readProcesscount[3] = {false, false, false}; // number of processes that have read the data
bool writeProcesscount[2] = {false, false}; // number of processes that have written the data

// int  prosesscount[6] = {0, 0, 0, 0, 0, 0}; // number of processes 

int processID = 0; // process ID for the current process



int getreadercount (){
    return readers; // return the number of readers in the critical section
}
int getwritercount (){
    return writers; // return the number of writers in the critical section
}
void increasereadercount (int readerid){
        if (readProcesscount[readerid]){ 
            readers++; // increment the number of readers in the critical section
        }  
    }


void decreasereadercount (int readerid){
        if (!readProcesscount[readerid]){ 
            readers--; // increment the number of readers in the critical section
        }  
    }


void increasewritercount (int writerid){
        if (writeProcesscount[writerid]){ 
        writers++; // increment the number of writers in the critical section
        }
    }

void decreasewritercount (int writerid){
        if (!writeProcesscount[writerid]){ 
            writers--; // increment the number of readers in the critical section
        }  
    }



void readerentry(int readerid) {
    // Entry section for readers
    if (getwritercount() == 0 && getreadercount()<=2) { // check if there are no writers or more than 2 readers in the critical section
        readProcesscount[readerid] = true; // mark the reader as having read the data  
        increasereadercount(readerid); // increment the number of readers in the critical section
    cout << "Reader " << ++readerid << " has entered the critical section. " << endl;
    Sleep(1); // simulate the time taken to read the data
    //cout << "Reader " << readerid << " is reading the data: " << criticalsection << endl; // print the data being read
    (criticalsection=="") ? cout << "There is no data in the critical section." << endl : cout << "Reader " << readerid << " is reading the data: " << criticalsection << endl; // print the data being read
    } else {
    cout << "Reader " << ++readerid<< " is waiting to enter the critical section." <<endl;
    }
}

void readerexit(int readerid) {
    // Exit section for readers
    if(readProcesscount[readerid]){
    readProcesscount[readerid] = false; // mark the reader as having exited the critical section
    
    decreasereadercount(readerid); // decrement the number of readers in the critical section

    cout << "Reader " << ++readerid<< " has exited the critical section." <<endl;
    } 
    else if 
    (!readProcesscount[readerid])
    {
    cout << "Reader" << ++readerid << " doesn't exist in critical section " <<endl; 
    }
}




void writerentry(int writerid) {
    // Entry section for writers
    if (getreadercount() == 0 && getwritercount() == 0) { // check if there are no readers or writers in the critical section
        writeProcesscount[writerid] = true; // mark the writer as having written the data
        increasewritercount(writerid); // increment the number of writers in the critical section
        cout << "Writer " << ++writerid << " has entered the critical section. " << endl;
        Sleep(1); // simulate the time taken to write the data
        updatecriticalsection(); // update the critical section with new data 
    } else if (!writeProcesscount[writerid]){
        cout << "Writer " << ++writerid << " is waiting to enter the critical section." <<endl;
    }
}

void writerexit(int writerid) {
    // Exit section for writers
    if (writeProcesscount[writerid]){
    writeProcesscount[writerid] = false;
    decreasewritercount(writerid); // decrement the number of writers in the critical section
    cout << "Writer " << ++writerid << " has exited the critical section." <<endl;
    } else {
    cout << "Writer " << ++writerid<< " isn't in the critical section. " <<endl;
    }
}


void checkcriticalsection() {
    // Check if the critical section is empty
    if (getreadercount() > 0 && getwritercount() > 0 || getreadercount() > 2) {
        cout << "Unstable Critical Section, More than 2 readers or both reader and writer are co-existing" <<endl;
    } else {
        cout << "Critical Section is in safe process" <<endl;
    }
}


/*
void readprocess(int readerStatus) {
    int readerid = rand() % 2; // generate a random reader ID
    readerentry(readerid); // call the entry section for the reader
 
 
    cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl; // print the number of readers and writers to keep count for testing if the thing is working
  
  
    cout << "Reader " << readerid + 1 << " is reading the data." <<endl;
    // Simulate reading data
    readerexit(readerid); // call the exit section for the reader
  
  
  
    cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl; // print the number of readers and writers to keep count for testing if the thing is working
}
void writeprocess(int writerStatus) {   
    int writerid = rand() % 1; // generate a random writer ID
    writerentry(writerid); // call the entry section for the writer
   
    cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl; // print the number of readers and writers to keep count for testing if the thing is working

    cout << "Writer " << writerid + 1<< " is writing the data." <<endl;
    // Simulate writing data

    cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl; // print the number of readers and writers to keep count for testing if the thing is working

    writerexit(writerid); // call the exit section for the writer
}
*/


void process(){
    
    int processid = rand() % 5; // generate a random process ID
    processID = processid; // set the process ID for the current proc

    int randreader = rand() % 2;
    int randwriter = rand() % 1; 

    switch (processID)
    {   // The professor said I had only used 5 switch cases, but I used 6, 0 is the first case.
        
    case 0:
        //cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        //readProcesscount[randreader]=true;
        if (!readProcesscount[randreader])
        readerentry(randreader);
        else
        readerexit(randreader);

        break;
    case 1:
        //cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        if (readProcesscount[randreader])
        readerexit(randreader);
        else
        readerentry(randreader);
        break;
    case 2: 
        //cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        if (!writeProcesscount[randwriter])
        writerentry(randwriter); 
        else
        writerexit(randwriter);
    case 3:
        // cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        if (writeProcesscount[randwriter])
        writerexit(randwriter);
        else
        writerentry(randwriter);
    case 4:
        // cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        if (!readProcesscount[randreader])
        readerentry(randreader);
        else
        readerexit(randreader);
    case 5:
        //cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;
        if (writeProcesscount[randwriter])
                writerexit(randwriter);
        else 
                writerentry(randwriter);
    default:
        break;
    }






    // this doesn't work really as intended. 
   /* switch (processID) {
        case 0:
        case 1:
        case 2: {
            int randomnumber = russianroulette(2);
            readprocess(randomnumber);
        break;
        }
    
        case 3:
        case 4: {
            int randomnumber = russianroulette(1);
            writeprocess(randomnumber);
        break;
        }
        case 5:
        case 6: {
            int gamble = rand()%1;
            switch (gamble)
            {
            case 0:{
                int randomnumber = russianroulette(2);
                readprocess(randomnumber);
            break;
            }
            case 1:{
                int randomnumber = russianroulette(1);
                writeprocess(randomnumber);
            break;
            }
            
            default:
                break;
            }
        }
    
        default:
            break;
    }  */
 } 






int main(){
    srand(time(0)); // seed the random number generator with the current time

    int MAX_P = 6; 

    for (int i =0; i <= MAX_P; i++) {
        cout << "Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl; // print the number of readers and writers to keep count for testing if the thing is working
        process();
        // cout << "Process ID: " << processID + 1 << " Readers: " << readers << " readers, " << "Writers: " << writers << " writers" << endl;  // test case two 
        checkcriticalsection();
    }
   
    return 0; 

    
}

