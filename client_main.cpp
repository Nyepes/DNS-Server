#include "Client.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./client <domain name>";
        exit(2);
    }
    std::string s = argv[1];
    s = Client::queryDNS(s);
    for (const auto& ch: s) {
        std::cout << std::to_string((uint8_t)(ch)) << '.';
    }
    printf("%c\n", 8);
}