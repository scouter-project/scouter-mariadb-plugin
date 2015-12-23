/*
 * command_manager.cc
 *
 *  Created on: 2015. 7. 28.
 *      Author: windfree
 */
#include "log.h"
#include "manager/command_manager.h"
#include "manager/service_manager.h"
#include "net/tcp_socket_client.h"
#include "net/net_proxy.h"
#include "io/data_output.h"
#include "util/constants.h"
#include "util/obj_name_util.h"
#include "util/util.h"
#include <stddef.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "common/spotter_var.h"
#include "util/util.h"
namespace spotter {

command_manager* command_manager::instance = 0;
pthread_mutex_t command_manager::mutex;
bool command_manager::connected = false;
command_manager::command_manager() {
	connected = false;

	thread_id = -1;

}
command_manager::~command_manager() {
	if(tcp_sock != 0) {
		delete tcp_sock;
		tcp_sock = 0;
	}

}

command_manager* command_manager::get_instance() {
	if(instance  == 0) {
		pthread_mutex_init(&mutex,NULL);
		pthread_mutex_lock(&mutex);
		instance = new command_manager();
		pthread_mutex_unlock(&mutex);
		pthread_mutex_destroy(&mutex);
	}
	return instance;
}

int command_manager::execute(void* p) {
	/*while(util::sleep(1	)) {
		std::cout<<"retry connect"	<<std::endl;
		if(connect()) {
			break;
		}
	}*/
	if(connect()) {
		send_object_hash();
		read_command();
	}

}

void* command_manager::start_thread(void* p) {
		((command_manager*)p)->execute(NULL);
}

void command_manager::run() {
	if(!connected) {
		if (pthread_create(&thread_id, 0, start_thread,(void *) this)!= 0) {
			sql_print_error("Could not create spotter command manager thread! errno:%d\n",errno);
			//exit(0);
		}
	}
}

bool command_manager::connect() {
	/* for test only setting ip/port  */
	tcp_sock = new tcp_socket_client();
	sock_addr.sin_family = AF_INET;
	int result ;
	result = inet_pton(AF_INET,server_ip,&sock_addr.sin_addr);
	if(result == 1) {
		sock_addr.sin_port = htons(server_tcp_port);
		tcp_sock->set_addr((struct sockaddr*)&sock_addr);
		connected = tcp_sock->connect();
		if(!connected) {
			delete tcp_sock;
			tcp_sock = 0;
		}
	} else {
		delete tcp_sock;
		tcp_sock = 0;
	}
	return connected;

}

void command_manager::send_object_hash() {
	data_output* out = new data_output();
	out->write_int32(TCP_AGENT);
	out->write_int32(obj_name_util::get_instance()->object_hash());
	int32_t size = out->get_offset();
	char* buff = out->to_byte_array();
	tcp_sock->writen(buff,size);
	delete[] buff;
	buff = 0;
	delete out;
	out = 0;
}

void command_manager::read_command() {
	while(connected) {
		uint8_t base_len;
		int32_t rc = read((char*)&base_len,1);
		if(rc == 0) { // server terminated
			handle_disconnect();
			break;
		}
		if(rc == -1) {
			continue;
		}
		int cmd_len = base_len;
		switch(cmd_len) {
			case 255: {
				int16_t len;
				if(!read((char*)&len,2)) {
							break;
				}
				len = ntohs(len);
				char* cmd = new char[len];
				if(!read(cmd,len)) {
					delete[] cmd;
					break;
				}
				handle_command(cmd);
				break;
			}
			case 254: {
				int32_t len;
				if(!read((char*)&len,4)) {
					break;
				}
				len = ntohl(len);
				char* cmd = new char[len];
				if(!read(cmd,len)) {
					delete[] cmd;
					break;
				}
				handle_command(cmd);
				break;
			}
			case 0:
				break;
			default: {
				char* cmd = new char[cmd_len + 1];
				if(!read(cmd,cmd_len)) {
					delete[] cmd;
					break;
				}
				cmd[cmd_len] = 0;
				handle_command(cmd);
				break;
			}
		}
	}

}


int command_manager::read(char* buff, int32_t size) {
	int rc = tcp_sock->readn(buff,size	);

	return rc;
}

void command_manager::handle_command(char* cmd) {
	uint8_t pack_type;
	if(!read((char*)&pack_type,1)) {
		delete[] cmd;
		return;
	}
	// read table count inside map pack and ignore it.
	uint8_t count;
	if(!read((char*)&count,1)) {
		delete[] cmd;
		return;
	}

	if(strcmp(cmd,"KEEP_ALIVE")== 0) { //kepp alive command, do nothing
	}else if(strcmp(cmd,"OBJECT_RESET_CACHE") == 0) {  // hash reset command. clear hash.
		net_proxy::get_instance()->clear_hash_set();
	} else {

	}
	uint8_t flag = 0x04;
	tcp_sock->writen((char*)&flag,1);

	delete[] cmd;
}

/*
 * if scouter server not ready, try to connect repeatedly.
*/


bool command_manager::is_connected() {
	return connected;
}

void command_manager::stop() {
	connected = false;
	if(thread_id != -1) {
		pthread_cancel(thread_id);
		pthread_join(thread_id, NULL);
	}
	if(tcp_sock !=0 && tcp_sock->is_connected()) {
		tcp_sock->close();
	}
}

void command_manager::handle_disconnect() {
	tcp_sock->close();
	connected = false;
	net_proxy::get_instance()->clear_hash_set();

}

} /* namespace argos */
