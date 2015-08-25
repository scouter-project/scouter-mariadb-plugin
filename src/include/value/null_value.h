/*
 * null_value.h
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#ifndef SRC_VALUE_NULL_VALUE_H_
#define SRC_VALUE_NULL_VALUE_H_

#include "value.h"

namespace spotter {
class data_input;
class data_output;
class null_value: public value {
public:
	null_value();
	virtual ~null_value();
	uint8_t get_value_type();
	void write(data_output* out);
	value* read(data_input* in);
};

} /* namespace spotter */

#endif /* SRC_VALUE_NULL_VALUE_H_ */
