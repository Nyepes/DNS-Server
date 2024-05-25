#include "Client.hpp"
#include "ServerClientShared.hpp"
// #include "Parser.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>

#define DNS_PORT 2048
#define SERVER_IP "127.0.0.1"

std::string Client::queryDNS(std::string name) {
    int connection_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in address_info = getAddrInfoClient(DNS_PORT, SERVER_IP);

    Packet dns_packet;
    dns_packet.header.id = 10000;
    dns_packet.header.question_count = 1;
    dns_packet.header.recursion_desired = 1;
    Question query;
    query.name = name.c_str();
    query.question_class = 1;
    query.type = QueryType::A;
    dns_packet.questions.push_back(query);
    char buffer[PACKET_SIZE];
    Parser::toBuffer(dns_packet, buffer);
    socklen_t len = sizeof(address_info);
    std::cout << "Sending" << std::endl;
    sendto(connection_socket, buffer, PACKET_SIZE, 0, (struct sockaddr*)&address_info, len);
    std::cout << "Waiting" << std::endl;
    recvfrom(connection_socket, buffer, PACKET_SIZE, 0, (struct sockaddr*)&address_info, &len);

    Packet p = Parser::fromBuffer(buffer);
    return name;

}