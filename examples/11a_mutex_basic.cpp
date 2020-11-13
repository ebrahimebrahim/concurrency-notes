/*
    Mutexes provide a way to enforce that a shared resource cannot be read/written-to while another thread is writing to it.
    This helps prevent data races.

    The program below has a data race. Read through it, then compile and run it and observe that the result varies each time.
*/

#include <iostream>
#include <thread>
#include <future>
#include <memory>
#include <vector>

class WrappedInteger {

    int x{};

public:

    void increment() {
        // Let's pretend that a long computation with many intermediate steps
        // is needed to increment the underlying integer x. We simulate this as follows:

        int old_x = x;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        x = old_x + 1;

    }

    void print() const {
        std::cout << "The underlying integer is now: " << x << std::endl;
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
