/*
 * ifs_innodb_locks_reader.h
 *
 *  Created on: 2015. 6. 29.
 *      Author: windfree
 */

#ifndef SRC_IFS_INNODB_LOCKS_READER_H_
#define SRC_IFS_INNODB_LOCKS_READER_H_

#include "task/task.h"
#include <vector>
class THD;
struct TABLE_LIST;
struct TABLE;
namespace spotter {
//
class ifs_innodb_locks_reader: public task {
private:
	const char* tbl_name;
	ST_SCHEMA_TABLE* is_spotter;

	THD* ib_locks_thd;
	THD* ib_lock_waits_thd;
	THD* ib_trx_thd;

	TABLE_LIST ib_locks_tblist;
	TABLE_LIST ib_lock_waits_tblist;
	TABLE_LIST ib_trx_tblist;
	int32_t wait_count;

	class lock_info {
	public:
		uint64_t wait_started;
		std::string locked_table;
		std::string locked_index;
		std::string locked_type;
		uint64_t waiting_trx_started;
		uint64_t waiting_trx_rows_locked;
		uint64_t waiting_trx_rows_modified;
		uint64_t waiting_pid;
		std::string waiting_query;
		std::string waiting_lock_mode;
		uint64_t blocking_pid;
		std::string blocking_query;
		std::string blocking_lock_mode;
		uint64_t blocking_trx_started;
		uint64_t blocking_trx_rows_locked;
		uint64_t blocking_trx_rows_modified;
	} ;
	//vectors for innodb_locks.
	std::vector<std::string> vec_lock_id;     // index 0
	std::vector<std::string> vec_lock_mode;   // index 2
	std::vector<std::string> vec_lock_type;   // index 3
	std::vector<std::string> vec_lock_table;  // index 4
	std::vector<std::string> vec_lock_index;  // index 5

	//vectors for innodb_lock_waits.
	std::vector<std::string> vec_requesting_trx_id;   // index 0
	std::vector<std::string> vec_requested_lock_id;   // index 1
	std::vector<std::string> vec_blocking_trx_id;     // index 2
	std::vector<std::string> vec_blocking_lock_id;    // index 3

	//vectors for innodb_trx
	std::vector<std::string> vec_trx_id;        							// index 0
	std::vector<uint64_t> vec_trx_started;   						 // index 2
	std::vector<uint64_t> vec_trx_wait_started;   						// index 4
	std::vector<uint64_t> vec_trx_mysql_thread_id;    			// index 6
	std::vector<std::string> vec_trx_query;						   			// index 7
	std::vector<uint64_t> vec_trx_rows_locked;   // index 13
	std::vector<uint64_t> vec_trx_rows_modified;   // index 14

	std::vector<lock_info*> vec_lock_info;

	bool read_ib_locks();
	bool read_ib_lock_waits();
	bool read_ib_trx();
	void join();
	void clear_innodb_lock_data();
	void release_resource();
	void init_column_nm();
	std::string columns[16];

protected:
	bool	init_ib_locks_table(THD* thd);
	bool init_ib_lock_waits_table(THD* thd);
	bool init_ib_trx_table(THD* thd);
	bool fill_ib_locks(THD *thd, TABLE_LIST *ptables, COND *unused);
	bool fill_ib_lock_waits(THD *thd, TABLE_LIST *ptables, COND *unused);
	bool fill_ib_trx(THD *thd, TABLE_LIST *ptables, COND *unused);
	void send_data();

public:
	ifs_innodb_locks_reader(void *p,std::string name);
	virtual ~ifs_innodb_locks_reader();
	bool read();
	void join_innodb_trx(std::string& req_trx_id, std::string& blk_trx_id,lock_info* cinfo);
	void join_innodb_lock(std::string& req_lock_id,std::string& blk_lock_id, lock_info* cinfo);

	int execute(void* p) ;
};

} /* namespace spotter */

#endif /* SRC_IFS_INNODB_LOCKS_READER_H_ */
