/*
    This is a redo of example 09, except using tasks (async objects) instead of threads.
    Major differences:
    - an async will return a future object that (eventually) contains the return value of the callable it was given
    - an async will pass any exceptions along into that future object, so that they get thrown on "get"
    - an async does not need to be joined at the end. there is a built in join in its destructor
    - an async does not necessarily execute in a separate thread. It could spawn a thread, or it could just
      do lazy evaluation, deferring evaluation till get is called on the future.
      
    You can set a policy to force it into one behavior-- just pass std::launch bitmask as the first argument to async.
    The default policy, which allows the system to decide, is usually a great option, because it will avoid overloading
    the system with threads if it happens to be in a situation where more threads are bad.
*/

#include <iostream>
#include <thread>
#include <future>

int return_forty_two() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 42;
}

int main() {

    std::future<int> ftr = std::async(return_forty_two);
    // If the function took parameters, we could list them after return_forty_two
    // If you want to force a policy (std::launch::async or std::launch::deferred) then pass it as first parameter

    std::cout << "The worker task has started; hopefully it fills our future bin with 42 ..." << std::endl;

    int hopefully_forty_two = ftr.get(); // blocks until a.set_value happens
    std::cout << "The future has arrived! Let's see what the future holds... we have " << hopefully_forty_two << std::endl;

    return 0;
}

// Prefer async over thread for simple tasks, to avoid the promise-future boilerplate and to allow the system to optimize thread pooling
// For anything complicated (e.g. where data needs to be shared between threads while they run, or where you play with mutexes and locks),
// prefer threads because they are the more low level and more general thing.