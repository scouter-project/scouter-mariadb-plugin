/*
 * pfs_event_stmt_summary_reader.cc
 *
 *  Created on: 2015. 5. 19.
 *      Author: windfree
 */
#include "my_global.h"
#include "task/pfs_event_stmt_summary_reader.h"

#include "data_pack/table_pack.h"
#include "value/list_value.h"
#include "util/util.h"
#include "util/log.h"
#include <records.h>
#include <handler.h>
#include "net/net_proxy.h"
namespace spotter {

pfs_event_stmt_summary_reader::pfs_event_stmt_summary_reader() : pfs_reader() {
	dbname = "performance_schema";
	tblname = "events_statements_summary_by_digest";
	interval = 1;
	init_column_nm();
	init_sql_alloc(&mem_field, 1024, 0, MYF(0));
}

pfs_event_stmt_summary_reader::~pfs_event_stmt_summary_reader() {

 free_root(&mem_field,MYF(0));
}

bool pfs_event_stmt_summary_reader::read() {
	//sql_print_information("spotter-start data read...table name:%s",tblname);
	READ_RECORD read_record_info;
	tables.init_one_table(STRING_WITH_LEN(dbname), STRING_WITH_LEN(tblname),
													tblname, TL_READ);
	 DBUG_ENTER("spotter-enter-open_and_lock_tables-stmt-digest");
	bool result = open_and_lock_tables(thd, &tables, FALSE, MYSQL_LOCK_IGNORE_TIMEOUT);
	 DBUG_PRINT("end",("spotter-leave-open_and_lock_tables-stmt-diges"));
	if (result)
	 {
		 sql_print_error("spotter plugin error: Can't open the %s.%s table.",dbname,tblname);
		 free_root(&mem_field,MYF(0));
		 DBUG_RETURN(false);

	 }

	if(table = tables.table) { /* if only table exists */
		int error = 0;
		if (init_read_record(&read_record_info, thd, table, NULL,1,0,FALSE))
		{
			sql_print_error("Could not initialize init_read_record of events_statements_summary_by_digest");
			return false;
		}
		table->use_all_columns();

		table_pack* pack = new table_pack();
		pack->set_key("events_statements_summary_by_digest");

		list_value *list_values[PFS_EVT_STMT_SUMMARY_COL_CNT] ;
		for(int i = 0 ; i < PFS_EVT_STMT_SUMMARY_COL_CNT; i++	) {
			list_values[i] = new list_value();
			pack->new_list_value(columns[i], list_values[i]);
		}

		while (!(error= read_record_info.read_record(&read_record_info))) {
			/*schema_name*/
			char* schema_name = get_field(&mem_field, table->field[0]);
			int32_t hash = util::hash(schema_name);
			list_values[0]->add(hash);
			netproxy->send_text(schema_name,hash);

			/*digest*/
			char* digest = get_field(&mem_field, table->field[1]);
			hash = util::hash(digest);
			list_values[1]->add(hash);
			netproxy->send_text(digest,hash);

			/*digest_text*/
			char* digest_text = get_field(&mem_field, table->field[2]);
			int32_t digest_text_hash = util::hash(digest_text);
			list_values[2]->add(digest_text_hash);
			netproxy->send_text(digest_text,digest_text_hash);

			/* COUNT_STAR   */
			list_values[3]->add(util::strtouint64(get_field(&mem_field, table->field[3])));
			/* SUM_TIMER_WAIT*/
			list_values[4]->add(util::strtouint64(get_field(&mem_field, table->field[4])));
			/*  MIN_TIMER_WAIT  */
			list_values[5]->add(util::strtouint64(get_field(&mem_field, table->field[5])));
			/* AVG_TIMER_WAIT    */
			list_values[6]->add(util::strtouint64(get_field(&mem_field, table->field[6])));
			/* MAX_TIMER_WAIT   */
			list_values[7]->add(util::strtouint64(get_field(&mem_field, table->field[7])));
			/* SUM_LOCK_TIME    */
			list_values[8]->add(util::strtouint64(get_field(&mem_field, table->field[8])));
			/* SUM_ERRORS   */
			list_values[9]->add(util::strtouint64(get_field(&mem_field, table->field[9])));
			/* SUM_WARNINGS   */
			list_values[10]->add(util::strtouint64(get_field(&mem_field, table->field[10])));
			/* SUM_ROWS_AFFECTED    */
			list_values[11]->add(util::strtouint64(get_field(&mem_field, table->field[11])));
			/* SUM_ROWS_SENT    */
			list_values[12]->add(util::strtouint64(get_field(&mem_field, table->field[12])));
			/* SUM_ROWS_EXAMINED    */
			list_values[13]->add(util::strtouint64(get_field(&mem_field, table->field[13])));
			/* SUM_CREATED_TMP_DISK_TABLES    */
			list_values[14]->add(util::strtouint64(get_field(&mem_field, table->field[14])));
			/*  SUM_CREATED_TMP_TABLES   */
			list_values[15]->add(util::strtouint64(get_field(&mem_field, table->field[15])));
			/* SUM_SELECT_FULL_JOIN   */
			list_values[16]->add(util::strtouint64(get_field(&mem_field, table->field[16])));
			/*  SUM_SELECT_FULL_RANGE_JOIN  */
			list_values[17]->add(util::strtouint64(get_field(&mem_field, table->field[17])));
			/*  SUM_SELECT_RANGE  */
			list_values[18]->add(util::strtouint64(get_field(&mem_field, table->field[18])));
			/*  SUM_SELECT_RANGE_CHECK   */
			list_values[19]->add(util::strtouint64(get_field(&mem_field, table->field[19])));
			/*  SUM_SELECT_SCAN   */
			list_values[20]->add(util::strtouint64(get_field(&mem_field, table->field[20])));
			/* SUM_SORT_MERGE_PASSES    */
			list_values[21]->add(util::strtouint64(get_field(&mem_field, table->field[21])));
			/*  SUM_SORT_RANGE   */
			list_values[22]->add(util::strtouint64(get_field(&mem_field, table->field[22])));
			/* SUM_SORT_ROWS   */
			list_values[23]->add(util::strtouint64(get_field(&mem_field, table->field[23])));
			/*  SUM_SORT_SCAN  */
			list_values[24]->add(util::strtouint64(get_field(&mem_field, table->field[24])));
			/*  SUM_NO_INDEX_USED   */
			list_values[25]->add(util::strtouint64(get_field(&mem_field, table->field[25])));
			/*  SUM_NO_GOOD_INDEX_USED  */
			list_values[26]->add(util::strtouint64(get_field(&mem_field, table->field[26])));
			/* FIRST SEEN */
			uint64_t first_milis =  util::get_seconds(get_field(&mem_field, table->field[27]),"%Y-%m-%d %H:%M:%S");
			list_values[27]->add(first_milis);
			/*LAST SEEN */
			uint64_t last_milis = util::get_seconds(get_field(&mem_field, table->field[28]),"%Y-%m-%d %H:%M:%S");
			list_values[28]->add(last_milis);

		}
		netproxy->send_pack(pack);
		free_root(&mem_field,MYF(0));
		end_read_record(&read_record_info);
	  //table->m_needs_reopen= TRUE;
		table->file->inited = handler::NONE;
	  close_mysql_tables(thd);// Force close to free memory
	  //sql_print_information("spotter-end data read...table name:%s",tblname);

	 }else {
		 free_root(&mem_field,MYF(0));
	 }

	DBUG_RETURN(true);

}


void pfs_event_stmt_summary_reader::init_column_nm() {
	std::string temp[] = {"SCHEMA_NAME","DIGEST","DIGEST_TEXT",
			"COUNT_STAR","SUM_TIMER_WAIT","MIN_TIMER_WAIT",
			"AVG_TIMER_WAIT","MAX_TIMER_WAIT","SUM_LOCK_TIME",
			"SUM_ERRORS","SUM_WARNINGS","SUM_ROWS_AFFECTED",
			"SUM_ROWS_SENT","SUM_ROWS_EXAMINED","SUM_CREATED_TMP_DISK_TABLES",
			"SUM_CREATED_TMP_TABLES","SUM_SELECT_FULL_JOIN","SUM_SELECT_FULL_RANGE_JOIN",
			"SUM_SELECT_RANGE","SUM_SELECT_RANGE_CHECK","SUM_SELECT_SCAN",
			"SUM_SORT_MERGE_PASSES","SUM_SORT_RANGE","SUM_SORT_ROWS",
			"SUM_SORT_SCAN","SUM_NO_INDEX_USED","SUM_NO_GOOD_INDEX_USED","FIRST_SEEN","LAST_SEEN"};
	int size = sizeof(temp) / sizeof(temp[0]);
	std::copy(temp,temp+size, columns);
}

} /* namespace spotter */
