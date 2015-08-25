/*
 * list_value.h
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#ifndef SRC_VALUE_LIST_VALUE_H_
#define SRC_VALUE_LIST_VALUE_H_

#include "value.h"
#include <vector>
namespace spotter {
class data_output;
class data_input;
class list_value: public value {
private:
	std::vector< value*> value_list;
public:
	list_value();
	list_value(std::vector< value*>& value);
	virtual ~list_value();
	value* get(int i);
	list_value* add( value* value	) ;
	list_value* add( value* value[]	) ;
	list_value* add(uint64_t value);
	list_value* add(const char* value);
	void write(data_output* out);
	value* read(data_input* in);
	uint8_t get_value_type();
	int get_size();
};

} /* namespace spotter */

#endif /* SRC_VALUE_LIST_VALUE_H_ */
