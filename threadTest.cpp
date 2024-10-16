#include <iostream>
#include <thread>

void myfunc() {
    std::cout << "myfunc\n";
    // do something ...
}

int main() {
    thread t1(myfunc);
    t1.join();
    return 0;
}