#include "PacketReader.hpp"
#include <string>
namespace DNS {
    class PacketWriter {
        public:
            PacketWriter(Buffer& buf);
            void write_next_byte(char data);
            void write_next_4bytes(int32_t data);
            void write_next_2bytes(int16_t data);
            void change_position(int new_pos);
            void write_at(unsigned idx, char data);
            void skip(int amount);
            void write_range(int, int end, string data);
            void write_next(int amount, string data);
            void write_domain_name(string name);
        private:
            Buffer& buffer;
            void check_range(int position);
    };
}
