
#include "Parser.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>

/*Start with Slow IO*/
#define DNS_PORT 2048



class Resolver {
    public:
    Resolver();
    void startResolver();
    void closeResolver();

    protected:
    void acceptMessage();
    void handleClient();
    ssize_t getClientPacket(Packet& p);
    Packet getClientRespones();

    struct sockaddr_in address_info;
    int server_socket;

    private:
    Packet queryNameServer(std::string name_server, char data[PACKET_SIZE], int packet_size);
    std::string findServerIpAddress(const Packet& answerPacket);
    std::string getNextNameServer(const Packet& answerPacket);
};