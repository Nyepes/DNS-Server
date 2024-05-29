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
    query.name = name;
    query.question_class = 1;
    query.type = QueryType::A;
    dns_packet.questions.push_back(query);
    char buffer[PACKET_SIZE];
    Parser::toBuffer(dns_packet, buffer);
    socklen_t len = sizeof(address_info);
    // std::cout << "Sending" << std::endl;
    sendto(connection_socket, buffer, PACKET_SIZE, 0, (struct sockaddr*)&address_info, len);
    // std::cout << "Waiting" << std::endl;
    ssize_t size = recvfrom(connection_socket, buffer, PACKET_SIZE, 0, (struct sockaddr*)&address_info, &len);
    // std::cout << "Size: " << size << std::endl;
    Packet p = Parser::fromBuffer(buffer);
    // std::cout << "ID: " << p.header.id << std::endl;
    // std::cout << "Z: " << p.header.reserved << std::endl; 
    // std::cout << "RA: " << p.header.recursion_available << std::endl;
    // std::cout << "RD: " << p.header.recursion_desired << std::endl;
    // std::cout << "OPCODE: " << p.header.opcode << std::endl;
    // std::cout << "TC: " << p.header.truncated_message << std::endl;
    // std::cout << "QR: " << p.header.query_response << std::endl;
    // std::cout <<  "Return Code " << p.header.result_code << std::endl;
    // std::cout <<  "Question Count " << p.header.question_count << std::endl;
    // std::cout <<  "AnswerCount " << p.header.answer_count << std::endl;
    // std::cout <<  "authorities Count " << p.header.authority_count << std::endl;
    // std::cout <<  "Additional Count " << p.header.additional_records_count << std::endl;

    return p.answers.at(0).data;

}