#include <iostream>
#include <thread>

int main() {
    std::cout << "Thread id = " << std::this_thread::get_id() << std::endl;

    return 0;
}
