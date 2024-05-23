// #pragma once
#include "Parser.hpp"
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

void setInt(char* buffer, int& pos, int32_t data) {
    buffer[pos] = (data >> 24) & 0x000000ff;
    buffer[pos + 1] = (data >> 16) & 0x000000ff;
    buffer[pos + 2] = (data >> 8) & 0x000000ff;
    buffer[pos + 3] = (data) & 0x000000ff;
    pos += sizeof(int32_t);
}
void setShort(char* buffer, int& pos, int16_t data) {
    buffer[pos] = (data >> 8) & 0x00ff;
    buffer[pos + 1] = (data) & 0x00ff;

    pos += sizeof(int16_t);
}
void setByte(char* buffer, int& pos, char data) {
    *((int8_t*)buffer + pos) = data;
    pos += sizeof(int8_t);
}
/*Returns Byte Written*/
void writeHeader(const Packet& packet, int& idx, char* out) {
    const Header& header = packet.header;
    setShort(out, idx, header.id);
    uint16_t flags = 0;

    flags = (header.query_response) << 7 | (header.opcode) << 6 | (header.authorative_answer) << 3 | header.truncated_message << 2 | header.recursion_desired;
    flags = flags << 8;
    flags |= ((header.recursion_available << 7 | header.reserved << 6 + header.result_code) & 0x0000ffff);
    setShort(out, idx, flags);
    setShort(out, idx, header.question_count);
    setShort(out, idx, header.answer_count);
    setShort(out, idx, header.authorative_answer);
    setShort(out, idx, header.additional_records_count);
}
void setData(char* out, int& idx, std::string data) {
    for (const auto& letter: data) {
        out[idx++] = letter;
    }
}
/*TODO Implement Compression*/
void writeString(char* out, int& idx, const std::string& name, std::unordered_map<std::string, int>& dn_idx) {
    if (dn_idx.count(name) != 0) {
        int word_idx = dn_idx.at(name);
        setByte(out, idx, 0xC0);
        setByte(out, idx, word_idx & 0x000000FF);
        return;
    }
    dn_idx.insert({name, idx});
    std::string cur;
    for (const auto& letter: name) {
        if (letter == '.') {
            setByte(out, idx, cur.size());
            setData(out, idx, cur);
            cur = "";
        } else {
            cur += letter;
        }
    }
    if (!cur.empty()) {
        setByte(out, idx, cur.size());
        setData(out, idx, cur); 
    }
    setByte(out, idx, 0);
}
void writeQuestions(const Packet& packet, int& idx, char* out, std::unordered_map<std::string, int>& dn_idx) {
    for (const auto& question: packet.questions) {
        writeString(out, idx, question.name, dn_idx);
        setShort(out, idx, question.type);
        setShort(out, idx, question.question_class);
    }
}
void writeRecord(const Record& record, int& idx, char* out, std::unordered_map<std::string, int>& dn_idx) {
    writeString(out, idx, record.domain_name, dn_idx);
    setShort(out, idx, record.type);
    setShort(out, idx, record.record_class);
    setInt(out, idx, record.time_to_live);
    setShort(out, idx, record.len);
    // Todo Support more Record Types
    const char* data = record.data.c_str();
    memcpy(out + idx, data, record.data.size());
    idx += record.data.size();
}
void writeAllRecords(const Packet& packet, int& idx, char* out, std::unordered_map<std::string, int>& dn_idx) {

    for (const auto& answer: packet.answers) {
        writeRecord(answer, idx, out, dn_idx);
    }

    for (const auto& authorities: packet.authorities) {
        writeRecord(authorities, idx, out, dn_idx);
    }

    for (const auto& additional_records: packet.additional_records) {
        writeRecord(additional_records, idx, out, dn_idx);
    }
}
void Parser::toBuffer(const Packet& packet, char out[PACKET_SIZE]) {
    int idx = 0;
    writeHeader(packet, idx, out);
    std::unordered_map<std::string, int> domain_name_indexes;

    writeQuestions(packet, idx, out, domain_name_indexes);
    writeAllRecords(packet, idx, out, domain_name_indexes);

}










