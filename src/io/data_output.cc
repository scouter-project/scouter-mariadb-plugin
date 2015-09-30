/*
 * data_output.cc
 *
 *  Created on: 2015. 4. 8.
 *      Author: windfree
 */

#include "my_global.h"
#include <arpa/inet.h>
#include <string.h>
#include <limits.h>
#include "sql_string.h"
#include "data_pack/pack.h"
#include "value/value.h"
#include "util/util.h"
#include "io/data_output.h"
//#include <sql_string.h>
namespace spotter {

data_output::data_output() {
	buffer= new char[128];
	offset = 0;
	buffer_length = 128;

}

data_output::data_output(int size) {
	buffer = new char[size];
	offset = 0;
	buffer_length = size;
}

data_output::~data_output() {
	delete[] buffer;
}

void data_output::check(int size) {
	if(offset + size > buffer_length) {
		int new_size = buffer_length * 2;
		if((offset + size) > new_size ) {
			new_size = (offset + size) * 2;
		}
		char* new_buffer = new char[new_size];
		memcpy(new_buffer,buffer,offset);
		delete[] buffer;
		buffer = new_buffer;
		buffer_length = new_size;
	}
}

void data_output::write_string(const char* value) {
	String str(value,&my_charset_utf8_unicode_ci);
	int str_size = str.length();
	write_blob(str.c_ptr(),str_size);

}

void data_output::write_uint32(uint32_t value) {
	check(sizeof(uint32_t));
	uint32_t temp = htonl(value);
	memcpy(buffer + offset,&temp,sizeof(uint32_t));
	offset += sizeof(uint32_t);
}

void data_output::write_uint64(uint64_t value) {
	check(sizeof(uint64_t));
	uint64_t temp = util::htonll(value);
	memcpy(buffer + offset,&temp,sizeof(uint64_t));
	offset += sizeof(uint64_t);
}

void data_output::write_int32(int32_t value) {
	write_uint32((int32_t)value);
}

void data_output::write_int64(int64_t value) {
	write_uint64(value);
}

void data_output::write_uint8(uint8_t value) {
	check(1);
	buffer[offset] = value;
	offset += 1;
}

void data_output::write_int8(int8_t value) {
	write_uint8(value);
}


void data_output::write_uint16(uint16_t value) {
	check(sizeof(uint16_t));
	uint16_t temp = htons(value);
	memcpy(buffer + offset,&temp,sizeof(uint16_t));
	offset += sizeof(uint16_t);
}


void data_output::write_int16(int16_t value) {
	write_uint16(value);
}

void data_output::write_bytes(char* bytes,int size) {
	check(size);
	memcpy(buffer + offset,bytes,size );
	offset += size;
}


char* data_output::to_byte_array() {
	char* return_buffer = new char[offset];
	memcpy(return_buffer,buffer,offset);
	return return_buffer;
}

data_output* data_output::write_pack(pack* pack) {
	this->write_uint8(pack->get_pack_type()	);
	pack->write(this);
	return this;
}

void data_output::write_decimal(int64_t value) {
	if(value == 0) {
		write_uint8(0);
	} else if(SCHAR_MIN <= value && value <=  SCHAR_MAX ) {
		write_uint8(1);
		write_uint8((char)value);
	} else if(SHRT_MIN <= value && value <= SHRT_MAX) {
		write_uint8(2);
		write_int16((int16_t)value);
	} else if(INT_MIN <= value  && value <= INT_MAX) {
		write_uint8(4);
		write_int32((int32_t)value);
	}  else if(LLONG_MIN <= value  && value <= LLONG_MAX) {
		write_uint8(8);
		write_int64(value);
	} else {
		write_uint8(8);
		write_uint64(value);
	}
}
void data_output::write_value( value* value) {
	//char result = value->get_value_type();
	this->write_uint8(value->get_value_type());
	value->write(this);
}
int data_output::get_offset() {
	return offset;
}


void data_output::write_blob(char* bytes, int size) {
	if(size == 0 || bytes == NULL) {
		write_int8(0);
	} else {
		if(size < 253) {
			write_uint8((uint8_t)size);
			write_bytes(bytes,size);
		} else if(size <= 65535) {
			write_uint8((uint8_t)255);
			write_uint16((uint16_t)size);
			write_bytes(bytes,size);
		} else {
			write_uint8((uint8_t)254);
			write_uint32((uint32_t)size);
			write_bytes(bytes,size);
		}
	}
}

} /* namespace spotter */

