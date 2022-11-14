#include "Client.h"

using boost::asio::ip::tcp;

Client::Client(tcp::socket* pSocket) :
    _pSocket(pSocket)
{
}

Client::~Client()
{
    if (_pSocket)
    {
        delete _pSocket;
    }
}

void Client::SendMessage(const nlohmann::json& req)
{
    std::string sRequest = req.dump();
    boost::asio::write(*_pSocket, boost::asio::buffer(sRequest, std::size(sRequest)));
}

std::string Client::ReadMessage()
{
    boost::asio::streambuf b;
    boost::asio::read_until(*_pSocket, b, "\0");
    std::istream is(&b);
    return std::string(std::istreambuf_iterator<char>(is), {});
}

bool Client::RegistrationProcess()
{
    std::cout << std::endl <<
                 "#####################\n"
                 "# Menu:             #\n"
                 "# (0) Exit          #\n"
                 "# (1) Registration  #\n"
                 "# (2) Authorization #\n"
                 "# (3) Remove user   #\n"
                 "#####################"
                 << std::endl;

    std::string sName;
    std::string sPassword;
    std::string sReply;
    switch (GetMenuOptionNum())
    {
        case 0:
        {
            exit(0);
        }
        case 1:
        {
            std::cout << "Enter your name: ";
            std::cin >> sName;
            std::cout << "Enter your password: ";
            std::cin >> sPassword;
            sReply = Registration(sName, sPassword);
            if (sReply == Registration::UserExist)
            {
                std::cout << "User with the same name already exists." << std::endl;
                return false;
            }
            return true;
        }
        case 2:
        {
            std::cout << "Enter your name: ";
            std::cin >> sName;
            std::cout << "Enter your password: ";
            std::cin >> sPassword;
            sReply = Authorization(sName, sPassword);
            if (sReply == Registration::UserNotExist)
            {
                std::cout << "User with this name does not exist." << std::endl;
                return false;
            }
            else if (sReply == Registration::InvalidPassword)
            {
                std::cout << "Invalid password." << std::endl;
                return false;
            }
            return true;
        }
        case 3:
        {
            std::cout << "Enter your name: ";
            std::cin >> sName;
            std::cout << "Enter your password: ";
            std::cin >> sPassword;
            sReply = RemoveUser(sName, sPassword);
            if (sReply == Registration::UserNotExist)
            {
                std::cout << "User with this name does not exist." << std::endl;
                return false;
            }
            else if (sReply == Registration::InvalidPassword)
            {
                std::cout << "Invalid password." << std::endl;
                return false;
            }
            else if (sReply == Registration::SuccessfullyRemoved)
            {
                std::cout << "User successfully removed." << std::endl;
                return false;
            }
            break;
        }
        default:
        {
            sReply = Registration::UnknownCommand;
            std::cout << "Unknown menu option" << std::endl;
            return false;
        }
    }
    return true;
}

