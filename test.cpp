#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

bool global_variable = false;
mutex m;
condition_variable cv;

void myMethod() {
    // do something
    cout << "Global variable is now true." << endl;
}

void waitForGlobalVariable() {
    unique_lock<mutex> lock(m);
    cv.wait(lock, []{ return global_variable; });
    myMethod();
}

int main() {
    thread t1(waitForGlobalVariable);

    // some code here that eventually changes the value of the global variable to true
    // for example, you could have another thread that sets the value of the global variable to true after some time

    {
        lock_guard<mutex> lock(m);
        global_variable = true;
    }

    cv.notify_all();
    t1.join();
    return 0;
}
