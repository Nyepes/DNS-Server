#ifndef PACKET_READER
#define PACKET_READER

#include <cstdint>
#include <string>

#define DNS_PACKET_SIZE 512

using namespace std;

namespace DNS {
    struct Buffer {
        char data[DNS_PACKET_SIZE];
        int pos;
    };
    class PacketReader {
        public:
            PacketReader(Buffer& buf);
            char read_next_byte();
            char read_current_byte();
            void change_position(int new_pos);
            char read_at(unsigned idx);
            void skip(int amount);
            string read_range(int begin, int end);
            string read_domain_name();
        private:
            Buffer& buffer;
            void check_range(int position);
    };
}
#endif