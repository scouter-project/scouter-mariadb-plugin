/*
 * object_pack.cc
 *
 *  Created on: 2015. 6. 15.
 *      Author: windfree
 */

#include "data_pack/object_pack.h"
#include "data_pack/pack_constants.h"
#include "util/util.h"
#include "value/text_value.h"
#include "io/data_output.h"
#include "io/data_input.h"
namespace spotter {

object_pack::object_pack() {
	mvalue = new map_value();
	alive = true;


}

object_pack::~object_pack() {
	delete mvalue;
}

void object_pack::put(std::string key, std::string value) {
	mvalue->insert(key,new text_value((char*)value.c_str()));
}

void object_pack::set_version(std::string ver) {
	version = ver;
}

void object_pack::set_obj_type(std::string value) {
	obj_type = value;
}

void object_pack::set_obj_name(std::string value) {
	obj_name = value;
}

void object_pack::set_address(std::string value) {
	address = value;
}

void object_pack::set_obj_hash(int value) {
	obj_hash = value;
}

void object_pack::write(data_output* out) {
	out->write_string(obj_type.c_str());
	out->write_decimal(obj_hash);
	out->write_string(obj_name.c_str());
	out->write_string(address.c_str());
	out->write_string(version.c_str());
	out->write_int8(1);
	out->write_decimal(wakeup);
	out->write_value(mvalue);

}


std::string object_pack::get_version() {
	return version;
}

pack* object_pack::read(data_input* in) {
	obj_type = std::string(in->read_string());
	obj_hash = in->read_decimal();
	obj_name = std::string(in->read_string());
	address = std::string(in->read_string());
	version = std::string(in->read_string());
	alive = in->read_uint8();
	wakeup = in->read_decimal();
	mvalue = (map_value*)in->read_value();
	return this;
}

int16_t object_pack::get_pack_type() {
	return pack_constants::OBJECT;
}

} /* namespace spotter */

