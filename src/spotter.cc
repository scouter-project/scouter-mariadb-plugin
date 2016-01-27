/*
 * spotter.cc

 *
 *  Created on: 2015. 6. 3.
 *      Author: Kim Kyungnam
 */

#include "spotter.h"
#include <mysql/plugin.h>
#include "net/net_proxy.h"

#include "util/log.h"
#include "util/util.h"
#include "util/obj_name_util.h"
#include "manager/service_manager.h"
#include "manager/command_manager.h"
#include "task/observer.h"
namespace spotter {
char plugin_run;
int text_hash_set_size;
int net_queue_size;
int status_interval;
int stmt_current_interval;
int stmt_summary_interval;
int lock_interval;
int server_udp_port;
int server_tcp_port;
char* server_ip;
char use_pfs; // not use.
static pthread_t spotter_thread;

observer* spotter_observer;
pthread_cond_t spotter_cond;
/**
   plugin initialization function
*/
pthread_handler_t run(void *arg __attribute__((unused)))
{
	while(mysqld_server_initialized == 0 ) {
		sleep(3);
	}

	if (my_thread_init()) {
	    return 0;
	}
	command_manager* cmd_mgr = command_manager::get_instance();
	cmd_mgr->run();

/*
	service_manager* srv_mgr = service_manager::get_instance();
	srv_mgr->set_plugin_arg(arg);
	srv_mgr->start();
*/
	spotter_observer = new observer();
	spotter_observer->set_interval(5);
	spotter_observer->run(NULL);
	my_thread_end();
	pthread_exit(0);
	return 0;
}

void init_component() {
	net_proxy::get_instance();
	obj_name_util::get_instance();
}

void deinit_component() {
	delete log::get_instance(); // erase.
	delete net_proxy::get_instance();
	delete obj_name_util::get_instance();

}
static int init(void *p) {
	init_component();
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	if (pthread_create(&spotter_thread, &attr, run, p) != 0)
	{
		sql_print_error("spotter plugin: failed to start a thread");
		return 1;
	}
	return 0;
}

/**
 *
   plugin initialization function
*/
static int deinit(void *p) {
	DBUG_PRINT("spotter-deinit",("observer"));
	if(spotter_observer != 0) {
		spotter_observer->stop();
		delete spotter_observer;
	}
	DBUG_PRINT("spotter-deinit",("srv-mgr"));
	service_manager* svc_mgr = service_manager::get_instance();
	if(svc_mgr->is_run()) {
		svc_mgr->stop();
	}
	delete svc_mgr;

	DBUG_PRINT("spotter-deinit",("cmd_mgr"));
	command_manager* cmd_mgr = command_manager::get_instance();
	cmd_mgr->stop();
	delete cmd_mgr;

	pthread_cancel(spotter_thread);
	pthread_join(spotter_thread, NULL);

	deinit_component();


	return 0;
}

static MYSQL_SYSVAR_BOOL(run,plugin_run,PLUGIN_VAR_RQCMDARG,
													"Flag for plugin start or stop.",
													NULL,NULL,TRUE	);
static MYSQL_SYSVAR_INT(text_list_size,text_hash_set_size,PLUGIN_VAR_RQCMDARG,
												"size of text hash list",
												NULL,NULL,20000,20000,100000,1024);
static MYSQL_SYSVAR_INT(net_queue_size,net_queue_size,PLUGIN_VAR_RQCMDARG,
												"size of socket data queue size. ",
												NULL,NULL,200,200,1000,1);

static MYSQL_SYSVAR_INT(status_interval,status_interval,PLUGIN_VAR_RQCMDARG,
												"interval of reading global_status table.",
												NULL,NULL,2,1,60,1);
static MYSQL_SYSVAR_INT(stmt_current_interval,stmt_current_interval,PLUGIN_VAR_RQCMDARG,
												"interval of reading events_statements_current table.",
												NULL,NULL,2,1,60,1);
static MYSQL_SYSVAR_INT(stmt_summary_interval,stmt_summary_interval,PLUGIN_VAR_RQCMDARG,
												"interval of reading events_statements_summary_by_digest table.",
												NULL,NULL,10,10,120,1);
static MYSQL_SYSVAR_INT(lock_interval,lock_interval,PLUGIN_VAR_RQCMDARG,
												"interval of reading innodb lock information",
												NULL,NULL,2,1,60,1);
static MYSQL_SYSVAR_BOOL(use_pfs,use_pfs,PLUGIN_VAR_RQCMDARG,
													"use performance_schema or not",
													NULL,NULL,TRUE);
static MYSQL_SYSVAR_STR(server_ip,server_ip,PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_MEMALLOC,
														"scouter server ip",
														NULL,NULL,"server_ip");
static MYSQL_SYSVAR_INT(server_udp_port,server_udp_port,PLUGIN_VAR_RQCMDARG,
												"scouter server port",
												NULL,NULL,6100,3000,40000,1);

static MYSQL_SYSVAR_INT(server_tcp_port,server_tcp_port,PLUGIN_VAR_RQCMDARG,
												"scouter server port",
												NULL,NULL,6100,3000,40000,1);

static struct st_mysql_sys_var* my_settings[] = {
  MYSQL_SYSVAR(run),
  MYSQL_SYSVAR(text_list_size),
  MYSQL_SYSVAR(net_queue_size),
  MYSQL_SYSVAR(status_interval),
  MYSQL_SYSVAR(stmt_current_interval),
  MYSQL_SYSVAR(stmt_summary_interval),
  MYSQL_SYSVAR(lock_interval),
  MYSQL_SYSVAR(use_pfs),
  MYSQL_SYSVAR(server_ip),
  MYSQL_SYSVAR(server_udp_port),
  MYSQL_SYSVAR(server_tcp_port),
  NULL
};

static struct st_mysql_daemon spotter = { MYSQL_DAEMON_INTERFACE_VERSION };

}
/* namespace spotter */
maria_declare_plugin(spotter) {
MYSQL_DAEMON_PLUGIN,
&spotter::spotter,
"spotter",
"Kim Kyungnam",
"MariaDB Monitoring by native api",
PLUGIN_LICENSE_GPL,
spotter::init, /* Plugin Init */
spotter::deinit, /* Plugin Deinit */
0x0100 /* 1.0 */,
NULL, /* status variables                */
spotter::my_settings, /* system variables                */
"1.0", /* string version */
MariaDB_PLUGIN_MATURITY_EXPERIMENTAL /* maturity */
}
maria_declare_plugin_end;
