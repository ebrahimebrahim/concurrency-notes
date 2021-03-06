/*
    A promise object has the potential to be filled with data.
    It has a corresponding future object which expects to "receive" the data eventually.
    This is a way of passing data between threads. 

    The future corresponding to a promise can be obtained via the "get_future" method.
    A promise cannot be copied, only moved. It is sort of like a unique_ptr in that regard.
    Once a promise sets the value of its resource via the "set_value" method,
    its future will be able to see that value.

    Call the "get" method of the future will block until a value is set by the promise,
    and then it will return that value. This is why it's useful in concurrent programming.
    The get method can only be called *once*. It is actually moving the data, not copying it.
*/

#include <iostream>
#include <thread>
#include <future>

/* (Unimportant comment: 
    In the following, we could pass by rvalue reference or by value.
    Either way we will have to std::move the promise that we make later on.
    The only difference will be in how many move constructors get called.
    There is disagreement about which way to go, but I think
    passing a move-only object by value is the cleaner way to communicate
    that this function is a sink for the object.)
*/  
void fill_in_forty_two(std::promise<int> a) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    a.set_value(42);
}

int main() {

    std::promise<int> fill_me_with_forty_two;
    std::future<int> ftr = fill_me_with_forty_two.get_future();

    std::thread t(fill_in_forty_two, std::move(fill_me_with_forty_two)); // promises have to be moved, not copied

    std::cout << "The worker thread has started; hopefully it fills our future bin with 42 ..." << std::endl;

    int hopefully_forty_two = ftr.get(); // blocks until a.set_value happens
    std::cout << "The promise has finally been fullfilled! Let's see what the future holds... we have " << hopefully_forty_two << std::endl;

    t.join();

    return 0;
}

/*
    If you want to handle an exception while setting a promise, you can actually "set_exception" rather than "set_value"
    in the promise. This will make send the exception along to the future and make the future throw it when its "get" finishes.
    So this way an exception from a worker thread could get handled in the main thread, for example.

    Besides "get", other useful methods of a std::future are "wait" and "wait_for",
    which you can look up.
*/