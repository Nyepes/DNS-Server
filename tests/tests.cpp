#include <gtest/gtest.h>
#include <iostream>
#include "../includes/DnsPacket.hpp"
#include "../includes/PacketReader.hpp"
using namespace DNS;

// Demonstrate some basic assertions.
namespace DNS {
    bool operator==(const Buffer& a, const Buffer& b) {
        for (int i = 0; i < DNS_PACKET_SIZE; ++i) {
            if (a.data[i] != b.data[i])  {
                return false;  
            }
        }
        return true;
    }
}

TEST(Parsing, ReadWrite) {
    FILE* response = fopen("../tests/response_packet.txt", "r");
    Buffer buffer;
    char* buf = &(buffer.data[0]);
    size_t size = 512;
    size_t read = fread(buf, 1, size, response);
    fclose(response);

    PacketReader reader(buffer);
    Packet packet(reader);
    Buffer out;
    out = packet.to_buffer(out);

    FILE* response1 = fopen("../tests/response_packet.txt", "r");
    Buffer buffer1;
    char* buf1 = &(buffer1.data[0]);
    size_t size1 = 512;
    size_t read1 = fread(buf1, 1, size1, response1);
    fclose(response1);

    PacketReader r2(buffer1);
    Packet p2(r2);
    buffer1.pos = 0;
    p2.to_buffer(buffer1);

    buffer1.pos = 0;
    PacketReader r3(buffer1);
    Packet p3 (r3);
    p3.to_buffer(buffer1);
    buffer1.pos = 0;
    EXPECT_EQ(out, buffer1);

}