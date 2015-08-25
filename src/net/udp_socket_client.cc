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
#include "config/configure.h"
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
	return ::send(sockfd,buffer,len,0);
}

} /* namespace spotter */

