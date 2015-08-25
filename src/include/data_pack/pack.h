/*
 * pack.h
 *
 *  Created on: 2015. 4. 23.
 *      Author: windfree
 */

#ifndef SRC_DATA_PACK_PACK_H_
#define SRC_DATA_PACK_PACK_H_
#include <stdint.h>
namespace spotter {
class data_output;
class data_input;
class pack {
public:
	pack();
	virtual ~pack();
	virtual void write(data_output* out) = 0;
	virtual pack* read(data_input* in) = 0;
	virtual int16_t get_pack_type() = 0;

};

} /* namespace spotter */

#endif /* SRC_DATA_PACK_PACK_H_ */
