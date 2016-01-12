/*
 * ifs_innodb_locks_reader.cc
 *
 *  Created on: 2015. 6. 29.
 *      Author: windfree
 */
#include "my_global.h"
#include "task/ifs_innodb_locks_reader.h"
#include "table.h"
#include "sql_show.h"
#include "sql_parse.h"
#include "sql_class.h"
#include "data_pack/table_pack.h"
#include "data_pack/perf_counter_pack.h"
#include "value/list_value.h"
#include "value/decimal_value.h"
#include "net/net_proxy.h"

namespace spotter {

ifs_innodb_locks_reader::ifs_innodb_locks_reader(void* p,std::string name) {
	tbl_name = name.c_str();
	interval = 1; // default value
	plugin_data = p;
	is_spotter = (ST_SCHEMA_TABLE*)p;
	init_column_nm();
}


ifs_innodb_locks_reader::~ifs_innodb_locks_reader() {

}


bool ifs_innodb_locks_reader::init_ib_locks_table(THD* thd) {
	thd->security_ctx->master_access = 0x100; // to pass check_global_access() in sql_parse.cc
	lex_start(thd);
	mysql_init_select(thd->lex);
	//is_spotter->table_name= tbl_name;
	is_spotter->table_name= "spotter_lock_info";
	ib_locks_tblist.init_one_table(INFORMATION_SCHEMA_NAME.str,
						 INFORMATION_SCHEMA_NAME.length,
						 is_spotter->table_name,
						 strlen(is_spotter->table_name),
						 0, TL_READ);

	ST_SCHEMA_TABLE* sch_ib_locks = find_schema_table(thd, "innodb_locks");

	ib_locks_tblist.schema_table = sch_ib_locks;
	ib_locks_tblist.table= create_schema_table(thd, &ib_locks_tblist);
	ib_locks_tblist.schema_table_name="innodb_locks";

	if (!ib_locks_tblist.table) {
		return false;
	}
	ib_locks_tblist.table->pos_in_table_list= &ib_locks_tblist;
	return true;
}


bool ifs_innodb_locks_reader::init_ib_lock_waits_table(THD* thd) {
	thd->security_ctx->master_access = 0x100; // to pass check_global_access() in sql_parse.cc
	lex_start(thd);
	mysql_init_select(thd->lex);
	is_spotter->table_name= "spotter_lock_wait_info";

	ib_lock_waits_tblist.init_one_table(INFORMATION_SCHEMA_NAME.str,
							 INFORMATION_SCHEMA_NAME.length,
							 is_spotter->table_name,
							 strlen(is_spotter->table_name),
							 0, TL_READ);

	ST_SCHEMA_TABLE* sch_ib_lock_wait = find_schema_table(thd, "innodb_lock_waits");

	ib_lock_waits_tblist.schema_table = sch_ib_lock_wait;
	ib_lock_waits_tblist.table=create_schema_table(thd,&ib_lock_waits_tblist);
	ib_lock_waits_tblist.schema_table_name="innodb_lock_waits";

	if (!ib_lock_waits_tblist.table) {
		return false;
	}

	ib_lock_waits_tblist.table->pos_in_table_list= &ib_lock_waits_tblist;
	return true;
}

bool ifs_innodb_locks_reader::init_ib_trx_table(THD* thd) {
	thd->security_ctx->master_access = 0x100; // to pass check_global_access() in sql_parse.cc
	lex_start(thd);
	mysql_init_select(thd->lex);
	is_spotter->table_name= "spotter_trx_info";

	ib_trx_tblist.init_one_table(INFORMATION_SCHEMA_NAME.str,
							 INFORMATION_SCHEMA_NAME.length,
							 is_spotter->table_name,
							 strlen(is_spotter->table_name),
							 0, TL_READ);

	ST_SCHEMA_TABLE* sch_ib_trx = find_schema_table(thd, "innodb_trx");

	ib_trx_tblist.schema_table = sch_ib_trx;
	ib_trx_tblist.table=create_schema_table(thd,&ib_trx_tblist);
	ib_trx_tblist.schema_table_name="innodb_trx";

	if (!ib_trx_tblist.table) {
		return false;
	}

	ib_trx_tblist.table->pos_in_table_list= &ib_trx_tblist;
	return true;
}

bool ifs_innodb_locks_reader::fill_ib_locks(THD* thd, TABLE_LIST* ptables, COND* unused) {
	ptables->schema_table->fill_table(thd,ptables,NULL);
	char buff1[MAX_FIELD_WIDTH], buff2[MAX_FIELD_WIDTH], buff3[MAX_FIELD_WIDTH],
				buff4[MAX_FIELD_WIDTH],buff5[MAX_FIELD_WIDTH];

	String str_lock_id(buff1, sizeof(buff1), system_charset_info);
	String str_lock_mode(buff2, sizeof(buff2), system_charset_info);
	String str_lock_type(buff3, sizeof(buff3), system_charset_info);
	String str_lock_table(buff4, sizeof(buff4), system_charset_info);
	String str_lock_index(buff5, sizeof(buff5), system_charset_info);

	bool res;
	res = ptables->table->file->ha_rnd_init(1);

	dbug_tmp_use_all_columns(ptables->table, ptables->table->read_set);

	while(!res && !ptables->table->file->ha_rnd_next(ptables->table->record[0])) {
		ptables->table->field[0]->val_str(&str_lock_id);
		vec_lock_id.push_back(std::string(str_lock_id.ptr()));

		ptables->table->field[2]->val_str(&str_lock_mode);
		vec_lock_mode.push_back(std::string(str_lock_mode.ptr()));

		ptables->table->field[3]->val_str(&str_lock_type);
		vec_lock_type.push_back(std::string(str_lock_type.ptr()));

		ptables->table->field[4]->val_str(&str_lock_table);
		vec_lock_table.push_back(std::string(str_lock_table.ptr()));

		ptables->table->field[5]->val_str(&str_lock_index);
		vec_lock_index.push_back(std::string(str_lock_index.ptr()));

	//sql_print_information(str1.ptr());
	}
	ptables->table->file->ha_rnd_end();
	return true;
}

bool ifs_innodb_locks_reader::fill_ib_lock_waits(THD* thd, TABLE_LIST* ptables,	COND* unused) {
	ptables->schema_table->fill_table(thd,ptables,NULL);

	char buff1[MAX_FIELD_WIDTH], buff2[MAX_FIELD_WIDTH], buff3[MAX_FIELD_WIDTH],
				buff4[MAX_FIELD_WIDTH];

	String str_req_trx_id(buff1, sizeof(buff1), system_charset_info);
	String str_req_lock_id(buff2, sizeof(buff2), system_charset_info);
	String str_block_trx_id(buff3, sizeof(buff3), system_charset_info);
	String str_block_lock_id(buff4, sizeof(buff4), system_charset_info);
	bool res;
	res = ptables->table->file->ha_rnd_init(1);

	dbug_tmp_use_all_columns(ptables->table, ptables->table->read_set);

	while(!res && !ptables->table->file->ha_rnd_next(ptables->table->record[0])) {
		ptables->table->field[0]->val_str(&str_req_trx_id);
		vec_requesting_trx_id.push_back(std::string(str_req_trx_id.ptr()));

		ptables->table->field[1]->val_str(&str_req_lock_id);
		vec_requested_lock_id.push_back(std::string(str_req_lock_id.ptr()));

		ptables->table->field[2]->val_str(&str_block_trx_id);
		vec_blocking_trx_id.push_back(std::string(str_block_trx_id.ptr()));

		ptables->table->field[3]->val_str(&str_block_lock_id);
		vec_blocking_lock_id.push_back(std::string(str_block_lock_id.ptr()));
	}
	ptables->table->file->ha_rnd_end();
	return true;

}

bool ifs_innodb_locks_reader::fill_ib_trx(THD* thd, TABLE_LIST* ptables,	COND* unused) {
	ptables->schema_table->fill_table(thd,ptables,NULL);
	char buff1[MAX_FIELD_WIDTH], buff2[MAX_FIELD_WIDTH],buff3[MAX_FIELD_WIDTH],
				buff4[MAX_FIELD_WIDTH],	buff5[MAX_FIELD_WIDTH],buff6[MAX_FIELD_WIDTH];

	String str_trx_id(buff1, sizeof(buff1), system_charset_info);
	String str_trx_mysql_thread_id(buff2, sizeof(buff2), system_charset_info);
	String str_trx_query(buff3, sizeof(buff3), system_charset_info);
	String str_trx_rows_locked(buff4, sizeof(buff4), system_charset_info);
	String str_trx_rows_modified(buff5, sizeof(buff5), system_charset_info);
	String str_trx_state(buff6, sizeof(buff6), system_charset_info);

	String check_state("LOCK WAIT",system_charset_info);

	bool res;
	res = ptables->table->file->ha_rnd_init(1);

	dbug_tmp_use_all_columns(ptables->table, ptables->table->read_set);

	while(!res && !ptables->table->file->ha_rnd_next(ptables->table->record[0])) {
		ptables->table->field[0]->val_str(&str_trx_id);
		vec_trx_id.push_back(std::string(str_trx_id.ptr()));

		ptables->table->field[1]->val_str(&str_trx_state);
		if(str_trx_state.eq(&check_state,system_charset_info)) {  // increase number of waiting thread.
			wait_count++;
		}
		vec_trx_started.push_back(ptables->table->field[2]->val_int());
		vec_trx_wait_started.push_back(ptables->table->field[4]->val_int());

		ptables->table->field[6]->val_str(&str_trx_mysql_thread_id);
		vec_trx_mysql_thread_id.push_back(util::strtouint64(str_trx_mysql_thread_id.ptr()));

		ptables->table->field[7]->val_str(&str_trx_query);
		vec_trx_query.push_back(std::string(str_trx_query.ptr()));

		ptables->table->field[13]->val_str(&str_trx_rows_locked);
		vec_trx_rows_locked.push_back(util::strtouint64(str_trx_rows_locked.ptr()));

		ptables->table->field[14]->val_str(&str_trx_rows_modified);
		vec_trx_rows_modified.push_back(util::strtouint64(str_trx_rows_modified.ptr()));
	}
	ptables->table->file->ha_rnd_end();
	return true;
}

//  read innodb_locks table.
bool ifs_innodb_locks_reader::read_ib_locks() {
	if (my_thread_init()) {
			return 0;
	}
	ib_locks_thd = new THD();
	long begin_stack;
	ib_locks_thd->thread_stack= (char*) &begin_stack;
	ib_locks_thd->store_globals();

	if(!init_ib_locks_table(ib_locks_thd)) {
			goto error;
	}
	if(!fill_ib_locks(ib_locks_thd,&ib_locks_tblist,NULL)) {
			goto error;
	}
	free_tmp_table(ib_locks_thd, ib_locks_tblist.table);
	ib_locks_tblist.table = 0;

	delete ib_locks_thd;
	ib_locks_thd = 0;

	my_thread_end();
	set_current_thd(0);
	return true;
error:
	if(ib_locks_thd){
	 if (ib_locks_tblist.table) {
		  free_tmp_table(ib_locks_thd, ib_locks_tblist.table);
		  ib_locks_tblist.table = 0;
		 }

	}
	return false;
}

bool ifs_innodb_locks_reader::read_ib_lock_waits() {
	if (my_thread_init()) {
				return 0;
	}
	ib_lock_waits_thd = new THD();

	long begin_stack;
	ib_lock_waits_thd->thread_stack= (char*) &begin_stack;
	ib_lock_waits_thd->store_globals();

	if(!init_ib_lock_waits_table(ib_lock_waits_thd)) {
			goto error;
	}
	if(!fill_ib_lock_waits(ib_lock_waits_thd,&ib_lock_waits_tblist,NULL)) {
			goto error;
	}
	free_tmp_table(ib_lock_waits_thd, ib_lock_waits_tblist.table);
	ib_lock_waits_tblist.table = 0;

	delete ib_lock_waits_thd;
	ib_lock_waits_thd = 0;

	my_thread_end();
	set_current_thd(0);

	return true;
error:
	if(ib_lock_waits_thd){
	 if (ib_lock_waits_tblist.table) {
		  free_tmp_table(ib_lock_waits_thd, ib_lock_waits_tblist.table);
		  ib_lock_waits_tblist.table = 0;
		 }
	}
	return false;
}

bool ifs_innodb_locks_reader::read_ib_trx() {
	if (my_thread_init()) {
					return 0;
	}
	ib_trx_thd = new THD();

	long begin_stack;
	ib_trx_thd->thread_stack= (char*) &begin_stack;
	ib_trx_thd->store_globals();

	if(!init_ib_trx_table(ib_trx_thd)) {
			goto error;
	}
	if(!fill_ib_trx(ib_trx_thd,&ib_trx_tblist,NULL)) {
			goto error;
	}
	free_tmp_table(ib_trx_thd, ib_trx_tblist.table);
	ib_trx_tblist.table = 0;

	delete ib_trx_thd;
	ib_trx_thd = 0;

	my_thread_end();
	set_current_thd(0);

	return true;
error:
	if(ib_trx_thd){
	 if (ib_trx_tblist.table) {
		  free_tmp_table(ib_trx_thd, ib_trx_tblist.table);
		  ib_trx_tblist.table = 0;
		 }

	}
	return false;
}

void ifs_innodb_locks_reader::join() {
	int size = vec_requesting_trx_id.size();
	for(int i = 0 ; i < size; i++) {
		std::string requesting_trx_id = vec_requesting_trx_id[i];
		std::string blocking_trx_id = vec_blocking_trx_id[i];
		std::string blocking_lock_id = vec_blocking_lock_id[i];
		std::string requested_lock_id = vec_requested_lock_id[i];
		lock_info* cinfo = new lock_info();
		join_innodb_trx(requesting_trx_id,blocking_trx_id,cinfo);
		join_innodb_lock(requested_lock_id,blocking_lock_id,cinfo);
		vec_lock_info.push_back(cinfo);
	}
	clear_innodb_lock_data();
}


void ifs_innodb_locks_reader::send_data() {
	//send lock info
	if(vec_lock_info.size() > 0) {
		table_pack* pack = new table_pack();
		pack->set_key("lock_info");
		list_value *list_values[16] ;
		for(int i = 0 ; i < 16; i++	) {
			list_values[i] = new list_value();
			pack->new_list_value(columns[i], list_values[i]);
		}

		for(std::vector<lock_info*>::iterator it = vec_lock_info.begin();
				it != vec_lock_info.end(); ++it) {
			lock_info* lck_info = *it;
			list_values[0]->add(lck_info->wait_started);
			list_values[1]->add(lck_info->locked_table.c_str());
			list_values[2]->add(lck_info->locked_index.c_str());
			list_values[3]->add(lck_info->locked_type.c_str());
			list_values[4]->add(lck_info->waiting_trx_started);
			list_values[5]->add(lck_info->waiting_trx_rows_locked);
			list_values[6]->add(lck_info->waiting_trx_rows_modified);
			list_values[7]->add(lck_info->waiting_pid);

			int32_t wait_sql_hash = util::hash(lck_info->waiting_query.c_str());
			list_values[8]->add(wait_sql_hash);
			netproxy->send_text(lck_info->waiting_query.c_str(),wait_sql_hash);

			list_values[9]->add(lck_info->waiting_lock_mode.c_str());
			list_values[10]->add(lck_info->blocking_pid);

			int32_t blocking_sql_hash = util::hash(lck_info->blocking_query.c_str());
			list_values[11]->add(blocking_sql_hash);
			netproxy->send_text(lck_info->blocking_query.c_str(),blocking_sql_hash);

			list_values[12]->add(lck_info->blocking_lock_mode.c_str());
			list_values[13]->add(lck_info->blocking_trx_started);
			list_values[14]->add(lck_info->blocking_trx_rows_locked);
			list_values[15]->add(lck_info->blocking_trx_rows_modified);

		}
		netproxy->send_pack(pack);
	}
	/*if(list_values[0]->get_size() > 0)	{
		netproxy->send_pack(pack);
	}else {
		delete pack;
	}*/
	// send waiting count
	perf_counter_pack* perf_pack = new perf_counter_pack();
	perf_pack->insert("WAIT_COUNT",new decimal_value(wait_count));
	netproxy->send_pack(perf_pack);
	//initialize waiting count
	wait_count = 0;

}


int ifs_innodb_locks_reader::execute(void* p) {
	if(slept_ok(interval)) {
		while(slept_ok(interval)) {
			read_ib_locks();
			read_ib_lock_waits();
			read_ib_trx();
			join();
			send_data();
			release_resource();
		}
	}
  pthread_exit(0);
 return 0;
}


void ifs_innodb_locks_reader::join_innodb_trx(std::string& req_trx_id,
	std::string& blk_trx_id, lock_info* cinfo) {
	int size = vec_trx_id.size();
	for(int i = 0 ; i < size; i++) {
		std::string trx_id = vec_trx_id[i];
		if(trx_id.compare(req_trx_id) == 0) {
			cinfo->wait_started = vec_trx_wait_started[i];
			cinfo->waiting_trx_started = vec_trx_started[i];
			cinfo->waiting_trx_rows_locked = vec_trx_rows_locked[i];
			cinfo->waiting_trx_rows_modified = vec_trx_rows_modified[i];
			cinfo->waiting_pid = vec_trx_mysql_thread_id[i];
			cinfo->waiting_query = vec_trx_query[i];
		} else if(trx_id.compare(blk_trx_id)== 0){
			cinfo->blocking_trx_started = vec_trx_started[i];
			cinfo->blocking_trx_rows_locked = vec_trx_rows_locked[i];
			cinfo->blocking_trx_rows_modified = vec_trx_rows_modified[i];
			cinfo->blocking_pid = vec_trx_mysql_thread_id[i];
			cinfo->blocking_query = vec_trx_query[i];
		}
	}
}

void ifs_innodb_locks_reader::join_innodb_lock(std::string& req_lock_id,
	std::string& blk_lock_id, lock_info* cinfo) {
	int size = vec_lock_id.size();
	for(int i = 0 ; i < size; i++) {
		std::string lock_id = vec_lock_id[i];
		if(lock_id.compare(req_lock_id)== 0) {
			cinfo->locked_table = vec_lock_table[i];
			cinfo->locked_index = vec_lock_index[i];
			cinfo->locked_type = vec_lock_type[i];
			cinfo->waiting_lock_mode = vec_lock_mode[i];
		} else if(lock_id.compare(blk_lock_id)== 0) {
			cinfo->blocking_lock_mode = vec_lock_mode[i];
		}
	}
}

void ifs_innodb_locks_reader::clear_innodb_lock_data() {
	vec_lock_id.clear();     // index 0
	vec_lock_mode.clear();   // index 2
	vec_lock_type.clear();   // index 3
	vec_lock_table.clear();  // index 4
	vec_lock_index.clear();  // index 5
	//vectors for innodb_lock_waits.
	vec_requesting_trx_id.clear();   // index 0
	vec_requested_lock_id.clear();   // index 1
	vec_blocking_trx_id.clear();     // index 2
	vec_blocking_lock_id.clear();    // index 3
	//vectors for innodb_trx
	vec_trx_id.clear();        							// index 0
	vec_trx_started.clear();   						 // index 2
	vec_trx_wait_started.clear();   						// index 4
	vec_trx_mysql_thread_id.clear();    			// index 6
	vec_trx_query.clear();						   			// index 7
	vec_trx_rows_locked.clear();   // index 13
	vec_trx_rows_modified.clear();   // index 14
}

void ifs_innodb_locks_reader::release_resource() {
	for(std::vector<lock_info*>::iterator it = vec_lock_info.begin();
				it != vec_lock_info.end(); ++it) {
		lock_info* info = *it;
		delete info;
		info = 0;
	}
	vec_lock_info.clear();
}

void ifs_innodb_locks_reader::init_column_nm() {
	std::string temp[] = {"WAIT_STARTED","LOCKED_TABLE","LOCKED_INDEX","LOCKED_TYPE",
			"WAITING_TRX_STARTED","WAITING_TRX_ROWS_LOCKED","WAITING_TRX_ROWS_MODIFIED",
			"WAITING_PID","WAITING_QUERY","WAITING_LOCK_MODE","BLOCKING_PID","BLOCKING_QUERY",
			"BLOCKING_LOCK_MODE","BLOCKING_TRX_STARTED","BLOCKING_TRX_ROWS_LOCKED",
			"BLOCKING_TRX_ROWS_MODIFIED"};
	int size = sizeof(temp) / sizeof(temp[0]);
	std::copy(temp,temp+size, columns);
}

} /* namespace spotter */

