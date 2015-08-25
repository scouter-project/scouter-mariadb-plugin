/*
 * tcp_socket_client.cc
 *
 *  Created on: 2015. 7. 28.
 *      Author: windfree
 */
#include "my_global.h"
#include "log.h"
#include "net/tcp_socket_client.h"
#include <iostream>
#include <netinet/tcp.h>
#include <netinet/in.h>
namespace spotter {

tcp_socket_client::tcp_socket_client() {
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	use_tcp_no_delay = 0;
	send_buff_size = 0;
	is_connect = false;
	sock_init();

}

tcp_socket_client::tcp_socket_client(struct sockaddr* serv_addr) {
	tcp_socket_client();
	dest_addr = serv_addr;
}

tcp_socket_client::~tcp_socket_client() {
	close();

}


void tcp_socket_client::sock_init() {
	int flag = fcntl(sockfd,F_GETFL,0);
	fcntl(sockfd,F_SETFL,flag|O_NONBLOCK);

	if(send_buff_size > 0) {
		setsockopt(sockfd,SOL_SOCKET,SO_SNDBUF,(char*)send_buff_size,sizeof(send_buff_size)	);
	}
	if(use_tcp_no_delay) {
		int nval = 1;
		setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,(char*)nval,sizeof(int));
	}
}

void tcp_socket_client::close() {
	if(is_connect) {
		::close(sockfd);
	}
	is_connect = false;

}

bool tcp_socket_client::connect() {
	if(is_connect) {
		return true;
	}
	fd_set wset,rset;
	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(sockfd,&rset);
	FD_SET(sockfd,&wset);
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	int rc = ::connect(sockfd,dest_addr,sizeof(*dest_addr));
	if(rc && errno != EINPROGRESS) {
		return false;
	}
	if (rc == 0) { // already connected
		is_connect = true;
		return true;
	}
	rc = select(sockfd + 1,&rset,&wset,NULL,&timeout);
	if(rc == 0) { // connection timeout
		return false;
	}
	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset)) {
		int opt;
		uint len = sizeof(opt);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&opt,&len) < 0) {
			return false;
		}
		if(opt) {
			return false;
		}
		is_connect = true;
		return true;
	}


}

bool tcp_socket_client::is_connected() {
	return is_connect;

}

int tcp_socket_client::send(char* buffer, int32_t len) {

}

void tcp_socket_client::set_addr(sockaddr* addr) {
	dest_addr = addr;

}

void tcp_socket_client::set_use_no_delay(int32_t value) {
	use_tcp_no_delay = value;
}

void tcp_socket_client::set_send_buff_size(int32_t value) {
	send_buff_size = value;
}


int tcp_socket_client::readn(char* buff, int32_t len) {
	fd_set rset,fd;
	FD_ZERO(&rset);
	if(sockfd < 0 || sockfd >= FD_SETSIZE) {
		return 0;
	}
	FD_SET(sockfd,&rset);
	struct timeval timeout;

	timeout.tv_usec = 0;

	int32_t left_size;
	int32_t read_size;
	left_size = len;
	char* buf ;
	buf = buff;

	while(left_size > 0) {
		timeout.tv_sec = 10;
		fd = rset;
		int rc = select(sockfd + 1,&fd,NULL,NULL,&timeout);
		if(rc < 0) { //select failure
			//return 0;
		}
		if(rc == 0) { // if select timeout, continue while loop.
			continue;
		}
		if(FD_ISSET(sockfd,&fd)) {
			read_size = ::read(sockfd,buf,left_size);
			if(read_size < 0) {
				if(errno == EINTR) {
					read_size = 0;  // call read again.
				} else {
					return -1;
				}
			} else if(read_size == 0) { // EOF
				break;
			}
			left_size -= read_size;
			buf += read_size;
		}
	}
	return (len - left_size);

}

int32_t tcp_socket_client::writen(char* buff, int32_t len) {
	fd_set wset,fd;
	FD_ZERO(&wset);
	if(sockfd < 0 || sockfd >= FD_SETSIZE) {
			return 0;
	}
	FD_SET(sockfd,&wset);

	int32_t left_size;
	int32_t write_size;
	char* buf;
	buf = buff;
	left_size = len;

	while(left_size > 0) {
		fd = wset;
		select(sockfd+1,NULL,&fd,NULL,NULL);
		if(FD_ISSET(sockfd,&wset)) {
			write_size = ::write(sockfd,buf,left_size);
			if(write_size  <=0) {
				if(write_size < 0 && errno == EINTR) {
					write_size = 0;
				}else {
					return -1;
				}
			}
			left_size -= write_size;
			buf += write_size;
		}
	}
	return len;
}

} /* namespace spotter */
