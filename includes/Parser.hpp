#include <Packet.hpp>

class Parser {
    public:
    static int toBuffer(const Packet& packet, char out[PACKET_SIZE]);
    static Packet fromBuffer(char buffer[PACKET_SIZE]);
};