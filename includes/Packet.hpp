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
    A = 1, // Host Address
    NS = 2, //Authoriative Name Server
    MD = 3, // OBSOLETE (ERROR) 3 a mail destination (Obsolete - use MX)
    MF = 4, // OBSOLETE (ERROR) a mail forwarder (Obsolete - use MX)
    CNAME = 5, // the canonical name for an alias
    SOA = 6, //marks the start of a zone of authority
    MB = 7, // (DO NOT IMPLEMENT) a mailbox domain name (EXPERIMENTAL)
    MG = 8, // (DO NOT IMPLEMENT) a mail group member (EXPERIMENTAL)
    MR = 9, // (DO NOT IMPLEMENT) a mail rename domain name (EXPERIMENTAL)
    NONE = 10, //(DO NOT IMPLEMENT) a null RR (EXPERIMENTAL)
    WKS = 11, // a well known service description
    PTR = 12, // a domain name pointer
    HINFO = 13, // host information
    MINFO = 14, // mailbox or mail list information
    MX = 15, // mail exchange
    TXT = 16, // text strings
    AAAA = 28,
    // Only For Type in Record Class
    AXFR = 252, // A request for a transfer of an entire zone
    MAILB = 253, // A request for mailbox-related records (MB, MG or MR)
    MAILA = 254, // A request for mail agent RRs (Obsolete - see MX)
    ALL = 255 // A request for all records
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