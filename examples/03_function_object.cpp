#include <iostream>
#include <thread>

class Peup {
public:
    void operator()();
};

void Peup::operator()() {
    std::cout << "a Peup instance has been called as though it were a function\n";
}

int main(){
    std::thread t{Peup()};
    std::cout << "Main done with it's work.\n";
    
    t.join();

    return 0;
}