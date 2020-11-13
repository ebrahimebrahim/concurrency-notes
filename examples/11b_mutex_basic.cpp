/*
    Now we introduce a mutex to save the day.
    main() is unchanged
*/

#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <memory>
#include <vector>

class WrappedInteger {

    int x{};
    mutable std::mutex mutex;
    // mutexes want to be mutable!
    // https://channel9.msdn.com/posts/C-and-Beyond-2012-Herb-Sutter-You-dont-know-blank-and-blank
    // If we didn't say mutable, then we would get a compile error from saying that the print method below is const

public:

    void increment() {
        // Let's pretend that a long computation with many intermediate steps
        // is needed to increment the underlying integer x. We simulate this as follows:

        mutex.lock(); // This will block the current thread until the mutex is an unlocked state so that this thread can lock it.
        int old_x = x;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        x = old_x + 1;
        mutex.unlock(); // Unlock the mutex, potentially unblocking a thread that might be waiting to lock it.

    }

    void print() const {
        mutex.lock();
        // print is reading and not writing data, so why lock?
        // It's because some other thread might be writing, and if we read while another thread
        // is writing then that is also a data race and also a potential bug.
        // It just happens not to happen in the main() of this particular example.
        std::cout << "The underlying integer is now: " << x << std::endl;
        mutex.unlock();
    }


};

int main() {

    auto wi = std::make_shared<WrappedInteger>();

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 1000; ++i)
        futures.emplace_back(std::async(std::launch::async, &WrappedInteger::increment, wi));
        // Try changing the async policy to std::launch::deferred to see that giving up concurrency fixes the problem
    
    
    for (auto & ftr : futures) ftr.wait();

    wi->print();



    return 0;
}
