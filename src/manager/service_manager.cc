/*
 * service_manager.cc
 *
 *  Created on: 2015. 7. 23.
 *      Author: windfree
 */

#include "manager/service_manager.h"
#include "task/pfs_event_stmt_current_reader.h"
#include "task/pfs_event_stmt_summary_reader.h"
#include "task/ifs_global_status_reader.h"
#include "task/spotter_heartbeat.h"
#include "task/observer.h"
#include "task/ifs_innodb_locks_reader.h"
#include "net/udp_sender.h"
#include "common/spotter_var.h"
namespace spotter {


service_manager* service_manager::instance = 0;
pthread_mutex_t service_manager::mutex;

service_manager::service_manager() {
	running = false;
}

service_manager* service_manager::get_instance() {
	if(instance == 0) {
		pthread_mutex_init(&mutex,0);
		pthread_mutex_lock(&mutex);
		instance = new service_manager();
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);
	}
	return instance;
}

service_manager::~service_manager() {

}

void service_manager::start() {
	udp_sender* sender =  udp_sender::get_instance();
	sender->start();

	pthread_cond_init(&spotter_cond,NULL);

	task* ifs_gstatus_reader = new ifs_global_status_reader(plugin_arg,std::string("spotter_gstatus"));
	ifs_gstatus_reader->set_interval(status_interval);
	ifs_gstatus_reader->run(NULL);
	task_list.push_back(ifs_gstatus_reader);

/*
	task* pfs_current_stmt_reader = new pfs_event_stmt_current_reader();
	pfs_current_stmt_reader->set_interval(stmt_current_interval) ;
	pfs_current_stmt_reader->run(NULL);
	task_list.push_back(pfs_current_stmt_reader);

	task* pfs_summary_stmt_reader = new pfs_event_stmt_summary_reader();
	pfs_summary_stmt_reader->set_interval(stmt_summary_interval) ;
	pfs_summary_stmt_reader->run(NULL);
	task_list.push_back(pfs_summary_stmt_reader);

	task* innodb_lck_reader = new ifs_innodb_locks_reader(plugin_arg,std::string("spotter_innodb_locks"));
	innodb_lck_reader->set_interval(lock_interval);
	innodb_lck_reader->run(NULL);
	task_list.push_back(innodb_lck_reader);

	task* heart_beat_checker = new spotter_heartbeat();
	heart_beat_checker->set_interval(3);
	heart_beat_checker->run(NULL);
	task_list.push_back(heart_beat_checker);
*/
	running = true;

}




void service_manager::stop() {
	pthread_cond_signal(&spotter_cond);

	running = false;
	for(std::vector<task*>::iterator it=task_list.begin(); it!=task_list.end();++it) {
		(*it)->stop();
		delete *it;
	}
	task_list.clear();
	udp_sender::get_instance()->stop();
	pthread_cond_destroy(&spotter_cond);
}

void service_manager::set_plugin_arg(void* p) {
	plugin_arg = p;
}

bool service_manager::is_run() {
	return running;
}

} /* namespace argos */