void Client::MainProcess()
{
    std::cout << std::endl <<
                 "#################################\n"
                 "# Menu:                         #\n"
                 "# (0) Exit                      #\n"
                 "#################################\n"
                 "# (10) Show my balance          #\n"
                 "# (11) Show my active offers    #\n"
                 "# (12) Show my completed offers #\n"
                 "# (13) Show all sale offers     #\n"
                 "# (14) Show all purchase offers #\n"
                 "#################################\n"
                 "# (20) Replenish balance, USD   #\n"
                 "# (21) Replenish balance, RUB   #\n"
                 "# (22) Remove from balance, USD #\n"
                 "# (23) Remove from balance, RUB #\n"
                 "#################################\n"
                 "# (30) Sale USD                 #\n"
                 "# (31) Purchase USD             #\n"
                 "# (32) Remove offer             #\n"
                 "#################################"
                 << std::endl;

    nlohmann::json req;
    req["UserId"] = _sUserId;
    nlohmann::json reply;

    switch (GetMenuOptionNum())
    {
        case 0:
        {
            exit(0);
            break;
        }
        case 10:
        {
            reply = nlohmann::json::parse(ShowBalance());
            std::cout << "You have:\n" + std::string(reply["AmountUSD"]) + " USD\n" +
                         std::string(reply["AmountRUB"]) + " RUB" << std::endl;
            break;
        }
        case 11:
        {
            reply = nlohmann::json::parse(ShowUserActiveOffers());
            size_t uSaleNum = std::stoi(std::string(reply["SaleNum"]));
            if (!uSaleNum)
            {
                std::cout << "You have not active sales." << std::endl;
            }
            else
            {
                std::vector<std::string> vSaleOfferId = GetFields(reply, "SaleOfferId", uSaleNum);
                std::vector<std::string> vSaleAmount = GetFields(reply, "SaleAmount", uSaleNum);
                std::vector<std::string> vSalePrice = GetFields(reply, "SalePrice", uSaleNum);
                std::cout << "Sales:\n"
                             "OfferId\t\tAmount\t\tPrice" << std::endl;
                for (size_t i = 0; i < uSaleNum; i++)
                {
                    std::cout << vSaleOfferId[i] + "\t\t" +
                                 vSaleAmount[i] + "\t" +
                                 vSalePrice[i] << std::endl;
                }
            }
            size_t uPurchaseNum = std::stoi(std::string(reply["PurchaseNum"]));
            if (!uPurchaseNum)
            {
                std::cout << "You have not active purchases." << std::endl;
            }
            else
            {
                std::vector<std::string> vPurchaseOfferId = GetFields(reply, "PurchaseOfferId", uPurchaseNum);
                std::vector<std::string> vPurchaseAmount = GetFields(reply, "PurchaseAmount", uPurchaseNum);
                std::vector<std::string> vPurchasePrice = GetFields(reply, "PurchasePrice", uPurchaseNum);
                std::cout << "Purchases:\n"
                             "OfferId\t\tAmount\t\tPrice" << std::endl;
                for (size_t i = 0; i < uPurchaseNum; i++)
                {
                    std::cout << vPurchaseOfferId[i] + "\t\t" +
                                 vPurchaseAmount[i] + "\t" +
                                 vPurchasePrice[i] << std::endl;
                }
            }
            break;
        }
        case 12:
        {
            reply = nlohmann::json::parse(ShowUserCompletedOffers());
            size_t uOffersNum = std::stoi(std::string(reply["OffersNum"]));
            if (!uOffersNum)
            {
                std::cout << "You have not completed offers." << std::endl;
            }
            else
            {
                std::vector<std::string> vOfferId = GetFields(reply, "OfferId", uOffersNum);
                std::vector<std::string> vOriginalAmount = GetFields(reply, "OriginalAmount", uOffersNum);
                std::vector<std::string> vPrice = GetFields(reply, "Price", uOffersNum);
                std::vector<std::string> vType = GetFields(reply, "Type", uOffersNum);
                std::cout << "Completed offers:\n"
                             "OfferId\t\tOriginalAmount\t\tPrice\t\tType" << std::endl;
                for (size_t i = 0; i < uOffersNum; i++)
                {
                    std::cout << vOfferId[i] + "\t\t" +
                                 vOriginalAmount[i] + "\t\t" +
                                 vPrice[i] + "\t" +
                                 vType[i] << std::endl;
                }
            }
            break;
        }
        case 13:
        {
            reply = nlohmann::json::parse(ShowAllSaleOffers());
            std::string str = std::string(reply["UsersNum"]);
            size_t uUsersNum = std::stoi(std::string(reply["UsersNum"]));
            if (!uUsersNum)
            {
                std::cout << "No active offers from another users." << std::endl;
            }
            else
            {
                std::vector<std::string> vUsername = GetFields(reply, "Username", uUsersNum);
                std::vector<std::string> vAmount = GetFields(reply, "Amount", uUsersNum);
                std::vector<std::string> vPrice = GetFields(reply, "Price", uUsersNum);
                std::cout << "Current sale offers:\n"
                             "Username\tAmount\t\tPrice" << std::endl;
                for (size_t i = 0; i < uUsersNum; i++)
                {
                    std::cout << vUsername[i] + "\t\t" +
                                 vAmount[i] + "\t" +
                                 vPrice[i] << std::endl;
                }
                std::cout << "Max price: " + std::string(reply["Max"])  << std::endl;;
                std::cout << "Min price: " + std::string(reply["Min"])  << std::endl;;
                std::cout << "Mean price: " + std::string(reply["Mean"])  << std::endl;;
            }
            break;
        }
        case 14:
        {
            reply = nlohmann::json::parse(ShowAllPurchaseOffers());
            std::string str = std::string(reply["UsersNum"]);
            size_t uUsersNum = std::stoi(std::string(reply["UsersNum"]));
            if (!uUsersNum)
            {
                std::cout << "No active offers from another users." << std::endl;
            }
            else
            {
                std::vector<std::string> vUsername = GetFields(reply, "Username", uUsersNum);
                std::vector<std::string> vAmount = GetFields(reply, "Amount", uUsersNum);
                std::vector<std::string> vPrice = GetFields(reply, "Price", uUsersNum);
                std::cout << "Current purchase offers:\n"
                             "Username\tAmount\t\tPrice" << std::endl;
                for (size_t i = 0; i < uUsersNum; i++)
                {
                    std::cout << vUsername[i] + "\t\t" +
                                 vAmount[i] + "\t" +
                                 vPrice[i] << std::endl;
                }
                std::cout << "Max price: " + std::string(reply["Max"]) << std::endl;
                std::cout << "Min price: " + std::string(reply["Min"]) << std::endl;
                std::cout << "Mean price: " + std::string(reply["Mean"]) << std::endl;
            }
            break;
        }
        case 20:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(ReplenishUSD(dAmount));
            std::cout << "You replenished the balance by " + std::string(reply["Amount"]) + " USD." << std::endl;
            break;
        }
        case 21:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(ReplenishRUB(dAmount));
            std::cout << "You replenished the balance by " + std::string(reply["Amount"]) + " RUB." << std::endl;
            break;
        }
        case 22:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(RemoveUSD(dAmount));
            std::cout << "You removed the balance by " + std::string(reply["Amount"]) + " USD." << std::endl;
            break;
        }
        case 23:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(RemoveRUB(dAmount));
            std::cout << "You removed the balance by " + std::string(reply["Amount"]) + " RUB." << std::endl;
            break;
        }
        case 30:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            std::cout << "At what price?" << std::endl;
            double dPrice = GetAmount();
            if (dPrice == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(SaleUSD(dAmount, dPrice));

            size_t uNumOffers = std::stoi(std::string(reply["NumOffers"]));
            std::vector<std::string> vAmounts = GetFields(reply, "Amount", uNumOffers);
            std::vector<std::string> vPrices = GetFields(reply, "Price", uNumOffers);
            for (size_t i = 0; i < uNumOffers; i++)
            {
                std::cout << "You sold " + vAmounts[i] +
                             " USD at a price of " + vPrices[i]
                             << std::endl;
            }

            if(reply["Completed"] == "1")
            {
                std::cout << "Offer completed." << std::endl;
            }
            else
            {
                std::cout << "Offer accepted. You put up " + std::string(reply["FinalAmount"]) +
                             " USD for sale at a price of " + std::string(reply["FinalPrice"]) << std::endl;
            }
            break;
        }
        case 31:
        {
            std::cout << "How much?" << std::endl;
            double dAmount = GetAmount();
            if (dAmount == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            std::cout << "At what price?" << std::endl;
            double dPrice = GetAmount();
            if (dPrice == -1)
            {
                std::cout << "Incorrect value, operation stoped." << std::endl;
                break;
            }
            reply = nlohmann::json::parse(PurchaseUSD(dAmount, dPrice));

            size_t uNumOffers = std::stoi(std::string(reply["NumOffers"]));
            std::vector<std::string> vAmounts = GetFields(reply, "Amount", uNumOffers);
            std::vector<std::string> vPrices = GetFields(reply, "Price", uNumOffers);
            for (size_t i = 0; i < uNumOffers; i++)
            {
                std::cout << "You bought " + vAmounts[i] +
                             " USD at a price of " + vPrices[i]
                             << std::endl;
            }

            if(reply["Completed"] == "1")
            {
                std::cout << "Offer completed." << std::endl;
            }
            else
            {
                std::cout << "Offer accepted. You put up " + std::string(reply["FinalAmount"]) +
                             " USD for purchase at a price of " + std::string(reply["FinalPrice"]) << std::endl;
            }
            break;
        }
        case 32:
        {
            std::cout << "What is the offer id?" << std::endl;
            std::string sOfferId;
            std::cin >> sOfferId;
            reply = nlohmann::json::parse(RemoveOffer(sOfferId));
            if (reply["OfferId"] == "-1")
            {
                std::cout << "Incorrect offer id." << std::endl;
            }
            else if (reply["OfferId"] == "0")
            {
                std::cout << "Offer with that id does not exist." << std::endl;
            }
            else
            {
                std::cout << "Offer removed." << std::endl;
            }
            break;
        }
        default:
        {
            std::cout << "Unknown menu option" << std::endl;
            break;
        }
    }
}

