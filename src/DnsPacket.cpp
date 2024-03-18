#include "../includes/DnsPacket.hpp"

#include <iostream>
#include <cstring>
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
        os <<"rcode: " << (ResultCode) packet.header.result_code<< std::endl;
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
Buffer& Packet::to_buffer(Buffer& buf) {
    buf.pos = 0;
    PacketWriter writer(buf);
    write_header(writer);
    for (auto question: questions) {
        write_question(writer, *question);
    }
    for (auto record: answers) {
        write_record(writer, *record);
    }
    for (auto record: authorities) {
        write_record(writer, *record);
    }
    for (auto record: additional_records) {
        write_record(writer, *record);
    }
    return buf;
}

void Packet::write_header(PacketWriter& writer) {
    writer.write_next_2bytes(header.id);
    uint16_t other = 0;
    other |= header.query_response << 15;
    other |= header.opcode << 11;
    other |= header.authorative_answer << 10;
    other |= header.truncated_message << 9;
    other |= header.recursion_desired << 8;
    other |= header.recursion_available << 7;
    other |= header.reserved << 4;
    other |= header.result_code;
    writer.write_next_2bytes(other);
    writer.write_next_2bytes(questions.size());
    writer.write_next_2bytes(answers.size());
    writer.write_next_2bytes(authorities.size());
    writer.write_next_2bytes(additional_records.size());
}
void Packet::write_question(PacketWriter& writer, Question& question) {
    writer.write_domain_name(question.name);
    writer.write_next_2bytes(question.type);
    writer.write_next_2bytes(question.question_class);
}
void Packet::write_record(PacketWriter& writer, Record& record) {
    writer.write_domain_name(record.domain_name);
    writer.write_next_2bytes(record.type);
    writer.write_next_2bytes(record.record_class);
    writer.write_next_4bytes(record.time_to_live);
    writer.write_next_2bytes(record.len);
    writer.write_next(record.len, record.data);
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
    header.authority_count = reader.read_next_2bytes();
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

Packet::Packet(Packet& other) {
    copy_from_other(other);
}
Packet& Packet::operator=(Packet& RHS) {
    del();
    copy_from_other(RHS);
    return *this;
}
Packet::~Packet() {
    del();
}
void Packet::del() {
    for (int i = 0; i < questions.size(); ++i) {
        free(questions.at(i));
    }
    for (int i = 0; i < answers.size(); ++i) {
        free(answers.at(i));
    }
    for (int i = 0; i < authorities.size(); ++i) {
        free(authorities.at(i));
    }
    for (int i = 0; i < additional_records.size(); ++i ) {
        free(additional_records.at(i));
    }
}
void Packet::copy_from_other(Packet& other) {

    questions.resize(other.questions.size());
    answers.resize(other.answers.size());
    authorities.resize(other.authorities.size());
    additional_records.resize(other.additional_records.size());

    for (int i = 0; i < other.questions.size(); ++i) {
        questions.at(i) = new Question;
        memcpy(questions.at(i), other.questions.at(i), sizeof(Question));
    }
    for (int i = 0; i < other.answers.size(); ++i) {
        answers.at(i) = new Record;
        memcpy(answers.at(i), other.answers.at(i), sizeof(Record));
    }
    for (int i = 0; i < other.authorities.size(); ++i) {
        authorities.at(i) = new Record;
        memcpy(authorities.at(i), other.authorities.at(i), sizeof(Record));
    }
    for (int i = 0; i < other.additional_records.size(); ++i) {
        additional_records.at(i) = new Record;
        memcpy(additional_records.at(i), other.additional_records.at(i), sizeof(Record));
    }
}
