/*
 * data_input.h
 *
 *  Created on: 2015. 4. 20.
 *      Author: windfree
 */

#ifndef PLUGIN_SPOTTER_IO_DATA_INPUT_H_
#define PLUGIN_SPOTTER_IO_DATA_INPUT_H_

#include <stdint.h>
namespace spotter {
class value;
class data_input {
	private:
		char* buffer;
		int buffer_length;
		int offset;

	public:
		data_input(char* buff);
		virtual ~data_input();
		uint32_t read_uint32();
		uint64_t read_uint64();
		int32_t read_int32();
		int64_t read_int64();
		uint8_t read_uint8();
		int8_t read_int8();
		uint16_t read_uint16();
		int16_t read_int16();
		int64_t read_decimal();
		char* read_string();
		value* read_value();

};

} /* namespace spotter */

#endif /* PLUGIN_SPOTTER_IO_DATA_INPUT_H_ */
