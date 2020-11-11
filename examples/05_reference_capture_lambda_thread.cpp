/*
    The thread that captures a reference to id gets affected by a change that takes place
    after the thread is created but before it finishes executing!
    This is a common source of errors in concurrent programming.
*/

#include <iostream>
#include <thread>

int main(){

    int id = 0;

    auto lambda_capturing_by_reference  = [&id]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "ID captured by reference is " << id << std::endl;
    };

    auto lambda_capturing_by_value  = [id]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "ID captured by value is " << id << std::endl;
    };

    std::thread t1(lambda_capturing_by_reference); // We start executing t1 here
    std::thread t2(lambda_capturing_by_value);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    id = 42; // But this will actually affect t1!

    t1.join();
    t2.join();

    

    return 0;
}