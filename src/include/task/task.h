/*
 * task.h
 *
 *  Created on: 2015. 6. 15.
 *      Author: windfree
 */

#ifndef SRC_TASK_TASK_H_
#define SRC_TASK_TASK_H_
#ifndef MYSQL_SERVER
#define MYSQL_SERVER
#endif

//#include "table.h"
#include "sql_base.h"
#include "util/util.h"

namespace spotter {
class net_proxy;
class task {
private:
	virtual bool going_down();
	pthread_t spotter_thread_id;

protected:
	void* plugin_data;
	net_proxy* netproxy;
	mysql_mutex_t sleep_mutex;
	mysql_cond_t sleep_condition;
	volatile bool shutdown_plugin;
	struct mysql_spotter_context* ctx;
	int interval;
	virtual int execute(void* p) = 0 ;
	int slept_ok(time_t sec);
	bool is_running;
	static void* start_thread(void* p) {
										((task*)p)->execute(p);
										return NULL;
	}
public:
	task();

	virtual ~task();
	void run(void* p);
	void stop() ;
	void set_interval(int val);
	bool is_run();
};

} /* namespace spotter */

#endif /* SRC_TASK_TASK_H_ */
