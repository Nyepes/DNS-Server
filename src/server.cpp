#include "Resolver.hpp"
#include "ServerClientShared.hpp"

#include <cstdlib>
#include <string.h>
#include <iostream>

Resolver::Resolver() {
    return;
}
void Resolver::startResolver() {
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0) {
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
        handleClient();
    }
}
void Resolver::handleClient() {
    Packet packet = getClientRespones();
    ssize_t size = getClientPacket(packet);
    char packet_data[512];
    Parser::toBuffer(packet, packet_data);
    sendto(server_socket, packet_data, size, 0, (struct sockaddr*)&address_info, sizeof(address_info));
    /* TODO: Check for errors and what not */
}
/* Recursive Resolve */
ssize_t Resolver::getClientPacket(Packet& p) {
    std::string domain_name = p.questions.at(0).name;
    std::string name_server = "192.5.6.30";
    char data[PACKET_SIZE];
    char answers[PACKET_SIZE];
    int bufferSize = Parser::toBuffer(p, data);
    Record answer; 
    ((uint16_t*)data)[1]= htons(((uint16_t*)data)[1]);
    while (1) {
        std::cout << "Looking" << std::endl;
        struct sockaddr_in addr_info = getAddrInfoClient(53, name_server);
        socklen_t addr_info_len = sizeof(addr_info);
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        ssize_t sent = sendto(sock, data, bufferSize, 0, (struct sockaddr*)&addr_info, addr_info_len);
        ssize_t bytes = recvfrom(sock, answers, PACKET_SIZE, 0, (struct sockaddr*)&addr_info, &addr_info_len);
        Packet answerPacket = Parser::fromBuffer(answers);

        if (answerPacket.header.answer_count >= 1 && answerPacket.answers.at(0).type == QueryType::A) {
            answer = answerPacket.answers.at(0);
            break;
        } 
        if (answerPacket.header.answer_count >= 1 && answerPacket.answers.at(0).type == QueryType::CNAME) {
            p.questions.at(0).name = answerPacket.answers.at(0).data;
            bufferSize = Parser::toBuffer(p, data);
            continue;
        }
        const auto& additional_records = answerPacket.additional_records;
        // int rand_idx = 0 % additional_records.size();
        
        int idx = 0;
        for (int i = 0; i < additional_records.size(); ++i) {
            if (additional_records.at(i).type == QueryType::A) {
                idx = i;
            }
        }
        std::cout << "Hello" << std::endl;
        name_server = additional_records.at(idx).data;
        // name_server = data;
        std::cout << name_server << std::endl;
    }
    std::cout << "done" << std::endl;
    std::cout <<answer.data << std::endl;
    p.answers.push_back(answer);
    p.header.answer_count += 1;
    return PACKET_SIZE;
}

Packet Resolver::getClientRespones() {
    socklen_t address_length = sizeof(sockaddr);
    char packet_data[PACKET_SIZE];
    size_t size = recvfrom(server_socket, packet_data, PACKET_SIZE, 0, (struct sockaddr*)&address_info, &address_length);
    Packet response = Parser::fromBuffer(packet_data);
    return response;

}