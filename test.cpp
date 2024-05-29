#include "Parser.hpp"
#include <cassert>
#include <iostream>

/* PART 1: Parser Tests */

/*Creating Request Packet From buffer*/
int test1() {
    char buffer[512];
    FILE* file = fopen("test_files/test1.txt", "r");
    size_t size = fread(buffer, 1, 512, file);
    Packet packet = Parser::fromBuffer(buffer);
    assert(packet.header.id == 30598);
    assert(packet.header.query_response == 0);
    assert(packet.header.opcode == 0);
    assert(packet.header.authorative_answer == 0);
    assert(packet.header.truncated_message == 0);
    assert(packet.header.recursion_desired == 1);
    assert(packet.header.recursion_available == 0);
    assert(packet.header.reserved == 0);
    assert(packet.header.result_code == 0);
    assert(packet.header.question_count == 1);
    assert(packet.header.answer_count == 0);
    assert(packet.header.authority_count == 0);
    assert(packet.header.additional_records_count == 0);
    assert(packet.questions.size() == 1);
    assert(packet.answers.size() == 0);
    assert(packet.authorities.size() == 0);
    assert(packet.additional_records.size() == 0);
    assert(packet.questions.at(0).name == "google.com");
    assert(packet.questions.at(0).type == 1);
    assert(packet.questions.at(0).question_class == 1);
    return 0;
}
/*Creating Answer Packet from Buffer*/
int test2() {
    char buffer[512];
    FILE* file = fopen("test_files/test2.txt", "r");
    size_t size = fread(buffer, 1, 512, file);
    Packet packet = Parser::fromBuffer(buffer);
    assert(packet.header.id == 32128);
    assert(packet.header.query_response == 1);
    assert(packet.header.opcode == 0);
    assert(packet.header.authorative_answer == 0);
    assert(packet.header.truncated_message == 0);
    assert(packet.header.recursion_desired == 1);
    assert(packet.header.recursion_available == 1);
    assert(packet.header.reserved == 0);
    assert(packet.header.result_code == 0);
    assert(packet.header.question_count == 1);
    assert(packet.header.answer_count == 1);
    assert(packet.header.authority_count == 0);
    assert(packet.header.additional_records_count == 0);
    assert(packet.answers.size() == 1);
    assert(packet.questions.size() == 1);
    assert(packet.authorities.size() == 0);
    assert(packet.additional_records.size() == 0);
    assert(packet.questions.at(0).name == "google.com");
    assert(packet.questions.at(0).type == 1);
    assert(packet.questions.at(0).question_class == 1);
    assert(packet.answers.at(0).domain_name == "google.com");
    assert(packet.answers.at(0).len == 4);
    assert(packet.answers.at(0).record_class == 1);
    assert(packet.answers.at(0).type == 1);
    // std::cout << packet.answers.at(0).data << std::endl;
    // assert(packet.answers.at(0).data == "142.250.190.46");
    assert(packet.answers.at(0).data.at(0) == (char)142);
    assert(packet.answers.at(0).data.at(1) == (char)250);
    assert(packet.answers.at(0).data.at(2) == (char)190);
    assert(packet.answers.at(0).data.at(3) == (char)46);

    return 0;
}
/*Create Buffer from Packet */
int test3() {
    Packet packet;
    packet.header.id = 10000;
    packet.header.recursion_desired = 1;
    packet.header.question_count = 1;
    packet.questions.push_back(Question());
    packet.questions.at(0).name = "youtube.com";
    packet.questions.at(0).type = A;
    packet.questions.at(0).question_class = 1;
    packet.answers.push_back(Record());
    char ans[] = 
    {
        char(0x27), char(0x10), char(0x01), char(0x00), char(0x00), char(0x01), char(0x00), char(0x00), char(0x00), char(0x00), 
        char(0x00), char(0x00), char(0x07), char(0x79), char(0x6f), char(0x75), char(0x74), char(0x75), char(0x62), char(0x65), 
        char(0x03), char(0x63), char(0x6f), char(0x6d), char(0x00), char(0x00), char(0x01), char(0x00), char(0x01)
    };
    char buf[512];
    Parser::toBuffer(packet, buf);
    for (int i = 0; i < 29; ++i) {
        assert(ans[i] == buf[i]);
    }
    return 0;
}