char readByte(char* data, int& index) {
    int8_t result =  data[index];
    index += sizeof(int8_t);
    return result;
}
int16_t readShort(char* data, int& index) {
    data = data + index;
    int16_t result = (
        ((uint16_t)(data[0] << 8) & 0xff00) | 
        ((uint16_t)(data[1]) & 0x00ff)
    );
    index += sizeof(int16_t);
    return result;
    
}
int32_t readInt(char* data, int& index) {
    int32_t result = *(int32_t*)(data + index);
    index += sizeof(int32_t);
    return result;
}

#define DNS_MAX_JUMPS 10
std::string readName(char* buffer, int& index) {
    int position = index;
    std::string name;
    bool jumped = true;
    int num_jumps = 0;

    while (1) {
        uint8_t length = readByte(buffer, position);
        if (length == 0) break;
        if ((length & 0xC0) == 0xC0) { // Upper two bits are set
            uint8_t next_byte = readByte(buffer, position);
            if (num_jumps >= DNS_MAX_JUMPS) return "";
            position = (((uint16_t)length ^ 0xC0) << 8) + next_byte;
            jumped = true;
            num_jumps++;
        } else {
            jumped = false;
            name += std::string(buffer + position, length);
            name += ".";
            position += length;
        }
    }
    if ((buffer[index] & 0xC0) == 0xC0) {
        index += 2;
    } else {
        index = position;
    }
    name.pop_back();
    return name;
}
void readHeader(Packet& packet, char* buffer, int& idx) {
    // int idx = 0;
    packet.header.id = readShort(buffer, idx); //readShort(buffer, idx);
    uint16_t other = readShort(buffer, idx); //reader;
    packet.header.query_response = (other & 0x8000) >> 15;
    packet.header.opcode = (other & 0x7800) >> 11;
    packet.header.authorative_answer = (other & 0x0400) >> 10;
    packet.header.truncated_message = (other & 0x0200) >> 9;
    packet.header.recursion_desired = (other & (0x0100)) >> 8;

    packet.header.recursion_available = (other & 0x0080) >> 7;
    packet.header.reserved = (other & 0x0070) >> 4;

    packet.header.result_code = ResultCode(other & 0x000F);
    packet.header.question_count = readShort(buffer, idx);
    packet.header.answer_count = readShort(buffer, idx);
    packet.header.authority_count = readShort(buffer, idx);
    packet.header.additional_records_count = readShort(buffer, idx);
    // return idx;
}
void readQuestion(Packet& packet, char* buffer, int& idx) {
    // int idx = 0;
    packet.questions.resize(packet.header.question_count);
    for (int i = 0; i < packet.questions.size(); ++i) {
        Question question;
        question.name = readName(buffer, idx);
        int res = readShort(buffer, idx);
        question.type = QueryType(res);
        question.question_class = readShort(buffer, idx);
        packet.questions.at(i) = question;
    }
}
Record readRecord(int& idx, char* buffer) {
    Record record;
    record.domain_name = readName(buffer, idx);//reader.read_domain_name();
    record.type = QueryType(readShort(buffer, idx));

    record.record_class = readShort(buffer, idx);

    record.time_to_live = readInt(buffer, idx);

    record.len = readShort(buffer, idx);

    record.data = std::string(buffer + idx, record.len);//reader.read_next(record.len);

    idx += record.len;
    return record;
}
int readAllRecords(Packet& packet, char* buffer, int& idx) {

    packet.answers.resize(packet.header.answer_count);
    packet.authorities.resize(packet.header.authority_count);
    packet.additional_records.resize(packet.header.additional_records_count);
    for (int i = 0; i < packet.answers.size(); ++i) {
        packet.answers.at(i) = readRecord(idx, buffer);
    }

    for (int i = 0; i < packet.authorities.size(); ++i) {
        packet.authorities.at(i) = readRecord(idx, buffer);
    }


    for (int i = 0; i < packet.additional_records.size(); ++i) {
        packet.additional_records.at(i) = readRecord(idx, buffer);
    }

    return idx;
}
Packet Parser::fromBuffer(char buffer[PACKET_SIZE]) {
    Packet packet;

    int index = 0;
    readHeader(packet, buffer, index);

    readQuestion(packet, buffer, index);
    readAllRecords(packet, buffer, index);


    return packet;
}