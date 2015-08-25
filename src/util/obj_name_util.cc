/*
 * obj_name_util.cc
 *
 *  Created on: 2015. 5. 20.
 *      Author: windfree
 */
#include "mysqld.h"
#include "util/obj_name_util.h"
#include "util/util.h"
#include<sstream>
#include <unistd.h>
#include <pthread.h>
uint mysqld_port;
namespace spotter {
obj_name_util* obj_name_util::instance = NULL;
obj_name_util::obj_name_util() {
	obj_type = std::string("mariaplugin");

	init_name();
	init_hash();
}

obj_name_util::~obj_name_util() {

}

obj_name_util* obj_name_util::get_instance(){
	if(instance == NULL) {
		pthread_mutex_t mutex;
		pthread_mutex_init(&mutex,NULL);
		pthread_mutex_lock(&mutex);
		if(instance == NULL) {
			instance = new obj_name_util();
		}
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);
	}
	return instance;
}

void obj_name_util::init_name() {
	char hname[80];
	if(gethostname(hname,sizeof(hname)) != 0) {
		obj_name = "/host";
	}
	std::string sep("/");
	std::string value(hname);
	std::string str_port = util::to_string(mysqld_port);
	obj_name = sep.append(hname).append("/").append(str_port);
}

void obj_name_util::init_hash() {
	obj_hash = util::hash((char*)obj_name.c_str());
}


std::string obj_name_util::object_name() {
	return obj_name;
}

std::string obj_name_util::object_type() {
	return obj_type;
}

int32_t obj_name_util::object_hash() {
	return obj_hash;
}

} /* namespace spotter */

