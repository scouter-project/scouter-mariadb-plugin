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
/*

 * pfs_event_stmt_summary_reader.h
 *
 *  Created on: 2015. 5. 19.
 *      Author: windfree
 */

#ifndef SRC_READER_PFS_EVENT_STMT_SUMMARY_READER_H_
#define SRC_READER_PFS_EVENT_STMT_SUMMARY_READER_H_

#include "task/pfs_reader.h"
#include <iostream>
#include "util/constants.h"
namespace spotter {

class pfs_event_stmt_summary_reader: public pfs_reader {
private:
	std::string columns[PFS_EVT_STMT_SUMMARY_COL_CNT];
	void init_column_nm();
	MEM_ROOT mem_field;
public:
	pfs_event_stmt_summary_reader();
	pfs_event_stmt_summary_reader(const pfs_event_stmt_summary_reader& clazz);
	virtual ~pfs_event_stmt_summary_reader();
	pfs_event_stmt_summary_reader& operator=(const pfs_event_stmt_summary_reader& clazz);
	bool read();



	//virtual void setContext(void* p);
};

} /* namespace spotter */

#endif /* SRC_READER_PFS_EVENT_STMT_SUMMARY_READER_H_ */
