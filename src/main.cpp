#include "DnsPacket.hpp"
#include <iostream>
#include <iostream>

using namespace DNS;
int main() {
    // Buffer* buffer = new Buffer;
    // buffer->data[0] = 0x06;
    // buffer->data[1] = 0x67;
    // buffer->data[2] = 0x6f;
    // buffer->data[3] = 0x6f;
    // buffer->data[4] = 0x67;
    // buffer->data[5] = 0x6c;
    // buffer->data[6] = 0x65;

    // buffer->data[7] = 0x03;
    // buffer->data[8] = 0x63;
    // buffer->data[9] = 0x6f;
    // buffer->data[10] = 0x6d;
    // buffer->data[11] = 0x0;

    // buffer->data[12] = 0xc0;
    // buffer->data[13] = 0x00;
    // buffer->data[14] = 0x01;
    // buffer->data[15] = 'a';
    // buffer->data[16] = 0x0;
    // buffer -> pos = 12;
    // PacketReader reader = PacketReader(*buffer);
    // string domain = reader.read_domain_name();
    // char next = reader.read_next_byte();
    // std::cout << domain << std::endl;
    // std::cout << (unsigned)next << std::endl;
    // free(buffer);

    FILE* response = fopen("response_packet.txt", "r");
    Buffer buffer;
    char* buf = &(buffer.data[0]);
    size_t size = 512;
    size_t read = fread(buf, 1, size, response);
    PacketReader reader(buffer);
    Packet packet(reader);
    std::cout << packet << std::endl;

}