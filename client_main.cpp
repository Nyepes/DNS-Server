#include "Client.hpp"
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./client <domain name>";
        exit(2);
    }
    std::string s = argv[1];
    s = Client::queryDNS(s);
    std::cout << s << std::endl;
    // sleep(20);
    // s = Client::queryDNS(s);


    printf("%c\n", 8);
}