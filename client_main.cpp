#include "Client.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::string s = "google.com";
    // Client::queryDNS(s);
    std::cout << Client::queryDNS(s) << std::endl;
}