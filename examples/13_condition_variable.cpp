/*
    Here is an implementation of a thread-safe message queue.
    It can be a shared resource among many threads who send messages to it.
    When anyone calls receive on it, they are blocked until there is something to receive.

    Read over the template class MessageQueue and then return here.

    First, why the mutex? Well, the MessageQueue is intended to be a shared object among multiple threads.
    So the mutex is there to guard data reading and writing and protect from data race.

    Now what's the deal with _cond?

    The idea is that when we call receive, we want to block until there is data to be received.
    We could poll for data in a while loop, but that is a waste of cpu.
    So instead we creat a std::condition_variable, which we can *wait* on and which can *notify* waiters to wake up.
    This is why there is a notify taking place in MessageQueue<T>::send

    Okay so how is the waiting working in MessageQueue<T>::receive?
    _cond.wait will first check the lambda function we gave it. If true, just move on and don't block.
    If false, then it will unlock the lock we gave it and block until a notify is received.
    This is a great feature of condition_variables, that they can both unlock a mutex and start listening for notify in one atomic move.
    (If we had tried to do this manually in two moves, then the message push_back and the notify could occur in between these two moves,
    which is a subtle and rare but horrible concurrency bug!)
    When a notify is finally recieved, the lambda condition will be checked again.
    If it's true, relock the mutex and unblock. If false, stay blocked.
    (Checking the lambda again helps things not break when there is a spurious wakeup-- the OS can randomly choose to wake up a thread sometimes.)

    Now the rest of this code should make sense. Read over everything, compile, and run.


*/


#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <algorithm>
#include <deque>
#include <memory>
#include <vector>
#include <string>


template<class T>
class MessageQueue {

    mutable std::mutex _mutex;
    mutable std::condition_variable _cond;
    std::deque<T> _messages;

public:

    T receive() {

        std::unique_lock<std::mutex> lock(_mutex);
        _cond.wait(lock, [this] { return !_messages.empty(); });

        T v = std::move(_messages.back());
        _messages.pop_back();

        return v; // will not be copied due to return value optimization
    }

    void send(T &&v) {
        std::lock_guard<std::mutex> lock(_mutex);
        _messages.push_back(std::move(v));
        _cond.notify_one(); 
    }
};


int main() {

    std::shared_ptr<MessageQueue<std::string>> mq;

    std::vector<std::future<void>> futures;

    futures.emplace_back(std::async(std::launch::async, [mq](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        mq->send("This message should arrive after two seconds.");
    } ));

    futures.emplace_back(std::async(std::launch::async, [mq](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mq->send("This message should arrive after one second.");
    } ));

    futures.emplace_back(std::async(std::launch::async, [mq](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        mq->send("This message should arrive after three seconds.");
    } ));

    // This is a contrived example where we know there will be exactly three messages.
    for (int i = 0; i < 3; ++i){
        std::string message = mq->receive();
        std::cout << "> " << message << std::endl;
    }

    // Join, out of principle, even though in this contrived example all three threads should be done by now.
    for (auto & future : futures)
        future.wait();


    return 0;
}