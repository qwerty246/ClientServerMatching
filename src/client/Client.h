#pragma once

#include <hpp/Common.hpp>
#include <hpp/json.hpp>

#include <iostream>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
    Client(tcp::socket* socket);
    ~Client();

    void SendMessage(const nlohmann::json& req);
    std::string ReadMessage();
    bool RegistrationProcess();
    void MainProcess();
private:
    short GetMenuOptionNum() const;
    double GetAmount() const;

    std::string Registration(std::string_view sUsername, std::string_view sPassword);
    std::string Authorization(std::string_view sUsername, std::string_view sPassword);
    std::string RemoveUser(std::string_view sUsername, std::string_view sPassword);

    std::string ShowBalance();
    std::string ShowUserActiveOffers();
    std::string ShowUserCompletedOffers();
    std::string ShowAllSaleOffers();
    std::string ShowAllPurchaseOffers();

    std::string ReplenishUSD(double dAmount);
    std::string ReplenishRUB(double dAmount);
    std::string RemoveUSD(double dAmount);
    std::string RemoveRUB(double dAmount);

    std::string SaleUSD(double dAmount, double dPrice);
    std::string PurchaseUSD(double dAmount, double dPrice);
    std::string RemoveOffer(const std::string& sOfferId);

    std::vector<std::string> GetFields(const nlohmann::json& reply, const char* pParam, size_t uNumOffers);

    std::string _sUserId;
    tcp::socket* const _pSocket;
};
