/*
 * tcp_socket_client.h
 *
 *  Created on: 2015. 7. 28.
 *      Author: windfree
 */

#ifndef SRC_NET_TCP_SOCKET_CLIENT_H_
#define SRC_NET_TCP_SOCKET_CLIENT_H_
#include "net/socket_client.h"
namespace spotter {

class tcp_socket_client: public spotter::socket_client {
private:
	int32_t use_tcp_no_delay;
	int32_t send_buff_size;

	void sock_init();
public:
	tcp_socket_client();
	tcp_socket_client(struct sockaddr* serv_addr);
	virtual ~tcp_socket_client();
	void close();
	bool connect();
	bool is_connected();
	int send(char* buffer,int32_t len);
	void set_addr(sockaddr* addr);
	void set_use_no_delay(int32_t value);
	void set_send_buff_size(int32_t value);
	int32_t readn(char* buff, int32_t len);
	int32_t writen(char* buff, int32_t len );

};

} /* namespace spotter */

#endif /* SRC_NET_TCP_SOCKET_CLIENT_H_ */
