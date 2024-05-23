
#include "Parser.hpp"
#include <string.h>

class Client {
    static Packet queryDNS(std::string name);
    static std::vector<Record> getAnswerList(Packet);
};