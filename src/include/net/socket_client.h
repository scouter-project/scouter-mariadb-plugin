/*
 * data_sender.h
 *
 *  Created on: 2015. 4. 28.
 *      Author: windfree
 */

#ifndef SRC_NET_DATA_SENDER_H_
#define SRC_NET_DATA_SENDER_H_
//#ifdef _LINUX_

#include <sys/socket.h>
//#endif
namespace spotter {
class socket_client {
private:
protected:
	int sockfd;
	bool is_connect;
	sockaddr* dest_addr;
public:
	socket_client(struct sockaddr* serv_addr);
	socket_client();
	virtual ~socket_client();
	virtual void close() = 0;
	virtual bool is_connected() = 0;
	virtual int send(char* buffer, int32_t len) = 0;

};

} /* namespace spotter */

#endif /* SRC_NET_DATA_SENDER_H_ */
