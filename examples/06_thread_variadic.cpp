/*
    std::thread is a variadic template, which means we can pass any number of arguments to it!
    The arguments passed in after the callable object are parameters to be passed to that object.
    Passing an incorrect type/number of parameters results in a compiler error.
    Normally these parameters are passed via copy or move (depending on whether they are lvalues or rvalues),
    but they can also be passed by reference using std::ref.
*/

#include <iostream>
#include <thread>

void f1(int a) {
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    std::cout << "I have an integer: " << a << std::endl;
}

void f2(int a, char b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "I have an integer and a char: " << a << ", " << b << std::endl;
}

void f3(int & a) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "I am f3. I shall change my integer from " << a << " to " << ++a << std::endl;
}

int main(){

    std::thread t1(f1,42);
    std::thread t2(f2,74,'k');

    // std::thread t4(f1,"peup"); // ERROR: wrong parameter type for f1
    // std::thread t5(f2,74); // ERROR: wrong number of parameters for f2

    int x = 5;

    std::thread t3(f3,std::ref(x));

    // std::thread t4(f3,x); // ERROR: f3 wants to take x by reference


    t1.join();
    t2.join();

    x=8;

    std::cout << "Main is done with t1 and t2. Main just set x to " << x << std::endl;

    t3.join();

    std::cout << "x is now " << x << std::endl;

    return 0;
}