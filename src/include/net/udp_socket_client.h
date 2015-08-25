/*
 * udp_data_sender.h
 *
 *  Created on: 2015. 4. 28.
 *      Author: windfree
 */

#ifndef SRC_NET_UDP_DATA_SENDER_H_
#define SRC_NET_UDP_DATA_SENDER_H_

#include "net/socket_client.h"
#include <queue>

using namespace std;
namespace spotter {
class pack;
class data_output;
class udp_socket_client: public socket_client {
private:

public:
	udp_socket_client(struct sockaddr* serv_addr);
	udp_socket_client();
	~udp_socket_client();
	void close();
	bool connect();
	bool is_connected();
	int send(char* buffer,int32_t len);
	void set_addr(sockaddr* addr);
};

} /* namespace spotter */

#endif /* SRC_NET_UDP_DATA_SENDER_H_ */
