/*
 * observer.h
 *
 *  Created on: 2015. 7. 23.
 *      Author: Kim kyungnam
 */

#ifndef SRC_INCLUDE_TASK_OBSERVER_H_
#define SRC_INCLUDE_TASK_OBSERVER_H_
#include "task.h"
namespace spotter {
class service_manager;
class command_manager;
class observer: public task {
private:
	service_manager* srv_mgr;
	command_manager* cmd_mgr;
public:
	observer();
	virtual ~observer();
	int execute(void* p);
};

} /* namespace spotter */

#endif /* SRC_INCLUDE_TASK_OBSERVER_H_ */
