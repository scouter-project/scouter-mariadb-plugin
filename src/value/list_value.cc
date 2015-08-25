/*
 * list_value.cpp
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */
#include <my_global.h>
#include "value/list_value.h"
#include "value/decimal_value.h"
#include "value/text_value.h"

#include "io/data_output.h"
#include "io/data_input.h"
#include "value/value_constants.h"
#include "util/log.h"
namespace spotter {

list_value::list_value() {
	// TODO Auto-generated constructor stub
}

list_value::list_value(std::vector< value*>& value) {
	this->value_list = value;
}
list_value::~list_value() {
		for(std::vector< value*>::iterator it = this->value_list.begin(); it != value_list.end();++it	 ) {
		delete *it;
		*it=0;

	}
	this->value_list.clear();
}
value* list_value::get(int i)  {
	return this->value_list[i];
}

list_value* list_value::add( value* value) {
	this->value_list.push_back(value);
	return this;
}

list_value* list_value::add( value* value[]) {
	for(int i = 0 ; i < sizeof(value); i++){
		this->value_list.push_back(value[i]);
	}
	return this;
}

list_value* list_value::add(uint64_t value) {
	this->value_list.push_back(new decimal_value(value));
	return this;
}

list_value* list_value::add(const char* value) {
	this->value_list.push_back(new text_value(value));
	return this;
}
void list_value::write(data_output* out) {
	int size = get_size();
	out->write_decimal(size);
	for(int i = 0; i < size; i++	) {
		value* value = get(i);
		out->write_value(value);
	}

}

uint8_t list_value::get_value_type() {
	return value_constants::LIST;
}

int list_value::get_size() {
	return value_list.size();
}

value* list_value::read(data_input* in) {
	int count = (int)in->read_decimal();
	for(int i = 0 ; i < count; i++) {
		value* val = in->read_value();
		add(val);
		//add(in->read_value());
	}
	return this;
}

} /* namespace spotter */

