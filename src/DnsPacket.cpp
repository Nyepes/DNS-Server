#include "DnsPacket.hpp"
#include <iostream>

using namespace DNS;
namespace DNS {
    ostream& operator<<(ostream& os, const Packet& packet) {
        os <<"id: " << packet.header.id<< std::endl;
        os <<"query_response: " << packet.header.query_response<< std::endl;
        os <<"opcode: " << packet.header.opcode<< std::endl;
        os <<"aa: " << packet.header.authorative_answer<< std::endl;
        os <<"tc: " << packet.header.truncated_message<< std::endl;
        os <<"rd: " << packet.header.recursion_desired<< std::endl;
        os <<"ra: " << packet.header.recursion_available<< std::endl;
        os <<"z: " << packet.header.reserved<< std::endl;
        os <<"rcode: " << packet.header.result_code<< std::endl;
        os <<"qcount: " << packet.header.question_count<< std::endl;
        os <<"ancount: " << packet.header.answer_count<< std::endl;
        os <<"nscount: " << packet.header.authority_count<< std::endl;
        os <<"arcount: " << packet.header.additional_records_count<< std::endl;

        os << std::endl;
        for (int i = 0; i < packet.questions.size(); ++i) {
            os <<"qname: " << packet.questions.at(i)->name<< std::endl;
            os <<"qclass: " << packet.questions.at(i)->question_class<< std::endl;
            os <<"qtype: " << packet.questions.at(i)->type<< std::endl;
            os << std::endl;
        }
        os << std::endl;
        for (int i = 0; i < packet.answers.size(); ++i) {
            os <<"rec_name: " << packet.answers.at(i)->domain_name<< std::endl;
            os <<"len: " << packet.answers.at(i)->len<< std::endl;
            os <<"rec_class: " << packet.answers.at(i)->record_class << std::endl;
            os <<"ttl: " << packet.answers.at(i)->time_to_live << std::endl;
            os <<"type: " << packet.answers.at(i)->type << std::endl;

            os << std::endl;
        }
        return os;
    }
}
Packet::Packet(PacketReader& reader) {
    read_header(reader);
    for (int i = 0; i < header.question_count; ++i) {
        read_question(reader);
    }
    for (int i = 0; i < header.answer_count; ++i) {
        read_record(reader);
    }

}
void Packet::read_header(PacketReader& reader) {
    header.id = reader.read_next_2bytes();

    uint16_t other = reader.read_next_2bytes();
    header.query_response = (other & 0x8000) >> 15;

    header.opcode = (other & 0x7800) >> 11;

    header.authorative_answer = (other & 0x0400) >> 10;

    header.truncated_message = (other & 0x0200) >> 9;

    header.recursion_desired = (other & (0x0100)) >> 8;

    header.recursion_available = (other & 0x0080) >> 7;

    header.reserved = (other & 0x0070) >> 4;

    header.result_code = ResultCode(other & 0x000F);

    header.question_count = reader.read_next_2bytes();
    header.answer_count = reader.read_next_2bytes();
    header.authorative_answer = reader.read_next_2bytes();
    header.additional_records_count = reader.read_next_2bytes();
}
void Packet::read_question(PacketReader& reader) {
    Question* question = new Question;
    question -> name = reader.read_domain_name();
    int res = reader.read_next_2bytes();
    question -> type = QueryType(res);
    question -> question_class = reader.read_next_2bytes();
    questions.push_back(question);
}
void Packet::read_record(PacketReader& reader) {
    Record* record = new Record;
    record -> domain_name = reader.read_domain_name();
    record -> type = QueryType(reader.read_next_2bytes());
    record -> record_class = reader.read_next_2bytes();
    record -> time_to_live = reader.read_next_4bytes();
    record -> len = reader.read_next_2bytes();
    record -> data = reader.read_next(record -> len);
    answers.push_back(record);
}
