#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

void print(const boost::system::error_code&) {

    std::cout << "Hello, world!\n";
    std::cout << boost::this_thread::get_id() << std::endl;
}

int main() {

    std::cout << boost::this_thread::get_id() << std::endl;
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(20));
    t.async_wait(print);
    io.run();
    std::cout << "good morning!" << std::endl;
    return 0;
}
