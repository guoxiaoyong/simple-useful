#include <boost/thread/thread.hpp>
#include <iostream>
#include <string>

class HelloWorld
{
public:
    void hello(const std::string& str)
    {
        std::cout<<str<<std::endl;
    }
};

int main()
{
    HelloWorld obj;
    boost::thread thrd(boost::bind(&HelloWorld::hello, &obj, "Hello World, I'm a thread!"));
    thrd.join();

    return 0;
}
