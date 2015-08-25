/*
 * text_pack.cc
 *
 *  Created on: 2015. 6. 9.
 *      Author: windfree
 */

#include "data_pack/text_pack.h"
#include "data_pack/pack_constants.h"
#include "io/data_output.h"
#include "io/data_input.h"
#include "string.h"
#include "config/configure.h"
#include <iostream>
namespace spotter {


text_pack::text_pack() {
	this->type = "maria";
}

text_pack::text_pack(std::string type, int32_t hash, const char* text) {
	this->type = type;
	this->text = new char[strlen(text) + 1];
	strcpy(this->text, text);
	this->hash = hash;
}

text_pack::~text_pack() {
	delete[] this->text;
	this->text = 0;
}

int16_t text_pack::get_pack_type() {
	return pack_constants::TEXT;
}

void text_pack::write(data_output* out) {
	out->write_string(this->type.c_str());
	out->write_int32(this->hash);
	out->write_string(this->text);

	if(configure::get_instance()->debug_text) {
		std::cout<<"hash:"<<this->hash<<"text:"<<this->text<<std::endl;
	}

}

pack* text_pack::read(data_input* in) {
	this->type = std::string(in->read_string());
	this->hash = in->read_int32();
	this->text = in->read_string();
	return this;

}


} /* namespace spotter */
