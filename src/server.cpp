#include "Resolver.hpp"
#include "ServerClientShared.hpp"

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <future>
#include <unistd.h>
#include <functional>

#define ROOT_SERVER "192.5.6.30"
#define NUM_THREADS 5

Resolver::Resolver() {
    // TODO: Start Caching System
    return;
}
void Resolver::startResolver() {
    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0) {
        std::cout << 1 << std::endl;
        perror(NULL);
        exit(1);
    }
    address_info = getAddrInfoServer(DNS_PORT);
    int res = bind(server_socket, (sockaddr*)&address_info, sizeof(address_info));
    if (res < 0) {
        std::cout << 1 << std::endl;
        perror(NULL);
        exit(1);
    }
    std::vector<std::thread> threads(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; ++i) {
        std::cout << i << std::endl;
        threads.at(i) = std::thread(&Resolver::acceptMessage, this);
    }
    // std::thread(&Resolver::acceptMessage, this);
    acceptMessage();
    closeResolver();

}
void Resolver::closeResolver() {

}
void Resolver::acceptMessage() {
    char packet_data[PACKET_SIZE];
    while (1) {
        handleClient();
    }
}
void Resolver::handleClient() {
    Packet packet = getClientRespones();
    std::future<ssize_t> sizeFuture = std::async(std::launch::async, &Resolver::getClientPacket, this, std::ref(packet));
    ssize_t size = sizeFuture.get();
    char packet_data[512];
    Parser::toBuffer(packet, packet_data);
    sendto(server_socket, packet_data, size, 0, (struct sockaddr*)&address_info, sizeof(address_info));
}
ssize_t Resolver::getClientPacket(Packet& p) {
    std::string domain_name = p.questions.at(0).name;
    std::string name_server = ROOT_SERVER;
    char data[PACKET_SIZE];
    char answers[PACKET_SIZE]; 
    int bufferSize = Parser::toBuffer(p, data);
    Record answer; 
    while (1) {
        std::cout << name_server << std::endl;
        Packet answerPacket = queryNameServer(name_server, data, bufferSize);
        if (answerPacket.header.answer_count >= 1 && answerPacket.answers.at(0).type == QueryType::A) {
            answer = answerPacket.answers.at(0);
            break;
        } 
        if (answerPacket.header.answer_count >= 1 && answerPacket.answers.at(0).type == QueryType::CNAME) {
            p.questions.at(0).name = answerPacket.answers.at(0).data;
            return getClientPacket(p);
        }
        name_server = getNextNameServer(answerPacket);
    }
    p.answers.push_back(answer);
    p.header.answer_count += 1;
    return PACKET_SIZE;
}
std::string Resolver::getNextNameServer(const Packet& answerPacket) {
    std::cout << "next" << std::endl;

    if (answerPacket.additional_records.size() >= 1) {
        const auto& additional_records = answerPacket.additional_records;        
        int idx = 0;
        for (int i = 0; i < additional_records.size(); ++i) {
            if (additional_records.at(i).type == QueryType::A) {
                idx = i;
            }
        }
        return additional_records.at(idx).data;
    } else {
        auto nameServerFuture = std::async(std::launch::async, &Resolver::findServerIpAddress, this, std::ref(answerPacket));
        return nameServerFuture.get();
    }
}
Packet Resolver::getClientRespones() {
    std::cout << "cr" << std::endl;
    socklen_t address_length = sizeof(sockaddr);
    char packet_data[PACKET_SIZE];
    size_t size = recvfrom(server_socket, packet_data, PACKET_SIZE, 0, (struct sockaddr*)&address_info, &address_length);
    Packet response = Parser::fromBuffer(packet_data);
    std::cout << "recv" << std::endl;

    return response;

}
Packet Resolver::queryNameServer(std::string name_server, char data[PACKET_SIZE], int packet_size) {
    struct sockaddr_in addr_info = getAddrInfoClient(53, name_server);
    socklen_t addr_info_len = sizeof(addr_info);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    std::cout << "H1" << std::endl;
    char answers[PACKET_SIZE];
    std::cout << "H2" << std::endl;
    ssize_t sent = sendto(sock, data, packet_size, 0, (struct sockaddr*)&addr_info, addr_info_len);
    std::cout << "H3" << std::endl;
    ssize_t bytes = recvfrom(sock, answers, PACKET_SIZE, 0, (struct sockaddr*)&addr_info, &addr_info_len);
    std::cout << "H5: " << bytes << std::endl;
    Packet answerPacket = Parser::fromBuffer(answers);
    std::cout << "H5" << std::endl;
    close(sock);
    return answerPacket;
}
std::string Resolver::findServerIpAddress(const Packet& answerPacket) {
    Packet queryAuthorativeIP;
    queryAuthorativeIP.header.id = 10001;
    queryAuthorativeIP.header.question_count = 1;
    Question query;
    query.name = answerPacket.authorities.at(0).data;
    query.question_class = 1;
    query.type = QueryType::A;
    queryAuthorativeIP.questions.push_back(query);
    getClientPacket(queryAuthorativeIP);
    return queryAuthorativeIP.answers.at(0).data;
}