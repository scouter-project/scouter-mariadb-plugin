/*
 * spotter_common.h
 *
 *  Created on: 2015. 8. 6.
 *      Author: windfree
 */

#ifndef SRC_INCLUDE_COMMON_SPOTTER_VAR_H_
#define SRC_INCLUDE_COMMON_SPOTTER_VAR_H_

#include <pthread.h>
namespace spotter {
extern pthread_cond_t spotter_cond;
extern char plugin_run;
extern int text_hash_set_size;
extern char* server_ip;
extern int server_udp_port;
extern int server_tcp_port;
extern int net_queue_size;
extern int status_interval;
extern int stmt_current_interval;
extern int stmt_summary_interval;
extern int lock_interval;
extern char use_pfs; // not use.
#endif /* SRC_INCLUDE_COMMON_SPOTTER_VAR_H_ */
}
