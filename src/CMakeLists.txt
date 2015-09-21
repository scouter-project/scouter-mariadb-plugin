
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

include_directories( ${CMAKE_SOURCE_DIR}/plugin/spotter/src/include )

#set(LIBS /home/windfree/workspace/ws-mariadb/mariadb-10.0.20/libmysqld/libmysqld.so)

ADD_DEFINITIONS(-DNO_EMBEDDED_ACCESS_CHECKS)

SET(SPOTTER_SOURCES  
 include/spotter.h
 include/task/task.h
 include/task/pfs_reader.h
 include/task/pfs_event_stmt_current_reader.h
 include/task/pfs_event_stmt_summary_reader.h
 include/task/ifs_global_status_reader.h
 include/task/spotter_heartbeat.h
 include/task/ifs_innodb_locks_reader.h
 include/task/observer.h
 include/util/log.h
 include/util/util.h
 include/util/hash_set.h
 include/util/obj_name_util.h
 include/util/constants.h
 include/io/data_output.h
 include/io/data_input.h
 include/data_pack/pack.h
 include/data_pack/table_pack.h
 include/data_pack/perf_counter_pack.h
 include/data_pack/text_pack.h
 include/data_pack/object_pack.h
 include/data_pack/pack_constants.h
 include/net/socket_client.h
 include/net/udp_socket_client.h
 include/net/tcp_socket_client.h
 include/net/net_proxy.h
 include/net/udp_sender.h
 include/value/value.h
 include/value/value_constants.h
 include/value/null_value.h
 include/value/number_value.h
 include/value/decimal_value.h
 include/value/text_value.h
 include/value/list_value.h
 include/value/map_value.h
 include/config/configure.h
 include/manager/service_manager.h
 include/manager/command_manager.h
 include/common/spotter_var.h
 spotter.cc
 task/task.cc
 task/pfs_reader.cc
 task/pfs_event_stmt_current_reader.cc
 task/pfs_event_stmt_summary_reader.cc
 task/ifs_global_status_reader.cc
 task/spotter_heartbeat.cc
 task/ifs_innodb_locks_reader.cc
 task/observer.cc
 util/log.cc
 util/util.cc
 util/hash_set.cc
 util/obj_name_util.cc
 io/data_output.cc
 io/data_input.cc
 data_pack/pack.cc
 data_pack/table_pack.cc
 data_pack/perf_counter_pack.cc
 data_pack/text_pack.cc
 data_pack/object_pack.cc
 data_pack/pack_constants.cc
 net/socket_client.cc
 net/udp_socket_client.cc
 net/tcp_socket_client.cc
 net/net_proxy.cc
 net/udp_sender.cc
 value/value.cc
 value/value_constants.cc
 value/null_value.cc
 value/number_value.cc
 value/decimal_value.cc
 value/text_value.cc
 value/list_value.cc
 value/map_value.cc
 config/configure.cc
 manager/service_manager.cc
 manager/command_manager.cc
   )

ADD_DEFINITIONS(-DMYSQL_DYNAMIC_PLUGIN  )
#target_link_libraries(spotter ${LIBS})
MYSQL_ADD_PLUGIN(spotter ${SPOTTER_SOURCES}
  									 MODULE_OUTPUT_NAME "libspotter"
  									  )

INSTALL(FILES spotter.ini DESTINATION ${INSTALL_PLUGINDIR} )


