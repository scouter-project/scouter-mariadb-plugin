/*
 * perf_counter_pack.h
 *
 *  Created on: 2015. 5. 20.
 *      Author: windfree
 */

#ifndef SRC_DATA_PACK_MAP_PACK_H_
#define SRC_DATA_PACK_MAP_PACK_H_

#include "pack.h"
#include <stdint.h>
#include <string>
namespace spotter {
class map_value;
//class Data_output;
//class Data_input;
class value;
class perf_counter_pack: public pack {
private:
	uint64_t time;
	std::string obj_name;
	uint8_t time_type;
	map_value* map_val;
	//int16_t pack_type;
public:
	perf_counter_pack();
	~perf_counter_pack();

	perf_counter_pack* set_time(uint64_t val);
	perf_counter_pack* set_obj_name(std::string val);

	int16_t get_pack_type();
	void write(data_output* out);
	pack* read(data_input* in);
	void insert(std::string key, value* value);

};

} /* namespace spotter */

#endif /* SRC_DATA_PACK_MAP_PACK_H_ */
