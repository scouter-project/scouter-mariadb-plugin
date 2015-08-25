/*
 * hash_pool.cc
 *
 *  Created on: 2015. 4. 24.
 *      Author: windfree
 */

#include <util/hash_set.h>
#include <iostream>
namespace spotter {
hash_set::hash_set() {
	pthread_mutex_init(&hash_set_mutex,NULL);
}

hash_set::~hash_set() {
	pthread_mutex_destroy(&hash_set_mutex);
	value_set.clear();

}

void hash_set::add(int32_t& hash) {
	pthread_mutex_lock(&hash_set_mutex);
	value_set.insert(hash);
	pthread_mutex_unlock(&hash_set_mutex);
}

bool hash_set::contains(int32_t val) {
	return (value_set.find(val) != value_set.end());
}

void hash_set::clear() {
	value_set.clear();
}

int hash_set::count() {
	return value_set.size();

}

set<int32_t> hash_set::get_array() {
	return this->value_set;
}


} /* namespace spotter */
