/*
 * numeric_value.cc
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#include "value/decimal_value.h"
#include "io/data_output.h"
#include "io/data_input.h"
#include "value/value_constants.h"
namespace spotter {

decimal_value::decimal_value() {
	dvalue = 0;
}

decimal_value::decimal_value(uint64_t val) {
	this->dvalue = val;
}

decimal_value::~decimal_value() {

}

void decimal_value::write(data_output* out) {
	out->write_decimal(this->dvalue);

}

value* decimal_value::read(data_input* in) {
	this->dvalue = in->read_decimal();
	return this;
}

uint8_t decimal_value::get_value_type() {
	return value_constants::DECIMAL;
}

int32_t decimal_value::get_int32_value() {
	return static_cast<int32_t>(dvalue);
}

uint32_t decimal_value::get_uint32_value() {
	return static_cast<uint32_t>(dvalue);
}

int64_t decimal_value::get_int64_value() {
	return static_cast<int64_t>(dvalue);
}

uint64_t decimal_value::get_uint64_value() {
	return static_cast<uint64_t>(dvalue);
}

int16_t decimal_value::get_int16_value() {
	return static_cast<int16_t>(dvalue);
}

uint16_t decimal_value::get_uint16_value() {
	return static_cast<uint16_t>(dvalue);
}


uint64_t decimal_value::get_value() {
	return dvalue;
}

} /* namespace spotter */

