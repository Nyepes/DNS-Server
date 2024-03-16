#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "PacketReader.hpp"

using namespace std;

namespace DNS {
    enum ResultCode {
        NO_ERROR = 0,
        FORMAT_ERROR = 1,
        SERVER_FAILURE = 2,
        NAME_ERROR = 3,
        NOT_IMPLEMENTED = 4,
        REFUSED = 5

    };
    enum QueryType {
        UNKNOWN,
        ANSWER = 1
    };
    struct Header {
        uint16_t id;

        uint8_t opcode;
        bool query_response;
        bool authorative_answer;
        bool truncated_message;
        bool recursion_desired;
        bool recursion_available;
        bool reserved;
        ResultCode result_code;

        uint16_t question_count;
        uint16_t answer_count;
        uint16_t authority_count;
        uint16_t resource_count;
    };
    struct Question {
        string name;
        QueryType type;
        uint16_t question_class;
    };
    struct Record {
        string domain_name;
        int type;
        int record_class;
        int time_to_live;
        int len;
    };
    class Packet {
        public:
            Packet(PacketReader& reader);
        private: 
            Header header;
            vector<Question*> questions;
            vector<Record*> records;
    };
}