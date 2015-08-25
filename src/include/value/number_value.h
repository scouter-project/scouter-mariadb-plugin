/*
 * number_value.h
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#ifndef SRC_VALUE_NUMBER_VALUE_H_
#define SRC_VALUE_NUMBER_VALUE_H_

#include "value.h"

namespace spotter {
class data_output;
class data_input;
class number_value: public value {
public:
	number_value();
	virtual ~number_value();
	void write(data_output* out)  = 0;
	value* read(data_input* in) = 0;
	uint8_t get_value_type() = 0;

	virtual int32_t get_int32_value() =0 ;
	virtual uint32_t get_uint32_value() = 0;
	virtual int64_t get_int64_value() = 0 ;
	virtual uint64_t get_uint64_value() = 0;
	virtual int16_t get_int16_value() = 0 ;
	virtual uint16_t get_uint16_value() = 0;

	//virtual float get_float_value();
	//virtual double get_double_value();
};

} /* namespace spotter */

#endif /* SRC_VALUE_NUMBER_VALUE_H_ */
