/*
 * util.h
 *
 *  Created on: 2015. 4. 8.
 *      Author: windfree
 */

#ifndef PLUGIN_SPOTTER_UTILITY_H_
#define PLUGIN_SPOTTER_UTILITY_H_
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
namespace spotter {

class util {
	static int hash_table[];
public:
	virtual ~util();
	static uint64_t htonll(uint64_t val);
	static uint64_t ntohll(uint64_t val);
	static int32_t hash(const char* str);
	static uint64_t strtouint64(const char* str);
	static uint64_t get_current_time();
	static std::string to_string(int64_t num);
	static uint64_t get_seconds(const char* time, const char* format);
	static bool sleep(int32_t sec);

};

} /* namespace spotter */

#endif /* PLUGIN_SPOTTER_UTILITY_H_ */
