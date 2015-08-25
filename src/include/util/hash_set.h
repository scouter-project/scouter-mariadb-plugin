/*
 * hash_pool.h
 *
 *  Created on: 2015. 4. 24.
 *      Author: windfree
 */

#ifndef SRC_UTIL_STRING_HASH_H_
#define SRC_UTIL_STRING_HASH_H_

#include <set>
#include <pthread.h>
#include <stdint.h>
using namespace std;
namespace spotter {

class hash_set {
private:
	pthread_mutex_t hash_set_mutex;
	set<int32_t> value_set;
public:
	hash_set();
	virtual ~hash_set();
	void add(int32_t& hash	);
	bool contains(int32_t val);
	void clear();
	int count();
	set<int32_t> get_array();


};

} /* namespace spotter */

#endif /* SRC_UTIL_STRING_HASH_H_ */
