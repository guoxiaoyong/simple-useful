#include <boost/thread/thread.hpp>
#include <iostream>

class HelloWorld
{
public:
    void hello()
    {
        std::cout<<"Hello world, I'm a thread!"<<std::endl;
    }
    static void start()
    {
        boost::thread thrd(boost::bind(&HelloWorld::hello, &HelloWorld::getInstance()));
        thrd.join();
    }
    static HelloWorld& getInstance()
    {
        if(!instance)
            instance = new HelloWorld;
        return *instance;
    }
private:
    HelloWorld() {}
    static HelloWorld* instance;
};

HelloWorld* HelloWorld::instance = 0;
int main()
{
    HelloWorld::start();

    system("pause");
    return 0;
}