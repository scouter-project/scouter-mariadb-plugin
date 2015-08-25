/*
 * data_input.cpp
 *
 *  Created on: 2015. 4. 20.
 *      Author: windfree
 */
#include "my_global.h"
#include <string.h>
#include <arpa/inet.h>
#include "util/util.h"
#include "value/value_constants.h"
#include "value/value.h"
#include "io/data_input.h"
namespace spotter {


data_input::data_input(char* buff) {
	buffer = buff;
	offset = 0;
	buffer_length = 0;

}

data_input::~data_input() {
	/*if(buffer != NULL) {
		delete buffer;
	}*/
	delete[] buffer;
	buffer = 0;
}

int64_t data_input::read_decimal() {
	char type = read_uint8();
	switch(type){
	case 0:
		return 0;
	case 1:
		return read_uint8();
	case 2:
		return read_int16();
	case 4:
		return read_int32();
	case 8:
		return read_int64();
	default:
		return read_int64();
	}
}
uint32_t data_input::read_uint32() {
	uint32_t temp = 0;
	memcpy(&temp,buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return ntohl(temp);

}

uint64_t data_input::read_uint64() {
	uint64_t temp = 0;
	memcpy(&temp,buffer + offset, sizeof(uint64_t));
	offset += sizeof(uint64_t);
	return util::ntohll(temp); //todo: use Util
}

int32_t data_input::read_int32() {
	uint32_t temp = 0;
	memcpy(&temp,buffer + offset, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	return (int32_t)ntohl(temp);
}

int64_t data_input::read_int64() {
	uint64_t temp = 0;
	memcpy(&temp,buffer + offset, sizeof(uint64_t));
	offset += sizeof(uint64_t);
	return (int64_t)util::ntohll(temp); //todo: use Util
}

uint8_t data_input::read_uint8() {
	return (uint8_t)buffer[offset++];
}

int8_t data_input::read_int8() {
	return (int8_t)buffer[offset++];
}


uint16_t data_input::read_uint16() {
	ushort temp = 0;
	memcpy(&temp,buffer + offset, sizeof(ushort));
	offset += sizeof(ushort);
	return ntohs(temp);
}

int16_t data_input::read_int16() {
	uint16_t temp = 0;
	memcpy(&temp,buffer + offset, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	return (int16_t)ntohs(temp);
}

char* data_input::read_string() {
	uint32_t size = read_uint32();
	char* str = new char[size + 1];
	memcpy(str,buffer + offset,size);
	str[size] = 0;
	offset+=size;
	return str;
}

value* data_input::read_value() {
	int8_t value_type = read_uint8();
	value* value = value_constants::create(value_type);
	//return  value_constants::create(value_type)->read(this);
	value->read(this);
	return value;
}

} /* namespace spotter */

