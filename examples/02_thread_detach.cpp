#include <iostream>
#include <thread>

void function_to_run_in_a_thread() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate work in function
    std::cout << "Done in function thread." << std::endl;
}

int main() {

    // create a thread
    std::thread t{function_to_run_in_a_thread};

    // simulate some work in the main thread
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Done in main thread." << std::endl;

    // This time we avoid std::terminate in a messier way:
    // detaching a thread simply turns off the joinable property
    // It does not wait for the thread to finish.
    // This way there's no error even though t gets killed before it finishes
    // This is a very bad thing. Don't do it.
    // Don't detach unless you are supplying your own mechanism to allow the thread to complete.

    t.detach();

    return 0;
}
