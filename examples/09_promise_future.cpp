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

void fill_in_forty_two(std::promise<int> && a) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    a.set_value(42);
}

int main() {

    std::promise<int> fill_me_with_forty_two;
    std::future<int> f = fill_me_with_forty_two.get_future();

    std::thread t(fill_in_forty_two, std::move(fill_me_with_forty_two)); // promises have to be moved, not copied

    std::cout << "The worker thread that should be sending us 42 has started..." << std::endl;

    int hopefully_forty_two = f.get(); // blocks until a.set_value happens
    std::cout << "The promise has finally been fullfilled! We have: " << hopefully_forty_two << std::endl;

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