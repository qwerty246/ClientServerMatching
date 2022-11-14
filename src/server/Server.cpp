#include "Server.h"
#include <hpp/Common.hpp>

Server::Server(boost::asio::io_service& ioService) :
    _ioService(ioService),
    _acceptor(ioService, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started! Listen " << port << " port" << std::endl;

    Session* pNewSession = new Session(_ioService);
    _acceptor.async_accept(pNewSession->Socket(),
                           boost::bind(&Server::HandleAccept,
                                       this,
                                       pNewSession,
                                       boost::asio::placeholders::error));
}

void Server::HandleAccept(Session* pNewSession, const boost::system::error_code& error)
{
    if (!error)
    {
        pNewSession->Start();
        pNewSession = new Session(_ioService);
        _acceptor.async_accept(pNewSession->Socket(),
                               boost::bind(&Server::HandleAccept,
                                           this,
                                           pNewSession,
                                           boost::asio::placeholders::error));
    }
    else
    {
        delete pNewSession;
    }
}
