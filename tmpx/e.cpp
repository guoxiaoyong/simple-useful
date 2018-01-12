#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

class HelloWorld {

public:
    void hello() {

        std::cout<<"Hello world, I'm a thread!"<<std::endl;
    }

    void start() {

        boost::function0<void> f = boost::bind(&HelloWorld::hello, this);
        boost::thread thrd(f);
        thrd.join();
    }
};


int main() {

    HelloWorld hello;
    hello.start();

    return 0;
}
