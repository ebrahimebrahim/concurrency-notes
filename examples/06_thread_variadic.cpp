/*
    std::thread is a variadic template, which means we can pass any number of arguments to it!
    The arguments passed in after the callable object are parameters to be passed to that object.
    Passing an incorrect type/number of parameters results in a compiler error.
    Normally these parameters are passed via copy or move (depending on whether they are lvalues or rvalues),
    but they can also be passed by reference using std::ref.
*/

#include <iostream>
#include <thread>

// TODO: Demonstrate all of this.