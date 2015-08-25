/*
 * numeric_value.h
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#ifndef SRC_VALUE_DECIMAL_VALUE_H_
#define SRC_VALUE_DECIMAL_VALUE_H_
#include <stdint.h>
#include "number_value.h"
class value;
namespace spotter {
class data_output;
class data_input;
class decimal_value: public number_value {
private:
	uint64_t dvalue;
public:
	decimal_value();
	decimal_value(uint64_t val);
	virtual ~decimal_value();
	void write(data_output* out)  ;
	value* read(data_input* in) ;
	uint8_t get_value_type() ;
	int32_t get_int32_value()  ;
	uint32_t get_uint32_value() ;
	int64_t get_int64_value()  ;
	uint64_t get_uint64_value() ;
	int16_t get_int16_value()  ;
	uint16_t get_uint16_value() ;
	uint64_t get_value();



};

} /* namespace spotter */

#endif /* SRC_VALUE_DECIMAL_VALUE_H_ */
