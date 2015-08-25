/*
 * map_value.cc
 *
 *  Created on: 2015. 5. 12.
 *      Author: windfree
 */

#include "value/map_value.h"

#include "io/data_output.h"
#include "io/data_input.h"
#include "value/value_constants.h"
namespace spotter {

map_value::map_value() {
	// TODO Auto-generated constructor stub

}

map_value::~map_value() {
	for(std::map<std::string,value*>::iterator it = table.begin(); it != table.end();++it) {
			delete it->second;
	}
}

void map_value::write(data_output* out) {
	out->write_decimal(get_size());
	for(std::map<std::string,value*>::const_iterator it = table.begin(); it != table.end();++it) {
		out->write_string(it->first.c_str());
		out->write_value(it->second);
	}
}

uint8_t map_value::get_value_type() {
	return value_constants::MAP;
}

void map_value::insert(std::string key, value* val) {
	this->table.insert(std::pair<std::string,value*>(key,val));
}

void map_value::erase(std::string key) {
	this->table.erase(key);
}

void map_value::clear() {
	this->table.clear();
}

int map_value::get_size() {
	return table.size();
}

value* map_value::read(data_input* in) {
	int count = (int)in->read_decimal();
	for(int i = 0 ; i < count; i++) {
		std::string key(std::string(in->read_string()));
		value* value = in->read_value();
		this->insert(key,value);
	}
	return this;

}

value* map_value::get_value(std::string key) {
	return table[key];
}

} /* namespace spotter */


