/*
 * spotter_heartbeat.h
 *
 *  Created on: 2015. 6. 12.
 *      Author: windfree
 */

#ifndef SRC_TASK_SPOTTER_HEARTBEAT_H_
#define SRC_TASK_SPOTTER_HEARTBEAT_H_
#include "task/task.h"
namespace spotter {
class obj_name_util;
class spotter_heartbeat : public task{
private:
	obj_name_util* name_util;
protected:
	int execute(void* p);
public:
	spotter_heartbeat();
	virtual ~spotter_heartbeat();
};

} /* namespace spotter */

#endif /* SRC_TASK_SPOTTER_HEARTBEAT_H_ */
