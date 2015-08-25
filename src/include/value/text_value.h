/*
 * text_value.h
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#ifndef SRC_VALUE_TEXT_VALUE_H_
#define SRC_VALUE_TEXT_VALUE_H_
#include "value.h"

namespace spotter {
class data_output;
class data_input;
class text_value: public value {
private:
	char* tvalue;
public:
	text_value(const char* val);
	text_value();
	text_value(const text_value& clazz);
	text_value& operator=(const text_value& clazz);
	virtual ~text_value();
	uint8_t get_value_type() ;
	void write(data_output* out) ;
	value* read(data_input* in);
	char* get_value();
};

} /* namespace spotter */

#endif /* SRC_VALUE_TEXT_VALUE_H_ */
