/*
 * null_value.cc
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#include "value/null_value.h"
#include "io/data_output.h"
#include "value/value_constants.h"
namespace spotter {

null_value::null_value() {
	// TODO Auto-generated constructor stub

}

null_value::~null_value() {
	// TODO Auto-generated destructor stub
}


uint8_t null_value::get_value_type() {
	return value_constants::NULL_VALUE;
}

void null_value::write(data_output* out) {

}

value* null_value::read(data_input* in) {
	return this;
}

} /* namespace spotter */


