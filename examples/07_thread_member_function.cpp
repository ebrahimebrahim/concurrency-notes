/*
    To spawn a thread from a member function (other than operator()), you just pass
    the member function pointer to the std::thread constructor and then also pass
    the object on which to call the member function. You can pass the object by value
    (which would copy) or you can can pass a pointer to the object if you want to operate
    on that object. A smart pointer also works. Using a shared pointer is a good way
    to make sure that a heap-allocated object doesn't die before a thread is finished with it.
*/

#include <iostream>
#include <thread>
#include <memory>

class Peup {
    int id{5};

public:
    void double_id() { id*=2; }
    void set_id(int new_id) { id=new_id; }
    int get_id() {return id;}
};

int main() {

    Peup peup{}; // id starts as 5

    std::thread t1(&Peup::double_id, peup); // pass peup by value, this is a copy
    t1.join(); // id is still 5 after this
    std::cout << "after t1, peup.id is " << peup.get_id() << std::endl;

    std::thread t2(&Peup::double_id, &peup); // pass address of peup to operate on actual peup
    t2.join(); // id should double to 10
    std::cout << "after t2, peup.id is " << peup.get_id() << std::endl;

    std::thread t3(&Peup::set_id, &peup, 42); // arguments listed after object are function parameters
    t3.join(); // id should be 42
    std::cout << "after t3, peup.id is " << peup.get_id() << std::endl;

    std::shared_ptr<Peup> peup_ptr = std::make_shared<Peup>();
    std::thread t4(&Peup::set_id,peup_ptr,100);
    // Since a shared ptr is getting passed by value (copied), the underlying heap resource is guaranteed to survive at least till t4 finishes.
    t4.join();
    std::cout << "after t4, the peup_ptr id is " << peup_ptr->get_id() << std::endl;

    return 0;

}