#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
using boost::asio::ip::tcp;

class client
{
public:
    client(boost::asio::io_service& io_service,tcp::endpoint& endpoint)
        : socket(io_service)//这里就把socket实例化了
    {
        //连接服务端 connect
        socket.async_connect(endpoint,
                             boost::bind(&client::handle_connect,this,boost::asio::placeholders::error)
                            );
        memset(getBuffer,0,1024);
    }
    ~client()
    {}
private:
    void handle_connect(const boost::system::error_code& error)
    {
        if(!error) {

            boost::asio::async_write(socket, boost::asio::buffer("hello,server"), boost::bind(&client::handle_write,this,boost::asio::placeholders::error));
            socket.async_read_some(boost::asio::buffer(getBuffer,1024), boost::bind(&client::handle_read,this,boost::asio::placeholders::error));

        }
        else
        {
            socket.close();
        }
    }
    void handle_read(const boost::system::error_code& error)
    {
        if(!error)
        {
            std::cout << getBuffer << std::endl;
            //boost::asio::async_read(socket,
            //         boost::asio::buffer(getBuffer,1024),
            //         boost::bind(&client::handle_read,this,boost::asio::placeholders::error)
            //        );

            //这样就可以实现循环读取了，相当于while（1）
            //当然，到了这里，做过网络的朋友就应该相当熟悉了，一些逻辑就可以自行扩展了
            //想做聊天室的朋友可以用多线程来实现
            socket.async_read_some(
                boost::asio::buffer(getBuffer,1024),
                boost::bind(&client::handle_read,this,boost::asio::placeholders::error)
            );
        }
        else
        {
            socket.close();
        }
    }
    void handle_write(const boost::system::error_code& error)
    {
    }

private:
    tcp::socket socket;
    char getBuffer[1024];
};

int main(int argc,char* argv[])
{
    //if(argc != 3)
    //{
    // std::cerr << “Usage: chat_client <host> <port>\n”;
    //    return 1;
    //}

    //我觉IO_SERVICE是一个基本性的接口，基本上通常用到的类实例都需要通过它来构造
    //功能我们可以看似socket
    boost::asio::io_service io_service;
    //这个终端就是服务器
    //它的定义就可以看作时sockaddr_in,我们用它来定义IP和PORT
    tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string("192.168.1.119"/*argv[1]*/),8100/*argv[2]*/);
    //既然socket和sockaddr_in已经定义好了，那么，就可以CONNECT了
    //之所以为了要把连接和数据处理封成一个类，就是为了方便管理数据,这点在服务端就会有明显的感觉了
    boost::shared_ptr<client> client_ptr(new client(io_service,endpoint));
    //执行收发数据的函数
    io_service.run();
    return 0;
}

