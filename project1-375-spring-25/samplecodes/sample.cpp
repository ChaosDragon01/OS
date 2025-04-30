// First, always include <windows.h> for all the Windows specific thread information

using namespace std; // This is a C++ program, so we'll use cout and endl
#include <windows.h>
#include <iostream>
#define MAX_THREADS 3
// Prototypes are good and handy, but not necessary in this example.
// These three functions are run by each of our three threads
// Please note how the functions are declared:
// In Windows, thread functions MUST be declared like this:
// DWORD WINAPI <name>(LPVOID)
// In short,
// Return value *must* be DWORD WINAPI
// And the parameter must be LPVOID
DWORD WINAPI genericThreadFunc1(LPVOID);
DWORD WINAPI printString(LPVOID);
DWORD WINAPI printNumber(LPVOID);

// We need an array of Handles to threads
HANDLE hThreads[MAX_THREADS];
// ...an array of thread id's
DWORD id[MAX_THREADS];
// And a waiter (which I'll explain later)
DWORD waiter;

// Here are the three functions that are defined.
// They do trivial things and should be mostly self explanatory.
DWORD WINAPI genericThreadFunc1(LPVOID n)
{
cout << "Thread started (genericThreadFunc1)..." << endl;
for(int i = 0; i < 100; i++) {
cout << "threadFunc1 says: " << i << endl;
}
cout << "...(genericThreadFunc1) Thread terminating." << endl;
return (DWORD)n;
}

DWORD WINAPI printString(LPVOID n)
{
cout << "Thread started (printString)..." << endl;
// NOTE: In the next line, we make a pointer and cast what was passed in.
// This is how you use the LPVOID parameters passed into the
// CreateThread call (below).
char* str = (char*)n;
for(int i = 0; i < 50; i++) {
cout << "printString says: " << str << endl;
}
cout << "...(printString) Thread terminating." << endl;
return (DWORD)n;
}

DWORD WINAPI printNumber(LPVOID n)
{
cout << "Thread started (printNumber)..." << endl;
int num = (int)n;
for (int i = num; i < (num + 100); i++) {
cout << "printNumber says: " << i << endl;
}
cout << "...(printHello) Thread terminating." << endl;
return (DWORD)n;
}

// Get ready, because here's where all the *REAL* magic happens
int main(int argc, char* argv[ ])
{
int CONSTANT = 2000;
char myString[20];
strcpy(myString,"Threads are Easy!");
// Here is where we call the CreateThread Windows API Function that actually
// creates and begins execution of a thread.
// Please read your help files for what each parameter does on
// your Operating system.
// Here's some basics:
// Parameter 0: Lookup
// Parameter 1: Stack size (0 is default which means 1MB)
// Parameter 2: The function to run with this thread
// Parameter 3: Any parameter that you want to pass to the thread function
// Parameter 4: Lookup
// Parameter 5: Once thread is created, an id is put in this variable passed in
hThreads[0] = CreateThread(NULL,0,genericThreadFunc1,(LPVOID)0,NULL,&id[0]);
hThreads[1] = CreateThread(NULL,0,printString,(LPVOID)myString,NULL,&id[1]);
hThreads[2] = CreateThread(NULL,0,printNumber,(LPVOID)CONSTANT,NULL,&id[2]);

// Now that all three threads are created and running, we need to stop
// the primary thread (which is this program itself - Remember that once
// "main" returns, our program exits)
// so that our threads have time to finish. To do this, we do what is
// called "Blocking".
// We're going to make main just stop and wait until all three threads
// are done.
// This is done easily with the next line of code. Please read the help
// file about the specific API call "WaitForMultipleObjects".
waiter = WaitForMultipleObjects(MAX_THREADS, hThreads, TRUE, INFINITE); 

// After all three threads have finished their task, "main" resumes and
// we're now ready to close the handles of the threads. This is just a
// bit of clean up work.
// Use the CloseHandle (API) function to do this. (Look it up in the
// help files as well)
for(int i = 0; i < MAX_THREADS; i++) {
    CloseHandle(hThreads[i]);
    }
    return 0;
    }