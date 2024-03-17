#include <gtest/gtest.h>
#include <iostream>
// Demonstrate some basic assertions.

TEST(Parsing, "Correctly parses google.com data") {
    FILE* response = fopen("../tests/response_packet.txt", "r");
    Buffer buffer;
    char* buf = &(buffer.data[0]);
    size_t size = 512;
    size_t read = fread(buf, 1, size, response);
    PacketReader reader(buffer);
    Packet packet(reader);
    EXPECT_EQ(packet.header.id == )
    std::cout << packet << std::endl;
}