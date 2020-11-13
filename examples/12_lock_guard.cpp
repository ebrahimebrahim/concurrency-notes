/*
    Actually it is usually best to avoid direct calls to lock!
    Neglecting to unlock a mutex can lead to *deadlock*, which is very bad.
    And it happens easily:
    - if you accidentally return early before you get to the unlock
    - if an exception is raised while under the lock

    Another form of deadlock can occur when various threads wait for each other to release resources before releasing their own resources.
    
    One way to mitigate the first form of deadlock is to use a lock_guard object.
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

public:

    void increment() {
        // Let's pretend that a long computation with many intermediate steps
        // is needed to increment the underlying integer x. We simulate this as follows:

        std::lock_guard<std::mutex> lck(mutex); // Construtor of lock_guard automatically locks the mutex.
        int old_x = x;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        x = old_x + 1;
        
        // No need to manually unlock now! Destructor of lock_guard will do it.

    }

    void print() const {
        std::lock_guard<std::mutex> lck(mutex); // Construtor of lock_guard automatically locks the mutex.
        std::cout << "The underlying integer is now: " << x << std::endl;
        // No need to manually unlock now! Destructor of lock_guard will do it.
    }


};

int main() {

    auto wi = std::make_shared<WrappedInteger>();

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 1000; ++i)
        futures.emplace_back(std::async(std::launch::async, &WrappedInteger::increment, wi));
    
    
    for (auto & ftr : futures) ftr.wait();

    wi->print();



    return 0;
}
