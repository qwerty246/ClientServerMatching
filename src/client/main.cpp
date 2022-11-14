#include "Client.h"

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service ioService;

        tcp::resolver resolver(ioService);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator it = resolver.resolve(query);

        tcp::socket* pSocket = new tcp::socket(ioService);
        pSocket->connect(*it);

        Client client(pSocket);
        std::string sMyId;
        std::cout << "Hello!" << std::endl;
        while (true)
        {
            if (client.RegistrationProcess())
            {
                break;
            }
        }
        while (true)
        {
            client.MainProcess();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
