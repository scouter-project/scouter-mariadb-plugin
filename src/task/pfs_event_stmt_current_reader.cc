/* Copyright (c)2015, LG CNS

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
   */

/*
 * monm_Read_pfs_stmt_current.cc
  *
 *  This class implements a reader which get data
 *  from pfs_event_stmt_current_reader table and send that data to Server.
  */
#include "my_global.h"
#include "task/pfs_event_stmt_current_reader.h"
#include "data_pack/table_pack.h"
#include "io/data_output.h"
#include "util/util.h"
#include "value/list_value.h"
#include "net/net_proxy.h"
#include "util/log.h"
#include <records.h>
#include <handler.h>
using namespace std;
namespace spotter {

pfs_event_stmt_current_reader::pfs_event_stmt_current_reader()
{
	dbname = "performance_schema";
	tblname = "events_statements_current";
	interval = 1;
	init_column_nm();


}

pfs_event_stmt_current_reader::~pfs_event_stmt_current_reader()
{
	/*if(thd) {
			delete thd;
			thd = 0;
	}*/
}

/* Read data from pfs_event_stmt_current_reader table using low level interface
 * and send data to APM Server.
 */
bool pfs_event_stmt_current_reader::read(){
	READ_RECORD read_record_info;
	tables.init_one_table(STRING_WITH_LEN(dbname), STRING_WITH_LEN(tblname),
													tblname, TL_READ);
	 DBUG_ENTER("spotter-enter-open_and_lock_tables-stmt-current");
	bool result = open_and_lock_tables(thd, &tables, FALSE, MYSQL_LOCK_IGNORE_TIMEOUT);
	 DBUG_PRINT("end",("spotter-leave-open_and_lock_tables-stmt-current"));
	if (result) {
		 sql_print_error("spotter plugin error: Can't open the %s.%s table.",dbname,tblname);
		 //free_root(&mem_field,MYF(0));
		 DBUG_RETURN(false);

	 }

	if(tables.table) { // if only table exists
		table = tables.table;
		int error = 0;
		init_sql_alloc(&mem_field, 1024, 0, MYF(0));
		if (init_read_record(&read_record_info, thd, table, NULL,1,0,FALSE)) {
			sql_print_error("Could not initialize init_read_record of events_statements_current");
			return false;
		}
		table->use_all_columns();

		table_pack* pack = new table_pack();
		pack->set_key("events_statements_current");

		list_value *list_values[PFS_EVT_STMT_CURRENT_COL_CNT] ;
		for(int i = 0 ; i < PFS_EVT_STMT_CURRENT_COL_CNT; i++	) {
			list_values[i] = new list_value();
			pack->new_list_value(columns[i], list_values[i]);
		}

		while (!(error= read_record_info.read_record(&read_record_info))) {
			/*thread_id*/
			list_values[0]->add(util::strtouint64(get_field(&mem_field, table->field[0])));
			/*event_id*/
			list_values[1]->add(util::strtouint64(get_field(&mem_field, table->field[1])));
			/*end_event_id*/
			list_values[2]->add(util::strtouint64(get_field(&mem_field, table->field[2])));
			/*event_name*/
			char* evt_name = get_field(&mem_field, table->field[3]);
			int32_t evt_name_hash = util::hash(evt_name);
			list_values[3]->add(evt_name_hash);
			netproxy->send_text(evt_name,evt_name_hash);
			/*source*/
			char* source = get_field(&mem_field, table->field[4]);
			int32_t source_hash = util::hash(source);
			list_values[4]->add(source_hash);
			netproxy->send_text(source,source_hash);
		 /*timer_start*/
			list_values[5]->add(util::strtouint64(get_field(&mem_field, table->field[5])));
			/*timer_end*/
			list_values[6]->add(util::strtouint64(get_field(&mem_field, table->field[6])));
			/*timer_wait*/
			list_values[7]->add(util::strtouint64(get_field(&mem_field, table->field[7])));
			/*lock_time*/
			list_values[8]->add(util::strtouint64(get_field(&mem_field, table->field[8])));
			/*sql_text*/
			char* sql = get_field(&mem_field, table->field[9]);
			int32_t sql_hash = util::hash(sql);
			list_values[9]->add(sql_hash);
			netproxy->send_text(sql,sql_hash);
			/*digest*/
			char* digest = get_field(&mem_field, table->field[10]);
			int32_t digest_hash = util::hash(digest);
			list_values[10]->add(digest_hash);
			netproxy->send_text(digest,digest_hash);
			/*digest_text*/
			char* digest_text = get_field(&mem_field, table->field[11]);
			int32_t digest_text_hash = util::hash(digest_text);
			list_values[11]->add(digest_text_hash);
			netproxy->send_text(digest_text,digest_text_hash);
			/*nesting_event_id*/
			list_values[12]->add(util::strtouint64(get_field(&mem_field, table->field[38])));
			/*nesting_event_type*/
			list_values[13]->add(util::strtouint64(get_field(&mem_field, table->field[39])));

			//for test only logging
		}
		if(list_values[0]->get_size() > 0)
		{
			 netproxy->send_pack(pack);
		}else {
			 delete pack;
	    }
		free_root(&mem_field,MYF(0));
		end_read_record(&read_record_info);
	  //table->m_needs_reopen= TRUE;
		table->file->inited = handler::NONE;
	  //close_thread_tables(thd);
	  close_mysql_tables(thd);// Force close to free memory
	}
	DBUG_RETURN(true);
	//return true;
}


void pfs_event_stmt_current_reader::init_column_nm() {
	std::string temp[] = {"THREAD_ID","EVENT_ID","END_EVENT_ID",
			"EVENT_NAME","SOURCE","TIMER_START",
			"TIMER_END","TIMER_WAIT","LOCK_TIME",
			"SQL_TEXT","DIGEST","DIGEST_TEXT","NESTING_EVENT_ID","NESTING_EVENT_TYPE"};
	int size = sizeof(temp) / sizeof(temp[0]);
	std::copy(temp,temp + size,columns );
}

pfs_event_stmt_current_reader::pfs_event_stmt_current_reader(
		const pfs_event_stmt_current_reader& clazz) {
}

pfs_event_stmt_current_reader& pfs_event_stmt_current_reader::operator =(
		const pfs_event_stmt_current_reader& clazz) {
}
}
