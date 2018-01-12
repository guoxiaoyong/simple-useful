#include <boost/thread/thread.hpp>
#include <iostream>

class HelloWorld {

public:
    static void hello()
    {
        std::cout<<"Hello world, I'm a thread!"<<std::endl;
    }
    static void start()
    {
        boost::thread thrd(hello);
        thrd.join();
    }
};

int main() {

    HelloWorld::start();
    return 0;
}
