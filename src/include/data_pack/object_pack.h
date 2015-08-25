/*
 * object_pack.h
 *
 *  Created on: 2015. 6. 15.
 *      Author: windfree
 */

#ifndef SRC_INCLUDE_DATA_PACK_OBJECT_PACK_H_
#define SRC_INCLUDE_DATA_PACK_OBJECT_PACK_H_

#include "data_pack/pack.h"
#include "value/map_value.h"
namespace spotter {

class object_pack: public pack {
private:
	std::string obj_type;
	std::string obj_name;
	int obj_hash;
	map_value* mvalue;
	std::string address;
	std::string version;
	uint64_t wakeup;
	bool alive;
public:
	object_pack();
	virtual ~object_pack();
	void put(std::string key, std::string value);
	void set_version(std::string ver);
	void set_obj_type(std::string value);
	void set_obj_name(std::string value);
	void set_address(std::string value);
	void set_obj_hash(int value);
	void write(data_output* out);
	pack* read(data_input* in);
	int16_t get_pack_type();
	std::string get_version();
};

} /* namespace spotter */

#endif /* SRC_INCLUDE_DATA_PACK_OBJECT_PACK_H_ */
