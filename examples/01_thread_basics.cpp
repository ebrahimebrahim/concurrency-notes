#include <iostream>
#include <thread>

void function_to_run_in_a_thread() {
    std::cout << "Function thread id: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate work in function
    std::cout << "Done in function thread." << std::endl;
}

int main() {
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;
    std::cout << "Number of cores hint from system: " << std::thread::hardware_concurrency() << std::endl; 

    // create a thread
    // it starts in a "joinable" state, and will lose that state when it is done with execution
    std::thread t{function_to_run_in_a_thread};

    // simulate some work in the main thread
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Done in main thread." << std::endl;

    // Now if main returns before t is finished, then the destructor of t
    // will get called while t is in a joinable state.
    // This results in a call to "std::terminate", which shows up as an error somehow.
    // We can avoid this by calling t.join to wait for t to finish:

    // "join" returns when the thread execution has completed,
    // giving time for the function to finish it's work
    t.join();
    // After join returns, the thread t loses the property "joinable"
    // This means it no longer represents a thread of execution.

    // Try commenting out the join and see the error that comes out

    return 0;
}
