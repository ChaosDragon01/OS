#include "Semaphore.h"

Semaphore::Semaphore(int init) : value(init) {}  // Constructor initializes the semaphore value to the given initial value (default is 1)
// The constructor initializes the semaphore value to the given initial value (default is 1)    

void Semaphore::wait() { // This function acts as a lock, it decrements the semaphore value if it's greater than 0
    // If the value is 0, it indicates that the resource is not available, and the function will block until it becomes available
    if (value > 0) value--;  // Decrement the semaphore value if the value is greater than 0 when this ffunction is called
    else {
     cout << "[PANIC] Wait failed: Semaphore already at 0\n"; // Print an error message if the semaphore value is already 0
    }
}


void Semaphore::signal() {
    value++; // Increment the semaphore value to indicate that the resource is now available
}


int Semaphore::getVal() const {
    return value;
}
