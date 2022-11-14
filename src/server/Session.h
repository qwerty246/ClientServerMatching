#pragma once

#include <string>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session
{
public:
    Session(boost::asio::io_service& ioService);
    ~Session() = default;

    tcp::socket& Socket();

    void Start();
    void HandleRead(const boost::system::error_code& error, size_t uBytesTransferred);
    void HandleWrite(const boost::system::error_code& error);
private:
    tcp::socket _socket;
    enum
    {
        eMaxLength = 1024
    };
    char _pData[eMaxLength];
};
