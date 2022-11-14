#pragma once

#include "Session.h"

#include <iostream>

class Server
{
public:
    Server(boost::asio::io_service& ioService);
    ~Server() = default;

    void HandleAccept(Session* pNewSession, const boost::system::error_code& error);
private:
    boost::asio::io_service& _ioService;
    tcp::acceptor _acceptor;
};
