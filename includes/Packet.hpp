#include <vector>
#include <string>
#include <cstdint>

#define PACKET_SIZE 512

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
    uint16_t id = 0;
    int opcode = 0;
    bool query_response = 0;
    bool authorative_answer = 0;
    bool truncated_message = 0;
    bool recursion_desired = 0;
    bool recursion_available = 0;

    int reserved = 0;
    ResultCode result_code = NO_ERROR;

    uint16_t question_count = 0;
    uint16_t answer_count = 0;
    uint16_t authority_count = 0;
    uint16_t additional_records_count = 0;
};
struct Question {
    std::string name;
    QueryType type = UNKNOWN;
    uint16_t question_class = 0;
};
struct Record {
    std::string domain_name;
    int type = 0;
    int record_class = 0;
    int time_to_live = 0;
    int len = 0;
    std::string data;
};
struct Packet {
    Header header;
    std::vector<Question> questions;
    std::vector<Record> answers;
    std::vector<Record> authorities;
    std::vector<Record> additional_records;
};