/*
 * text_value.cc
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */
#include <string.h>
#include "value/text_value.h"
#include "io/data_output.h"
#include "io/data_input.h"
#include "value/value_constants.h"
namespace spotter {

text_value::text_value() {
	tvalue = NULL;
}

text_value::~text_value() {
	if(tvalue != NULL) {
		delete[] tvalue;
	}
}

uint8_t text_value::get_value_type() {
	return value_constants::TEXT;
}

void text_value::write(data_output* out) {
	out->write_string(tvalue);

}

text_value::text_value(const char* val) {
	tvalue = new char[strlen(val) + 1];
	strcpy(tvalue, val);

}

text_value::text_value(const text_value& clazz) {
	tvalue = new char[strlen(clazz.tvalue) + 1];
	strcpy(tvalue, clazz.tvalue);
}

text_value& text_value::operator=(const text_value& clazz) {
	if(tvalue != NULL) {
		delete tvalue;
	}
	tvalue = new char[strlen(clazz.tvalue) + 1];
	strcpy(tvalue, clazz.tvalue);
	return *this;
}

value* text_value::read(data_input* in) {
	this->tvalue = in->read_string();
	return this;
}

char* text_value::get_value() {
	return this->tvalue;
}

} /* namespace spotter */


