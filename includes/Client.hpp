
#include "Parser.hpp"
#include <string.h>
#include <vector>

class Client {
    public:
    /* Returns the IP Address of the query given in name*/
    static std::string queryDNS(std::string name);
};