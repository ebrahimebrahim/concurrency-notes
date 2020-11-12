/*
    Spawning threads is called "forking" the process.
    Using threads follows the concept of "fork-join parallelism"
    
    Threads cannot be copied, only moved. This is why emplace_back is needed here.
*/

#include <iostream>
#include <thread>
#include <vector>

int main() {

    std::vector<std::thread> threads;

    // fork into 5 threads
    for (size_t i = 0; i < 5; ++i)
        threads.emplace_back(std::thread([](){ // push_back wouldn't work
            std::cout << "I am worker thread #" << std::this_thread::get_id() << std::endl;
        }));
    
    std::cout << "I am main thread, with id #" << std::this_thread::get_id() <<std::endl;

    // join
    for (auto & t : threads)
        t.join();

    return 0;
}