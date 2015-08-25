/*
 * ipfs_reader.h
 *
 *  Created on: 2015. 4. 13.
 *      Author: windfree
 */

#ifndef PLUGIN_SPOTTER_IPFS_READER_H_
#define PLUGIN_SPOTTER_IPFS_READER_H_

#include "task/task.h"
#include "sql_base.h"
class THD;
struct TABLE_LIST;
struct TABLE;
namespace spotter {
struct mysql_spotter_context;
class pfs_reader : public task {
private:
	bool going_down();
protected:
	TABLE_LIST tables;
	TABLE* table;
	THD* thd;
	const char* dbname ;
	const char* tblname ;

	int execute(void* p) ;
	virtual bool read() = 0;

public:
	pfs_reader();
	virtual ~pfs_reader();

};

} /* namespace spotter */

#endif /* PLUGIN_SPOTTER_IPFS_READER_H_ */
