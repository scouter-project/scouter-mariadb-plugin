/*
 * configure.h
 *
 *  Created on: 2015. 6. 23.
 *      Author: windfree
 */

#ifndef SRC_CONFIG_CONFIGURE_H_
#define SRC_CONFIG_CONFIGURE_H_

#include <pthread.h>
#include <stdint.h>
namespace spotter {

class configure {
private:
	static configure* instance;
	static pthread_mutex_t mutex;

	configure();

public:
	static configure* get_instance();
	virtual ~configure();
	static int32_t MAX_PACKET_SIZE;
	bool debug_text ;

};

} /* namespace spotter */

#endif /* SRC_CONFIG_CONFIGURE_H_ */