short Client::GetMenuOptionNum() const
{
    std::string sMenuOptionNum;
    std::cin >> sMenuOptionNum;
    try
    {
        return std::stoi(sMenuOptionNum);
    }
    catch (...)
    {
        return -1;
    }
}

double Client::GetAmount() const
{
    std::string sAmount;
    std::cin >> sAmount;
    try
    {
        return std::stod(sAmount);
    }
    catch (...)
    {
        return -1;
    }
}

std::string Client::Registration(std::string_view sUsername, std::string_view sPassword)
{
    nlohmann::json req;
    req["ReqType"] = Requests::Registration;
    req["Username"] = sUsername;
    req["Password"] = sPassword;
    SendMessage(req);
    nlohmann::json reply = nlohmann::json::parse(ReadMessage());
    if (reply["Message"] != Registration::UserExist)
    {
        _sUserId = reply["Message"];
    }
    return reply["Message"];
}

std::string Client::Authorization(std::string_view sUsername, std::string_view sPassword)
{
    nlohmann::json req;
    req["ReqType"] = Requests::Authorization;
    req["Username"] = sUsername;
    req["Password"] = sPassword;
    SendMessage(req);
    nlohmann::json reply = nlohmann::json::parse(ReadMessage());
    if (reply["Message"] != Registration::UserNotExist &&
        reply["Message"] != Registration::InvalidPassword)
    {
        _sUserId = reply["Message"];
    }
    return reply["Message"];
}

