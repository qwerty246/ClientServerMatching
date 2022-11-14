#include "Session.h"
#include "Core.h"
#include "RequestHandler.h"
#include <hpp/Common.hpp>
#include <hpp/json.hpp>

using boost::asio::ip::tcp;

Session::Session(boost::asio::io_service& ioService) :
    _socket(ioService)
{
}

tcp::socket& Session::Socket()
{
    return _socket;
}

void Session::Start()
{
    _socket.async_read_some(boost::asio::buffer(_pData, eMaxLength),
                            boost::bind(&Session::HandleRead,
                                        this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}

void Session::HandleRead(const boost::system::error_code& error, size_t uBytesTransferred)
{
    if (!error)
    {
        _pData[uBytesTransferred] = '\0';

        auto json = nlohmann::json::parse(_pData);
        const std::string sReqType = json["ReqType"];

        std::string sReply = "Error! Unknown request type.\n";
        Core& core = Core::GetCore();
        if (sReqType == Requests::Registration)
        {
            const std::string sUsername = json["Username"];
            const std::string sPassword = json["Password"];
            sReply = RequestHandler::RegisterNewUser(sUsername, sPassword).dump();
        }
        else if (sReqType == Requests::Authorization)
        {
            const std::string sUsername = json["Username"];
            const std::string sPassword = json["Password"];
            sReply = RequestHandler::AuthorizeUser(sUsername, sPassword).dump();
        }
        else if (sReqType == Requests::RemoveUser)
        {
            const std::string sUsername = json["Username"];
            const std::string sPassword = json["Password"];
            sReply = RequestHandler::RemoveUser(sUsername, sPassword).dump();
        }
        else
        {
            size_t uUserId = std::stoi(std::string(json["UserId"]));
            UserPtr pUser = core.GetUser(uUserId);
            if (sReqType == Requests::ShowBalance)
            {
                sReply = RequestHandler::ShowBalance(pUser).dump();
            }
            else if (sReqType == Requests::ShowUserActiveOffers)
            {
                sReply = RequestHandler::ShowUserActiveOffers(pUser).dump();
            }
            else if (sReqType == Requests::ShowUserCompletedOffers)
            {
                sReply = RequestHandler::ShowUserCompletedOffers(pUser).dump();
            }
            else if (sReqType == Requests::ShowAllSaleOffers)
            {
                sReply = RequestHandler::ShowAllOffers(pUser, true).dump();
            }
            else if (sReqType == Requests::ShowAllPurchaseOffers)
            {
                sReply = RequestHandler::ShowAllOffers(pUser, false).dump();
            }
            else if (sReqType == Requests::ReplenishUSD)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                sReply = RequestHandler::ReplenishUSD(pUser, dAmount).dump();
            }
            else if (sReqType == Requests::ReplenishRUB)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                sReply = RequestHandler::ReplenishRUB(pUser, dAmount).dump();
            }
            else if (sReqType == Requests::RemoveUSD)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                sReply = RequestHandler::RemoveUSD(pUser, dAmount).dump();
            }
            else if (sReqType == Requests::RemoveRUB)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                sReply = RequestHandler::RemoveRUB(pUser, dAmount).dump();
            }
            else if (sReqType == Requests::SaleUSD)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                double dPrice = std::stod(std::string(json["Price"]));
                nlohmann::json reply;
                RequestHandler::AddOfferAndTryToMatch(reply, pUser, pUser->CreateOfferSale(dAmount, dPrice));
                sReply = reply.dump();
            }
            else if (sReqType == Requests::PurchaseUSD)
            {
                double dAmount = std::stod(std::string(json["Amount"]));
                double dPrice = std::stod(std::string(json["Price"]));
                nlohmann::json reply;
                RequestHandler::AddOfferAndTryToMatch(reply, pUser, pUser->CreateOfferPurchase(dAmount, dPrice));
                sReply = reply.dump();
            }
            else if (sReqType == Requests::RemoveOffer)
            {
                std::string sOfferId = json["OfferId"];
                sReply = RequestHandler::RemoveOffer(pUser, sOfferId).dump();
            }
        }

        boost::asio::async_write(_socket,
                                 boost::asio::buffer(sReply, std::size(sReply)),
                                 boost::bind(&Session::HandleWrite,
                                             this,
                                             boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void Session::HandleWrite(const boost::system::error_code& error)
{
    if (!error)
    {
        _socket.async_read_some(boost::asio::buffer(_pData, eMaxLength),
                                boost::bind(&Session::HandleRead,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete this;
    }
}
