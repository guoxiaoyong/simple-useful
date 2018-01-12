#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main() {

    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(20));
    t.wait();
    std::cout << "Hello, world!\n";

    return 0;
}
