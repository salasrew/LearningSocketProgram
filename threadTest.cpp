#include <iostream>
#include <thread>

void myfunc() {
    std::cout << "myfunc\n";
    // do something ...
}

int main() {
    std::thread t1(myfunc);
    t1.join();

    system("pause");
    return 0;
}