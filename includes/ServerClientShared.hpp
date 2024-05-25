#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>


#define PORT 2048


struct sockaddr_in getAddrInfoServer(int port);

struct sockaddr_in getAddrInfoClient(int port, std::string ip_address);
