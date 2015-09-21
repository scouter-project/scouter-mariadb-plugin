/*
 * ipfs_reader.cpp
 *
 *  Created on: 2015. 4. 13.
 *      Author: windfree
 */

#include "task/pfs_reader.h"
#include "util/log.h"
#include <sql_class.h>

namespace spotter {

pfs_reader::pfs_reader(){
	table = NULL;
	tables.mdl_request.type = MDL_SHARED_READ;
	thd = NULL;
	dbname = NULL;
	tblname = NULL;
}

pfs_reader::~pfs_reader() {

}


bool pfs_reader::going_down() {
	 return shutdown_plugin || shutdown_in_progress || (thd && thd->killed);
}

int pfs_reader::execute(void* p) {
	if (my_thread_init()) {
				return 0;
	}
	if(slept_ok(interval)) {
		while(slept_ok(interval)) {
			thd = new THD();
			long begin_stack;
			thd->thread_stack= (char*) &begin_stack;
			thd->store_globals();
			read();
			delete thd;
			thd = 0;
			set_current_thd(0);
		}
		sql_print_information("spotter gathering thread exit...table name:%s",tblname);
	}

 my_thread_end();
 pthread_exit(0);
 return 0;
}

} /* namespace spotter */

