/*
 * pfs_pack.cc
 *
 *  Created on: 2015. 5. 7.
 *      Author: windfree
 */

#include "data_pack/table_pack.h"

#include "data_pack/pack_constants.h"
#include "value/map_value.h"
#include "value/list_value.h"
#include "util/util.h"
#include "util/obj_name_util.h"
#include "io/data_output.h"
#include "io/data_input.h"
namespace spotter {

table_pack::table_pack() {
	map_val = new map_value();
	time = util::get_current_time();
	obj_type ="mariaplugin";
	obj_hash = obj_name_util::get_instance()->object_hash();

}

table_pack::~table_pack() {
	delete map_val;
	map_val = 0;
}

int16_t table_pack::get_pack_type() {
	return pack_constants::TABLE;
}


table_pack* table_pack::new_list_value(std::string key, list_value* value) {
	this->map_val->insert(key,value);
	return this;
}

void table_pack::write(data_output* out) {
	out->write_decimal(time);
	out->write_string(obj_type.c_str());
	out->write_decimal(obj_hash);
	out->write_string(key.c_str());
	out->write_value(map_val);

}

pack* table_pack::read(data_input* in) {
	this->time = in->read_decimal();
	this->obj_type=in->read_string();
	this->obj_hash = in->read_decimal();
	this->key=in->read_string();
	this->map_val = (map_value*)in->read_value();
	return this;
}

table_pack* table_pack::set_time(uint64_t val) {
	time = val;
	return this;
}

table_pack* table_pack::set_obj_type(std::string val) {
	obj_type = val;
	return this;
}

table_pack* table_pack::set_obj_hash(int32_t val) {
	obj_hash = val;
	return this;
}

table_pack* table_pack::set_key(std::string val) {
	key = val;
	return this;
}


} /* namespace spotter */

