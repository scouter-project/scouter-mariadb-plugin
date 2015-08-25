/*
 * pfs_pack.h
 *
 *  Created on: 2015. 5. 7.
 *      Author: windfree
 */

#ifndef SRC_DATA_PACK_TABLE_PACK_H_
#define SRC_DATA_PACK_TABLE_PACK_H_

#include "pack.h"

#include <iostream>
#include <stdint.h>
namespace spotter {
class map_value;
class list_value;
//class Data_output;
//class Data_input;
class table_pack: public pack {
private:
	uint64_t time;
	std::string obj_type;
	int32_t obj_hash;
	std::string key;
	map_value* map_val;


public:
	table_pack();
	virtual ~table_pack();

	table_pack* set_time(uint64_t val);
	table_pack* set_obj_type(std::string val);
	table_pack* set_obj_hash(int32_t val);
	table_pack* set_key(std::string val);
	int16_t get_pack_type();
	void write(data_output* out);
	pack* read(data_input* in);
	table_pack* new_list_value(std::string key, list_value* value);

};

} /* namespace spotter */

#endif /* SRC_DATA_PACK_TABLE_PACK_H_ */
