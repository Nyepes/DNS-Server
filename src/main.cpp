#include "../includes/DnsPacket.hpp"
#include <iostream>

using namespace DNS;
int main() {
    FILE* response = fopen("../tests/response_packet.txt", "r");
    Buffer buffer;
    char* buf = &(buffer.data[0]);
    size_t size = 512;
    size_t read = fread(buf, 1, size, response);
    PacketReader reader(buffer);
}