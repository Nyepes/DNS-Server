#include "Resolver.hpp"
#include "ServerClientShared.hpp"


#include <string.h>
#include <iostream>

Resolver::Resolver() {
    return;
}
void Resolver::startResolver() {
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0) {
        std::cout << "Creating Socket" << std::endl;
        perror(NULL);
        exit(1);
    }
    address_info = getAddrInfoServer(DNS_PORT);
    int res = bind(server_socket, (sockaddr*)&address_info, sizeof(address_info));

    if (res < 0) {
        perror(NULL);
        exit(1);
    }
    acceptMessage();
    closeResolver();

}
void Resolver::closeResolver() {

}
void Resolver::acceptMessage() {
    // int connection_fd = socket();
    char packet_data[PACKET_SIZE];
    while (1) {
        std::cout << "Waiting" << std::endl;
        handleClient();
    }
}
void Resolver::handleClient() {
    Packet packet = getClientRespones();
    std::cout << "Recieved" << std::endl;
    ssize_t size = getClientPacket(packet);
    char packet_data[512];
    Parser::toBuffer(packet, packet_data);
    sendto(server_socket, packet_data, size, 0, (struct sockaddr*)&address_info, sizeof(address_info));
    /* TODO: Check for errors and what not */
}
/* Recursive Resolve */
ssize_t Resolver::getClientPacket(Packet& p) {
    std::string domain_name = p.questions.at(0).name;
    struct sockaddr_in addr_info = getAddrInfoClient(53, "8.8.8.8");
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    char data[PACKET_SIZE];
    Parser::toBuffer(p, data);
    socklen_t addr_info_len = sizeof(addr_info);
    sendto(sock, data, PACKET_SIZE, 0, (struct sockaddr*)&addr_info, sizeof(addr_info));
    ssize_t bytes = recvfrom(sock, data, PACKET_SIZE, 0, (struct sockaddr*)&addr_info, &addr_info_len);
    p = Parser::fromBuffer(data);
    return bytes;
    // int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, socklen_t tolen);
}

Packet Resolver::getClientRespones() {
    socklen_t address_length = sizeof(sockaddr);
    char packet_data[PACKET_SIZE];
    size_t size = recvfrom(server_socket, packet_data, PACKET_SIZE, 0, (struct sockaddr*)&address_info, &address_length);
    Packet response = Parser::fromBuffer(packet_data);
    return response;

}