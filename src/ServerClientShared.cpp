#include "ServerClientShared.hpp"

struct sockaddr_in getAddrInfoServer(int port) {
    struct sockaddr_in address_info;
    memset(&address_info, 0, sizeof(address_info));
    address_info.sin_port = htons(port);
    address_info.sin_family = AF_INET;
    address_info.sin_addr.s_addr = htonl(INADDR_ANY);
    return address_info;
}

struct sockaddr_in getAddrInfoClient(int port, std::string ip_address) {
    struct sockaddr_in address_info;
    memset(&address_info, 0, sizeof(address_info));
    address_info.sin_port = htons(port);
    address_info.sin_family = AF_INET;
    inet_aton(ip_address.c_str(), &address_info.sin_addr);
    return address_info;
}

