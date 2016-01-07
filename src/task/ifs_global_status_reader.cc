/*
 * ifs_global_status_reader2.cc
 *
 *  Created on: 2015. 5. 26.
 *      Author: windfree
 */

#include "task/ifs_global_status_reader.h"
#include "sql_parse.h"
#include "util/log.h"
#include "util/util.h"
#include "value/decimal_value.h"
#include "data_pack/perf_counter_pack.h"
#include "sql_class.h"
#include "table.h"
#include "net/net_proxy.h"

int fill_status(THD *thd, TABLE_LIST *tables, COND *cond);
extern ST_SCHEMA_TABLE schema_tables[];

namespace spotter {

ifs_global_status_reader::ifs_global_status_reader(void* p,std::string name) {
	tbl_name = name.c_str();
	interval = 1; // default value
	plugin_data = p;
	is_spotter = (ST_SCHEMA_TABLE*)p;
	first_read = true;
	last_read_time = 0;
	init_filter();
	init_fields();
	diff_sec = interval;

}


ifs_global_status_reader::~ifs_global_status_reader() {
	for(std::map<std::string,uint64_t*>::iterator it= value_map.begin();it != value_map.end();++it) {
			delete[] it->second;
		}
	delete[] spotter_fields;
	spotter_fields = 0;
	delete[] filter;
	filter = 0;
}

void ifs_global_status_reader::init_fields() {
	spotter_fields = new ST_FIELD_INFO[3];
	spotter_fields[0] = {"VARIABLE_NAME",   255, MYSQL_TYPE_STRING, 0, 0, 0, 0};
	spotter_fields[1] =  {"VARIABLE_VALUE", 1024, MYSQL_TYPE_STRING, 0, 0, 0, 0};
	spotter_fields[2] = {0, 0, MYSQL_TYPE_NULL, 0, 0, 0, 0};
	is_spotter->fields_info=  spotter_fields; ///< field descriptor
	is_spotter->idx_field1 = 0;
}


bool ifs_global_status_reader::init_table(THD* thd) {
	lex_start(thd);
	mysql_init_select(thd->lex);
	//is_spotter->table_name= tbl_name;
	is_spotter->table_name= "spotter_status";
	tables.init_one_table(INFORMATION_SCHEMA_NAME.str,
						 INFORMATION_SCHEMA_NAME.length,
						 is_spotter->table_name,
						 strlen(is_spotter->table_name),
						 0, TL_READ);

	tables.schema_table= is_spotter;
	tables.table= is_spotter->create_table(thd, &tables);
	//tables.table= create_schema_table(thd, &tables); after 10.1.X
	if (!tables.table) {
		return false;
	}

	tables.table->pos_in_table_list= &tables;

	return true;
}


bool ifs_global_status_reader::fill(THD* thd, TABLE_LIST* ptables,COND* unused) {
  ptables->schema_table= schema_tables + SCH_GLOBAL_STATUS;
  fill_status(thd, ptables, NULL);

  /*COND *cond;
  ptables->schema_table= schema_tables + SCH_GLOBAL_STATUS;
  cond= make_cond(thd, tables, filter);
  fill_status(thd, ptables, cond);*/

	char buff1[MAX_FIELD_WIDTH], buff2[MAX_FIELD_WIDTH];
	String str1(buff1, sizeof(buff1), system_charset_info);
	String str2(buff2, sizeof(buff2), system_charset_info);

	bool res;
	res = ptables->table->file->ha_rnd_init(1);

	dbug_tmp_use_all_columns(ptables->table, ptables->table->read_set);

	while(!res && !ptables->table->file->ha_rnd_next(ptables->table->record[0])) {
		ptables->table->field[0]->val_str(&str1);
		ptables->table->field[1]->val_str(&str2);
		//const char* name = str1.ptr();
		std::string name(str1.ptr());
		if(check(name.c_str())) {
			uint64_t status_value = util::strtouint64(str2.ptr());
			if(!first_read) {
				value_map[name][0] = value_map[name][1];
				value_map[name][1] = status_value;
			} else {
				uint64_t* val = new uint64_t[2];
				val[0] = 0;
				val[1] = status_value;
				value_map.insert(std::pair<std::string ,uint64_t*>(name,val));
			}
		}

    }

	ptables->table->file->ha_rnd_end();
	first_read = false;
  return true;
}

void ifs_global_status_reader::send_data() {
	/* send data */
	if(!first_read) {
		perf_counter_pack* pack = new perf_counter_pack();
		calc(value_map,pack);
		netproxy->send_pack(pack);
		//delete pack;
	}
}

bool ifs_global_status_reader::read() {
	if(!init_table(thd)) {
		goto error;
	}

	if(!fill(thd,&tables,NULL)) {
			goto error;
	}
	free_tmp_table(thd, tables.table);
	tables.table = 0;
	if(last_read_time != 0) {
		int32_t current_time = get_seconds();
		diff_sec = current_time - last_read_time;
		last_read_time = current_time;
	} else {
		last_read_time = get_seconds();
	}
	last_read_time = get_seconds();

	send_data();
	return true;
error:
	if(thd){
	 if (tables.table) {
		  free_tmp_table(thd, tables.table);
		  tables.table = 0;
		 }

	}
	return false;
}


void ifs_global_status_reader::init_filter() {
	filter = new LEX_STRING[1];
	/*filter =  {
				  {C_STRING_WITH_LEN("THREADS_RUNNING")},
				  {C_STRING_WITH_LEN("THREADS_CONNECTED")},
				  {C_STRING_WITH_LEN("COM_CALL_PROCEDURE")},
				  {C_STRING_WITH_LEN("COM_SELECT")},
				  {C_STRING_WITH_LEN("COM_INSERT")},
				  {C_STRING_WITH_LEN("COM_UPDATE")},
				  {C_STRING_WITH_LEN("COM_DELETE")},
				  {C_STRING_WITH_LEN("INNODB_BUFFER_POOL_READS")},
				  {C_STRING_WITH_LEN("INNODB_BUFFER_POOL_READ_REQUESTS")},
				  {C_STRING_WITH_LEN("KEY_READS")},
				  {C_STRING_WITH_LEN("KEY_READ_REQUESTS")},
				  {C_STRING_WITH_LEN("QCACHE_HITS")},
				  {C_STRING_WITH_LEN("QCACHE_INSERTS")},
				  {C_STRING_WITH_LEN("CONNECTIONS")},
				  {C_STRING_WITH_LEN("QUESTIONS")},
				  {C_STRING_WITH_LEN("UPTIME")},
				  {C_STRING_WITH_LEN("COM_COMMIT")},
				  {C_STRING_WITH_LEN("COM_ROLLBACK")},
				  {C_STRING_WITH_LEN("SLOW_QUERIES")},
				  {C_STRING_WITH_LEN("SELECT_FULL_JOIN")},
				  {C_STRING_WITH_LEN("OPENED_TABLE_DEFINITIONS")},
				  {C_STRING_WITH_LEN("OPENED_TABLES")},
				  {C_STRING_WITH_LEN("CREATED_TMP_DISK_TABLES")},
				  {C_STRING_WITH_LEN("CREATED_TMP_TABLES")},
				  {C_STRING_WITH_LEN("INNODB_ROW_LOCK_WAITS")},
				  {C_STRING_WITH_LEN("TABLE_LOCKS_IMMEDIATE")},
				  {C_STRING_WITH_LEN("TABLE_LOCKS_WAITED")},
				  {C_STRING_WITH_LEN("UPTIME_SINCE_FLUSH_STATUS")},
				     {0, 0}
		};*/

	filter[0] = {0, 0};

}


bool ifs_global_status_reader::check(const char* name) {
	if(strcmp(name,"THREADS_RUNNING") == 0 || strcmp(name,"THREADS_CONNECTED") == 0  || strcmp(name,"COM_CALL_PROCEDURE") == 0 ||
			strcmp(name,"COM_SELECT") == 0 || strcmp(name,"COM_INSERT") == 0 || strcmp(name,"COM_UPDATE") == 0 ||
			strcmp(name,"COM_DELETE") == 0 || strcmp(name,"INNODB_BUFFER_POOL_READS") == 0  || strcmp(name,"INNODB_BUFFER_POOL_READ_REQUESTS") == 0 ||
			strcmp(name,"KEY_READS") == 0 || strcmp(name,"KEY_READ_REQUESTS") == 0	 || strcmp(name,"QCACHE_HITS") == 0 || strcmp(name ,"QCACHE_INSERTS") == 0 ||
			strcmp(name,"THREADS_CREATED") == 0 || strcmp(name,"CONNECTIONS") == 0 || strcmp(name,"QUESTIONS") == 0 ||
			strcmp(name,"UPTIME") == 0 || strcmp(name,"COM_COMMIT") == 0 || strcmp(name,"COM_ROLLBACK") ==0 ||
			strcmp(name,"SLOW_QUERIES") == 0 || strcmp(name,"SELECT_FULL_JOIN") == 0 || strcmp(name,"OPENED_TABLE_DEFINITIONS") == 0 ||
			strcmp(name,"OPENED_TABLES") == 0 || strcmp(name,"CREATED_TMP_DISK_TABLES") ==0 || strcmp(name,"CREATED_TMP_TABLES") == 0 ||
			strcmp(name ,"INNODB_ROW_LOCK_WAITS") ==0 || strcmp(name ,"TABLE_LOCKS_IMMEDIATE") == 0 || strcmp(name,"TABLE_LOCKS_WAITED") == 0) {
		return true;
	} else
		return false;
}
void ifs_global_status_reader::calc(std::map<std::string, uint64_t*>& map,perf_counter_pack* pack) {
	/* connections-active */
		pack->insert("THREADS_RUNNING",new decimal_value(map["THREADS_RUNNING"][1])	);
		/* connections-total */
		pack->insert("THREADS_TOTAL",new decimal_value(map["THREADS_CONNECTED"][1]));
		/* delta : call count */
		pack->insert("COM_CALL_PROCEDURE",new decimal_value(calc_delta("COM_CALL_PROCEDURE",map)));
		/* delta: select count */
		pack->insert("COM_SELECT",new decimal_value(calc_delta("COM_SELECT",map)));
		/* delta: insert count */
		pack->insert("COM_INSERT",new decimal_value(calc_delta("COM_INSERT",map)));
		/* delta: delete count */
		pack->insert("COM_DELETE",new decimal_value(calc_delta("COM_DELETE",map)));
		/* delta: update count */
		pack->insert("COM_UPDATE",new decimal_value(calc_delta("COM_UPDATE",map)));
		/* delta: QPS */
		pack->insert("QPS",new decimal_value(calc_delta("QUESTIONS",map)));
		/* delta: TPS */
		pack->insert("TPS",new decimal_value(calc_delta("COM_COMMIT",map)+ calc_delta("COM_ROLLBACK",map)));
		/* delta: SLOW QUERIES */
		pack->insert("SLOW_QUERY",new decimal_value(calc_delta("SLOW_QUERIES",map)));
		/* delta: Table-full-scan */
		pack->insert("FULL_SCAN",new decimal_value(calc_delta("SELECT_FULL_JOIN",map)));
		/* delta: Opened-table-definition */
		pack->insert("OT_DEF",new decimal_value(calc_delta("OPENED_TABLE_DEFINITIONS",map)));
		/* delta: Opened table */
		pack->insert("OT_COUNT",new decimal_value(calc_delta("OPENED_TABLES",map)));
		/* delta: Disk temp table */
		pack->insert("DTEMP_TBL",new decimal_value(calc_delta("CREATED_TMP_DISK_TABLES",map)));
		/* delta: Memory temp table */
		//pack->insert("Memory-temp-table",new decimal_value(calc_delta("CREATED_TMP_TABLES",map)))
		uint64_t delta_tmp_tbl = calc_delta("CREATED_TMP_TABLES",map);
		uint64_t delta_disk_tmp_tbl = calc_delta("CREATED_TMP_DISK_TABLES",map);
		pack->insert("MTEMP_TBL",new decimal_value(delta_tmp_tbl - delta_disk_tmp_tbl));
		//pack->insert("MTEMP_TBL",new decimal_value(map["CREATED_TMP_TABLES"][1]-map["CREATED_TMP_DISK_TABLES"][1]));
		/* delta: Innodb row lock waits */
		pack->insert("INNO_RLOCK_W",new decimal_value(calc_delta("INNODB_ROW_LOCK_WAITS",map)));
		/* delta: Table lock-immediate */
		pack->insert("TBL_LOCK",new decimal_value(calc_delta("TABLE_LOCKS_IMMEDIATE",map)));
		/* delta: Table lock-waited */
		pack->insert("TBL_LOCK_W",new decimal_value(calc_delta("TABLE_LOCKS_WAITED",map)));
	 /* Hit Ratio : Innodb Buffer */
		uint64_t ratio = (100 - ((map["INNODB_BUFFER_POOL_READS"][1] / (map["INNODB_BUFFER_POOL_READ_REQUESTS"][1] + 1)) * 100));
		pack->insert("INNO_BUF_HIT",new decimal_value(ratio));
		/* Hit Ratio : Key cache */
		if(map["KEY_READ_REQUESTS"][1] >= map["KEY_READS"][1]  ) {
			ratio = (100 - ((map["KEY_READS"][1] / (map["KEY_READ_REQUESTS"][1] + 1)) * 100));
		} else {
			ratio = 0;
		}
		pack->insert("KEY_RATIO",new decimal_value(ratio));
		/* Hit Ratio : Query Cache */
		ratio = (map["QCACHE_HITS"][1] / (map["QCACHE_HITS"][1] + map["QCACHE_INSERTS"][1] + 1)) * 100;
		pack->insert("QCACHE_RATIO",new decimal_value(ratio));
		/* Hit Ratio : Thread Cache */
		ratio = 100 - ((map["THREADS_CREATED"][1] / (map["CONNECTIONS"][1] + 1)) * 100);
		pack->insert("TCACHE_RATIO",new decimal_value(ratio));
}


uint64_t ifs_global_status_reader::calc_delta(std::string key,	std::map<std::string, uint64_t*>& map) {
	uint64_t old_val;
	uint64_t new_val;
	old_val = map[key][0];
	new_val = map[key][1];

	if(old_val > new_val) {
			old_val = new_val;
	}

	return (new_val - old_val) / diff_sec;
}

int32_t ifs_global_status_reader::get_seconds() {
	time_t seconds;
	seconds = time(0);
	return seconds;
}


int ifs_global_status_reader::execute(void* p) {

	if(slept_ok(interval)) {
		while(slept_ok(interval)) {
			if (my_thread_init()) {
								return 0;
			}
			thd = new THD();
			long begin_stack;
			thd->thread_stack= (char*) &begin_stack;
			thd->store_globals();
			read();
			delete thd;
			thd = 0;
			set_current_thd(0);
			my_thread_end();
		}

	}


 pthread_exit(0);
 return 0;
}

} /* namespace spotter */


