#include "PacketReader.hpp"
#include <stdexcept>
#include <sstream>
#include <numeric>
#include <cstring>
#include <iostream>
#define DNS_MAX_JUMPS 10
using namespace DNS;
using namespace std;

PacketReader::PacketReader(Buffer& buf): buffer(buf) {
}
void PacketReader::check_range(int position) {
    if (position == DNS_PACKET_SIZE) {
        throw out_of_range("Finished Reading all Packet");
    }
}
char PacketReader::read_next_byte() {
    check_range(buffer.pos);
    char ans = buffer.data[buffer.pos];
    buffer.pos++;
    return ans;
}  
char PacketReader::read_current_byte() {
    check_range(buffer.pos);
    return buffer.data[buffer.pos];
}
void PacketReader::change_position(int new_pos) {
    check_range(new_pos);
    buffer.pos = new_pos;
}
string PacketReader::read_range(int begin, int end) {
    check_range(begin);
    check_range(end);
    string result;
    for (int i = begin; i < end; ++i) {
        result += read_at(i);
    }
    return result;
}
string PacketReader::read_next(int amount) { 
    return read_range(buffer.pos, buffer.pos + amount);
}
void PacketReader::skip(int amount) {
    int new_position = amount + buffer.pos;
    check_range(new_position);
    buffer.pos = new_position;
}
char PacketReader::read_at(unsigned idx) {
    check_range(idx);
    return buffer.data[idx];
}
uint32_t PacketReader::read_next_4bytes() {
    check_range(buffer.pos + 4);
    uint32_t ans = 0;
    for (int i = 0; i < 4; ++i) {
        ans = ans << 8;
        ans += buffer.data[buffer.pos] & 0xFF;
        buffer.pos = buffer.pos + 1;

    }
    return ans;
    
}
uint16_t PacketReader::read_next_2bytes() {
    check_range(buffer.pos + 2);
    uint16_t ans = 0;
    for (int i = 0; i < 2; ++i) {
        ans = ans << 8;
        ans += buffer.data[buffer.pos] & 0xFF;
        buffer.pos = buffer.pos + 1;
    }
    return ans;
}
string PacketReader::read_domain_name() {
    int position = buffer.pos;
    string name;
    bool jumped = true;
    int num_jumps = 0;
    while (1) {
        uint8_t length = read_at(position);
        position++;
        if (length == 0) break;
        if ((length & 0xC0) == 0xC0) { // Upper two bits are set
            uint8_t next_byte = read_at(position);
            if (num_jumps >= DNS_MAX_JUMPS) throw invalid_argument("Too many jumps");
            position = ((length ^ 0xC0) << 8) + next_byte;
            jumped = true;
            num_jumps++;
        } else {
            jumped = false;
            name += read_range(position, position + length);
            name += ".";
            position += length;
        }
    }
    if ((buffer.data[buffer.pos] & 0xC0) == 0xC0) {
        buffer.pos += 2;
    } else {
        buffer.pos = position;
    }

    name.pop_back();
    return name;


    
}