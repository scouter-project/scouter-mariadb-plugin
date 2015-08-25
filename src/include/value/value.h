/*
 * value.h
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#ifndef SRC_VALUE_VALUE_H_
#define SRC_VALUE_VALUE_H_

#include <stdint.h>
namespace spotter {
class data_output;
class data_input;
class value {
public:
	value();
	virtual ~value();
	virtual void write(data_output* out)=0;
	virtual value* read(data_input* in) = 0;
	virtual uint8_t get_value_type()  = 0;

};

} /* namespace spotter */

#endif /* SRC_VALUE_VALUE_H_ */
