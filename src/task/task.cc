/*
 * task.cc
 *
 *  Created on: 2015. 6. 15.
 *      Author: Kim Kyungnam
 */

#include "task/task.h"
#include "net/net_proxy.h"
#include "sql_base.h"
namespace spotter {

task::task() {
	shutdown_plugin= false;
	ctx = NULL;
	netproxy = net_proxy::get_instance();
	is_running = false;
	spotter_thread_id = -1;

}

task::~task() {

}

bool task::going_down() {
	return shutdown_plugin || shutdown_in_progress ;
}

int task::slept_ok(time_t sec) {
  struct timespec abstime;
  int ret= 0;
  set_timespec(abstime, sec);
  mysql_mutex_lock(&sleep_mutex);
  while (!going_down() && ret != ETIMEDOUT) {
	  ret= mysql_cond_timedwait(&sleep_condition, &sleep_mutex, &abstime);
	}
  mysql_mutex_unlock(&sleep_mutex);

  return !going_down();
}

void task::run(void* p) {
	if(is_running) {
					return ;
		}
	mysql_mutex_init(0, &sleep_mutex, 0);
	mysql_cond_init(0, &sleep_condition, 0);
	shutdown_plugin= false;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,	PTHREAD_CREATE_JOINABLE);
	if (pthread_create(&spotter_thread_id, &attr, start_thread,(void *) this)!= 0) {
		is_running = false;
		mysql_cond_destroy(&sleep_condition);
		mysql_mutex_destroy(&sleep_mutex);
		sql_print_error("Could not create spotter reader thread!\n");
		exit(0);
	}
	is_running = true;
}

void task::stop() {
	if(!is_running) {
			return;
		}
	is_running=false;
	mysql_mutex_lock(&sleep_mutex);
	shutdown_plugin= true;
	mysql_cond_signal(&sleep_condition);
	mysql_mutex_unlock(&sleep_mutex);
	if(spotter_thread_id != -1 ) {
		pthread_cancel(spotter_thread_id);
		pthread_join(spotter_thread_id,NULL);
	}
	mysql_mutex_destroy(&sleep_mutex);
	mysql_cond_destroy(&sleep_condition);

}

void task::set_interval(int val) {
	interval = val;
}

bool task::is_run() {
	return is_running;
}
} /* namespace spotter */


