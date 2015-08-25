/*
 * pack_constants.cc
 *
 *  Created on: 2015. 5. 19.
 *      Author: windfree
 */

#include "data_pack/pack_constants.h"

#include "data_pack/table_pack.h"
#include "data_pack/text_pack.h"
#include "data_pack/perf_counter_pack.h"
#include "data_pack/object_pack.h"

namespace spotter {
const uint8_t pack_constants::TEXT = 50;
const uint8_t pack_constants::PERF_COUNTER = 60;
const uint8_t pack_constants::TABLE = 61;
const uint8_t pack_constants::OBJECT = 80;

pack* pack_constants::create(uint8_t type) {
	switch (type) {
	case pack_constants::TABLE:
		return new table_pack();
	case pack_constants::PERF_COUNTER:
		return new perf_counter_pack();
	case pack_constants::OBJECT:
		return new object_pack();
	case pack_constants::TEXT:
		return new text_pack();
	}
}

} /* namespace spotter */

