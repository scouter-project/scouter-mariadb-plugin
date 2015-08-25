/*
 * spotter_heartbeat.cc
 *
 *  Created on: 2015. 6. 12.
 *      Author: windfree
 */

#include "task/spotter_heartbeat.h"
#include "data_pack/object_pack.h"
#include "util/obj_name_util.h"
#include "util/log.h"
#include "net/net_proxy.h"
namespace spotter {

spotter_heartbeat::spotter_heartbeat() {
	name_util = obj_name_util::get_instance();

}

spotter_heartbeat::~spotter_heartbeat() {
	// TODO Auto-generated destructor stub
}

int spotter_heartbeat::execute(void* p) {
	do {
		object_pack* opack = new object_pack();
		opack->set_obj_type(name_util->object_type());
		opack->set_obj_name(name_util->object_name());
		opack->set_obj_hash(name_util->object_hash());
		opack->set_version(std::string("1.0"));
		netproxy->send_pack(opack);
		//delete opack;
	}while(slept_ok(interval));

}

} /* namespace spotter */


