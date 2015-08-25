/*
 * pack_constants.h
 *
 *  Created on: 2015. 5. 19.
 *      Author: windfree
 */

#ifndef SRC_DATA_PACK_PACK_CONSTANTS_H_
#define SRC_DATA_PACK_PACK_CONSTANTS_H_

#include <stdint.h>

namespace spotter {
class pack;
class pack_constants {
public:
	const static uint8_t PERF_COUNTER;
	const static uint8_t TABLE;
	const static uint8_t TEXT;
	const static uint8_t OBJECT;

	static pack* create(uint8_t type);

};

} /* namespace spotter */

#endif /* SRC_DATA_PACK_PACK_CONSTANTS_H_ */
