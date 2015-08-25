/*
 * service_manager.h
 *
 *  Created on: 2015. 7. 23.
 *      Author: Kim kyungnam
 */

#ifndef SRC_MANAGER_SERVICE_MANAGER_H_
#define SRC_MANAGER_SERVICE_MANAGER_H_
#include "my_global.h"
#include <iostream>
#include <vector>
namespace spotter {
class task;
class service_manager {
private:
	static service_manager* instance;
	static pthread_mutex_t mutex;
	service_manager();
	std::vector<task*> task_list;
	void* plugin_arg;
	bool running;

public:
	static service_manager* get_instance();
	virtual ~service_manager();
	void start();
	void stop();
	void set_plugin_arg(void* p);
	bool is_run();
};

} /* namespace spotter */

#endif /* SRC_MANAGER_SERVICE_MANAGER_H_ */
