/*
 * perf_counter_pack.cc
 *
 *  Created on: 2015. 5. 20.
 *      Author: windfree
 */

#include "data_pack/perf_counter_pack.h"
#include "value/map_value.h"
#include "util/util.h"
#include "util/obj_name_util.h"
#include "io/data_output.h"
#include "io/data_input.h"
#include "data_pack/pack_constants.h"
namespace spotter {

perf_counter_pack::perf_counter_pack() {
	time = 0;
	time_type = 1;
	obj_name = obj_name_util::get_instance()->object_name();
	map_val = new map_value();
}

perf_counter_pack::~perf_counter_pack() {
	delete map_val;
	map_val = 0;
}

perf_counter_pack* perf_counter_pack::set_time(uint64_t val) {
	return this;
}

perf_counter_pack* perf_counter_pack::set_obj_name(std::string val) {
	return this;
}

int16_t perf_counter_pack::get_pack_type() {
	return pack_constants::PERF_COUNTER;
}

void perf_counter_pack::write(data_output* out) {
	out->write_uint64(time);
	out->write_string(obj_name.c_str());
	out->write_int8(time_type);
	out->write_value(map_val);
}


pack* perf_counter_pack::read(data_input* in) {
	time = in->read_uint64();
	obj_name =in->read_string();
	time_type = in->read_uint8();
	map_val = (map_value*)in->read_value(	);
	return this;
}

void perf_counter_pack::insert(std::string key, value* value) {\
	map_val->insert(key, value);
}


} /* namespace spotter */

