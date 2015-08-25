/*
 * text_pack.h
 *
 *  Created on: 2015. 6. 9.
 *      Author: windfree
 */

#ifndef SRC_DATA_PACK_TEXT_PACK_H_
#define SRC_DATA_PACK_TEXT_PACK_H_

#include "data_pack/pack.h"

#include <string>
namespace spotter {

class text_pack: public pack {
private:
	std::string type;
	int32_t hash;
	char* text;
public:
	text_pack();
	text_pack(std::string type, int32_t hash, const char* text);
	virtual ~text_pack();
	int16_t get_pack_type();
	void write(data_output* out);
	pack* read(data_input* in);
};

} /* namespace spotter */

#endif /* SRC_DATA_PACK_TEXT_PACK_H_ */
