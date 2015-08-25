/*
 * map_value.h
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#ifndef SRC_VALUE_MAP_VALUE_H_
#define SRC_VALUE_MAP_VALUE_H_

#include "value.h"
#include <map>
#include <string>
namespace spotter {
class data_output;
class data_input;
class map_value: public value {
private:
	std::map<std::string,value*> table;
public:
	map_value();
	virtual ~map_value();
	void write(data_output* out);
	value* read(data_input* in);
	uint8_t get_value_type();

	void insert(std::string key , value* val);
	void erase(std::string key);
	void clear();
	int get_size();
	value* get_value(std::string key);

};

} /* namespace spotter */

#endif /* SRC_VALUE_MAP_VALUE_H_ */
