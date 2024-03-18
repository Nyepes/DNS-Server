#include "../includes/PacketWriter.hpp"
#include <stdexcept>
#include <sstream>
#include <string>
#include <numeric>
#include <cstring>
#include <iostream>

#define DNS_MAX_JUMPS 10

using namespace DNS;
using namespace std;

PacketWriter::PacketWriter(Buffer& buf): buffer(buf) {
}
void PacketWriter::check_range(int position) {
    if (position == DNS_PACKET_SIZE) {
        throw out_of_range("Finished Reading all Packet");
    }
}
void PacketWriter::write_next_byte(char byte) {
    check_range(buffer.pos);
    buffer.data[buffer.pos] = byte;
    buffer.pos++;
} 
void PacketWriter::change_position(int new_pos) {
    check_range(new_pos);
    buffer.pos = new_pos;
}
void PacketWriter::write_range(int begin, int end, string data) {
    check_range(begin);
    check_range(end);
    for (int i = begin; i < end; ++i) {
        write_at(i, data.at(i - begin));
    }
}
void PacketWriter::write_next(int amount, string data) { 
    write_range(buffer.pos, buffer.pos + amount, data);
    buffer.pos = buffer.pos + amount;
}
void PacketWriter::skip(int amount) {
    int new_position = amount + buffer.pos;
    check_range(new_position);
    buffer.pos = new_position;
}
void PacketWriter::write_at(unsigned idx, char data) {
    check_range(idx);
    buffer.data[idx] = data;
}
void PacketWriter::write_next_4bytes(int32_t data) {
    check_range(buffer.pos + 4);
    uint32_t ans = 0;
    buffer.pos += 3;
    for (int i = 0; i < 4; ++i) {
        buffer.data[buffer.pos - i] = data & 0xFF;
        data = data >> 8;
    }    
    buffer.pos += 1;
}
void PacketWriter::write_next_2bytes(int16_t data) {
    check_range(buffer.pos + 2);
    uint32_t ans = 0;
    buffer.pos += 1;
    for (int i = 0; i < 2; ++i) {
        buffer.data[buffer.pos - i] = data & 0xFF;
        data = data >> 8;
    }
    buffer.pos += 1;
}
void PacketWriter::write_domain_name(string name) {
    string cur;
    for (auto letter: name) {
        if (letter == '.') {
            write_next_byte(cur.size());
            write_next(cur.size(), cur);
            cur = "";
        } else {
            cur += letter;
        }
    }
    if (!cur.empty()) {

        write_next_byte(cur.size());
        write_next(cur.size(), cur); 
    }
    write_next_byte(0);
}