std::string Client::RemoveUser(std::string_view sUsername, std::string_view sPassword)
{
    nlohmann::json req;
    req["ReqType"] = Requests::RemoveUser;
    req["Username"] = sUsername;
    req["Password"] = sPassword;
    SendMessage(req);
    nlohmann::json reply = nlohmann::json::parse(ReadMessage());
    if (reply["Message"] != Registration::SuccessfullyRemoved)
    {
        _sUserId.clear();
    }
    return reply["Message"];
}

std::string Client::ShowBalance()
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ShowBalance;
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ShowUserActiveOffers()
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ShowUserActiveOffers;
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ShowUserCompletedOffers()
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ShowUserCompletedOffers;
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ShowAllSaleOffers()
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ShowAllSaleOffers;
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ShowAllPurchaseOffers()
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ShowAllPurchaseOffers;
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ReplenishUSD(double dAmount)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ReplenishUSD;
    req["Amount"] = std::to_string(dAmount);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::ReplenishRUB(double dAmount)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::ReplenishRUB;
    req["Amount"] = std::to_string(dAmount);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::RemoveUSD(double dAmount)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::RemoveUSD;
    req["Amount"] = std::to_string(dAmount);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::RemoveRUB(double dAmount)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::RemoveRUB;
    req["Amount"] = std::to_string(dAmount);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::SaleUSD(double dAmount, double dPrice)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::SaleUSD;
    req["Amount"] = std::to_string(dAmount);
    req["Price"] = std::to_string(dPrice);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::PurchaseUSD(double dAmount, double dPrice)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::PurchaseUSD;
    req["Amount"] = std::to_string(dAmount);
    req["Price"] = std::to_string(dPrice);
    SendMessage(req);
    return ReadMessage();
}

std::string Client::RemoveOffer(const std::string& sOfferId)
{
    nlohmann::json req;
    req["UserId"] = _sUserId;
    req["ReqType"] = Requests::RemoveOffer;
    req["OfferId"] = sOfferId;
    SendMessage(req);
    return ReadMessage();
}

std::vector<std::string> Client::GetFields(const nlohmann::json& reply, const char* pParam, size_t uNumOffers)
{
    std::vector<std::string> vResult;
    if (uNumOffers)
    {
        std::string sParam = reply[pParam];
        for (size_t i = 0; i < uNumOffers; i++)
        {
            size_t pos = sParam.find("/");
            vResult.push_back(sParam.substr(0, pos ));
            sParam = sParam.substr(pos + 1);
        }
    }
    return vResult;
}
