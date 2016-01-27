/*
 * observer.cc
 *
 *  Created on: 2015. 7. 23.
 *      Author: windfree
 */

#include "task/observer.h"

#include "common/spotter_var.h"
#include "manager/service_manager.h"
#include "manager/command_manager.h"
namespace spotter {

observer::observer() {
}

observer::~observer() {
}

int observer::execute(void* p) {
	while(slept_ok(interval)) {
		srv_mgr = service_manager::get_instance();
		if(!plugin_run) {
			srv_mgr->stop();
		} else if(plugin_run ) {
			srv_mgr->start();
		}

		cmd_mgr = command_manager::get_instance();
		if(plugin_run && !cmd_mgr->is_connected()) {
			cmd_mgr->run();
		} else if(!plugin_run && cmd_mgr->is_connected()) {
			cmd_mgr->stop();
		}
	}
	return 1;
}

} /* namespace spotter */
