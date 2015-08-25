/*
 * value_constants.cc
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#include "value/value_constants.h"
#include "value/null_value.h"
#include "value/list_value.h"
#include "value/map_value.h"
#include "value/text_value.h"
#include "value/decimal_value.h"

namespace spotter {

const uint8_t value_constants::NULL_VALUE = 0 ;
const uint8_t value_constants::BOOLEAN = 10 ;
const uint8_t value_constants::DECIMAL = 20;
const uint8_t value_constants::FLOAT = 30;
const uint8_t value_constants::DOUBLE = 40;

const uint8_t value_constants::DOUBLE_SUMMARY = 45 ;
const uint8_t value_constants::LONG_SUMMARY = 46;

const uint8_t value_constants::TEXT = 50;
const uint8_t value_constants::TEXT_HASH = 51;
const uint8_t value_constants::BLOB = 60;
const uint8_t value_constants::IP4ADDR = 61;

const uint8_t value_constants::LIST = 70;

const uint8_t value_constants::ARRAY_INT = 71 ;
const uint8_t value_constants::ARRAY_FLOAT = 72;
const uint8_t value_constants::ARRAY_TEXT = 73;

const uint8_t value_constants::MAP = 80;

value* value_constants::create(int8_t code) {
	switch(code) {
	case value_constants::NULL_VALUE:
		return new null_value();
	case value_constants::DECIMAL:
		return new decimal_value();
	case value_constants::TEXT:
		return new text_value();
	case value_constants::MAP:
		return new map_value();
	case value_constants::LIST:
		return new list_value();
	}
}

} /* namespace spotter */


