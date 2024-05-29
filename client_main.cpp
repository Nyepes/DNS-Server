#include "Client.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::string s = "www.google.com";
    s = Client::queryDNS(s);
    for (const auto& ch: s) {
        std::cout << std::to_string((uint8_t)(ch)) << '.';
    }
    printf("%c\n", 8);
}