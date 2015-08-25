/*
 * spotter.h
 *
 *  Created on: 2015. 6. 3.
 *      Author: windfree
 */

#ifndef SRC_SPOTTER_H_
#define SRC_SPOTTER_H_

#define MYSQL_SERVER 1
#include <sql_class.h>
#include "task/pfs_event_stmt_current_reader.h"
#include "task/pfs_event_stmt_summary_reader.h"
#include "task/ifs_global_status_reader.h"
namespace spotter {
class spotter {
public:
	spotter();
	virtual ~spotter();
};
//extern mysql_mutex_t Pfs_event_stmt_current_reader::sleep_mutex;
//extern mysql_cond_t sleep_condition;
//extern volatile bool shutdown_plugin;

} /* namespace spotter */

#endif /* SRC_SPOTTER_H_ */