int test4() {
    Packet packet;
    packet.header.id = 40254;
    packet.header.query_response = 1;
    packet.header.recursion_desired = 1;
    packet.header.recursion_available = 1;

    packet.header.question_count = 1;
    packet.header.answer_count = 6;

    packet.questions.push_back(Question());
    packet.questions.at(0).name = "yahoo.com";
    packet.questions.at(0).type = A;
    packet.questions.at(0).question_class = 1;

    Record r4;
    r4.domain_name = "yahoo.com";
    r4.data = {char(74), char(6), char(143), char(25)};
    r4.len = 4;
    r4.record_class = 1;
    r4.type = 1;
    r4.time_to_live = 668;
    // r1.

    Record r6;
    r6.domain_name = "yahoo.com";
    r6.data = {char(74), char(6), char(143), char(26)};
    r6.len = 4;
    r6.record_class = 1;
    r6.type = 1;
    r6.time_to_live = 668;

    // r2.time_to_live = 1203;

    Record r2;
    r2.domain_name = "yahoo.com";
    r2.data = {char(98), char(137), char(11), char(163)};
    r2.len = 4;
    r2.record_class = 1;
    r2.type = 1;
    r2.time_to_live = 668;


    Record r3;
    r3.domain_name = "yahoo.com";
    r3.data = {char(74), char(6), char(231), char(21)};
    r3.len = 4;
    r3.record_class = 1;
    r3.type = 1;
    r3.time_to_live = 668;


    Record r5;
    r5.domain_name = "yahoo.com";
    r5.data = {char(74), char(6), char(231), char(20)};
    r5.len = 4;
    r5.record_class = 1;
    r5.type = 1;
    r5.time_to_live = 668;


    Record r1;
    r1.domain_name = "yahoo.com";
    r1.data = {char(98), char(137), char(11), char(164)};
    r1.len = 4;
    r1.record_class = 1;
    r1.type = 1;
    r1.time_to_live = 668;

    packet.answers.insert(packet.answers.end(), {r1, r2, r3, r4, r5, r6});
    char ans[] = {
        char(0x9d), char(0x3e), char(0x81), char(0x80), char(0x00), char(0x01), char(0x00), char(0x06), char(0x00), char(0x00), char(0x00), char(0x00), char(0x05), char(0x79), char(0x61), char(0x68),
        char(0x6f), char(0x6f), char(0x03), char(0x63), char(0x6f), char(0x6d), char(0x00), char(0x00), char(0x01), char(0x00), char(0x01), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00),
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x62), char(0x89), char(0x0b), char(0xa4), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00),
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x62), char(0x89), char(0x0b), char(0xa3), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00),
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x4a), char(0x06), char(0xe7), char(0x15), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00),
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x4a), char(0x06), char(0x8f), char(0x19), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00),
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x4a), char(0x06), char(0xe7), char(0x14), char(0xc0), char(0x0c), char(0x00), char(0x01), char(0x00), 
        char(0x01), char(0x00), char(0x00), char(0x02), char(0x9c), char(0x00), char(0x04), char(0x4a), char(0x06), char(0x8f), char(0x1a)
    };

    char buf[512];
    Parser::toBuffer(packet, buf);
    for (int i = 0; i < 123; ++i) {
        assert(ans[i] == buf[i]);
    }
    return 0;
}


/* PART2: SERVER TESTS*/
int main(int argc, char** argv) {
    test1();
    test2();
    test3();
    test4();
    printf("Succes!\n");
    return 0;
}