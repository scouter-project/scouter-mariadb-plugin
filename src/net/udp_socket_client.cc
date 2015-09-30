/*
 * udp_data_sender.cc
 *
 *  Created on: 2015. 4. 28.
 *      Author: windfree
 */
#include "my_global.h"
#include "data_pack/pack.h"
#include "io/data_output.h"
#include <iostream>
#include <string.h>
#include "net/udp_socket_client.h"
#include "util/obj_name_util.h"
#include "util/util.h"
#include "net/net_constants.h"
#include "config/configure.h"
#include <stdio.h>
namespace spotter {

udp_socket_client::udp_socket_client() {
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
	is_connect = false;
}
udp_socket_client::udp_socket_client(struct sockaddr* serv_addr) : socket_client(serv_addr) {
	udp_socket_client();
	dest_addr = serv_addr;
}

udp_socket_client::~udp_socket_client() {
	::close(sockfd);
}

void udp_socket_client::set_addr(sockaddr* addr) {
	dest_addr = addr;
}

bool udp_socket_client::connect() {
	if(is_connect) {
		return true;
	}
	::connect(sockfd,dest_addr,sizeof(*dest_addr));
	char* msg = "HB";
	::send(sockfd,msg,strlen(msg),0);
	unsigned int len = sizeof(*dest_addr);
	int rc = recvfrom(sockfd,msg,sizeof(msg),0,dest_addr,&len);
	if(rc < 0) {
		if(errno == EWOULDBLOCK) { // connect ok, but no more  data to read.
			is_connect = true;
			return true;
		}else {
			is_connect = false;
			return false;
		}
	}
	return true;
}

void udp_socket_client::close() {
	::close(sockfd);
	is_connect = false;
}

bool udp_socket_client::is_connected() {
	return is_connect;
}

int udp_socket_client::send(char* buffer, int32_t len) {
	if(len > configure::MAX_PACKET_SIZE) {
		char* buff = new char[len-4];
		memcpy(buff,buffer + 4, len -4);
		send_multi_packet(buff,len);
		delete[] buff;
		return 0;
	}
	return ::send(sockfd,buffer,len,0);
}


int udp_socket_client::send_multi_packet(char* buffer, int32_t len) {
	int64_t key = util::get_next_key();
	int32_t total = len / configure::MAX_PACKET_SIZE;
	int32_t remain = len % configure::MAX_PACKET_SIZE;
	if(remain > 0) {
		total++;
	}

	int num = 0;
	for(num = 0 ; num < len / configure::MAX_PACKET_SIZE ; num++	) {
		char* buff = new char[configure::MAX_PACKET_SIZE];
		memcpy(buff,buffer + (num * configure::MAX_PACKET_SIZE),configure::MAX_PACKET_SIZE);
		send_multi_packet(key,total,num,configure::MAX_PACKET_SIZE,buff);
		delete[] buff;
	}

	if(remain > 0) {
		char* buff = new char[remain];
		memcpy(buff,buffer + len - remain, remain);
		send_multi_packet(key,total,num,remain,buff);
		delete[] buff;
	}


}


int udp_socket_client::send_multi_packet(int64_t key, int32_t total,	int32_t num, int packet_size, char* buffer) {
	data_output* out = new data_output();
	int32_t obj_hash = obj_hash = obj_name_util::get_instance()->object_hash();
	out->write_bytes(net_constants::MTU_PACK,4);
	out->write_int32(obj_hash);
	out->write_int64(key);
	out->write_int16((int16_t)total);
	out->write_int16((int16_t)num);
	out->write_blob(buffer,packet_size);
	char* send_buff = out->to_byte_array();
	int32_t len = out->get_offset();
	/*std::cout<<"hash: "<<obj_hash<<"  key:" <<key<< " num:"<<num<<" body len:"<<packet_size<<std::endl;
	for(int i = 0; i < packet_size; i++) {
		printf("%d ",buffer[i]);
		if(i != 0 && i % 10 == 0) {
			cout<<std::endl;
		}
	}
	std::cout<<std::endl;*/
	::send(sockfd,send_buff,len,0);
	delete[] send_buff ;
	send_buff = 0;
	delete out;
	out = 0;
}

} /* namespace spotter */

