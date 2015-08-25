/*
 * value_constants.h
 *
 *  Created on: 2015. 5. 11.
 *      Author: windfree
 */

#ifndef SRC_VALUE_VALUE_CONSTANTS_H_
#define SRC_VALUE_VALUE_CONSTANTS_H_

#include <stdint.h>
namespace spotter {
class value;
class value_constants {
public:
		const static uint8_t NULL_VALUE;
	const static uint8_t BOOLEAN;
	const static uint8_t DECIMAL;
	const static uint8_t FLOAT;
  const static uint8_t DOUBLE;

  const static uint8_t DOUBLE_SUMMARY;
  const static uint8_t LONG_SUMMARY;

  const static uint8_t TEXT;
  const static uint8_t TEXT_HASH;
  const static uint8_t BLOB;
  const static uint8_t IP4ADDR;

  const static uint8_t LIST;

  const static uint8_t ARRAY_INT;
  const static uint8_t ARRAY_FLOAT;
  const static uint8_t ARRAY_TEXT;

  const static uint8_t MAP;
  static value* create(int8_t code);


};


} /* namespace spotter */

#endif /* SRC_VALUE_VALUE_CONSTANTS_H_ */
