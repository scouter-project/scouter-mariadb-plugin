/*
 * ifs_global_status_reader2.h
 *
 *  Created on: 2015. 5. 26.
 *      Author: windfree
 */

#ifndef SRC_READER_IFS_GLOBAL_STATUS_READER2_H_
#define SRC_READER_IFS_GLOBAL_STATUS_READER2_H_

#ifndef MYSQL_SERVER
#define MYSQL_SERVER
#endif

#include "my_global.h"
#include "task/task.h"
#include <map>
#include "sql_show.h"
class THD;
class map;
struct TABLE_LIST;
struct TABLE;

namespace spotter {
class perf_counter_pack;
class ifs_global_status_reader: public task {
private:
	TABLE_LIST tables;
	TABLE* table;
	THD* thd;
	const char* tbl_name;
	ST_FIELD_INFO* spotter_fields;
	LEX_STRING* filter;
	ST_SCHEMA_TABLE* is_spotter;

	void init_filter();
	void init_fields();
	bool	init_table(THD* thd);
	bool fill(THD *thd, TABLE_LIST *tables, COND *unused);
	bool check(const char* name);
	void send_data();
	void calc(std::map<std::string,uint64_t*>& map, perf_counter_pack* pack);
	uint64_t calc_delta(std::string	 key, std::map<std::string,uint64_t*>& map);
	int32_t get_seconds();

	std::map<std::string,uint64_t*> value_map;
	bool first_read;
	int32_t last_read_time;


public:
	ifs_global_status_reader(void* p,std::string name);
	virtual ~ifs_global_status_reader();
	bool read();
	int execute(void* p) ;
	//void stop();

};

} /* namespace spotter */

#endif /* SRC_READER_IFS_GLOBAL_STATUS_READER2_H_ */
