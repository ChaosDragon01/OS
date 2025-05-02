#include <iostream> // for cout and endl commands we inlude iostream
using namespace std; // we use the standard namespace to avoid writing std:: before every command

class Semaphore {  /// Semaphore class definition, for the semaphore object
private:
    int value; // the value of the semaphore, which is an integer
    // The semaphore value is used to control access to a shared resource
public:
    Semaphore(int init = 1); // constructor with default value of 1
    // The constructor initializes the semaphore value to the given initial value (default is 1)
    void wait(); // this functions as a lock, it decrements the semaphore value if it's greater than 0
    // If the value is 0, it indicates that the resource is not available, and the function will block until it becomes available
    // This is similar to a mutex lock, which is used to protect shared resources in concurrent programming
    // The wait() function is used to acquire the semaphore, blocking if necessary

    void signal(); // this function increments the semaphore value, indicating that the resource is now available
    // This is similar to releasing a mutex lock, this function is used to release the semaphore, allowing other threads to acquire it
    // also it's like the counter/opposite of the wait() function


    int getVal() const;// this function returns the current value of the semaphore
};