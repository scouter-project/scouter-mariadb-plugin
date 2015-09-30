/*
 * udpsender.h
 *
 *  Created on: 2015. 6. 23.
 *      Author: windfree
 */

#ifndef SRC_UDPSENDER_H_
#define SRC_UDPSENDER_H_

#include <pthread.h>
#include <vector>
#include <queue>
#include <netinet/in.h>
namespace spotter {
class data_output;
class pack;
class udp_socket_client;
class socket_client;
class udp_sender {
private :
	static udp_sender* instance;
	static pthread_mutex_t mutex;
	udp_sender();
	udp_socket_client* udp;
	pthread_t thread_id;
	std::vector<data_output*> buffer_vector ;
	std::queue<pack*> data_queue;
	static void* start_thread(void* p);

	void run();

	bool running;

	//void send(std::vector<data_output*> outs);
	void send_multi_pack(int pack_count);
	struct sockaddr_in sock_addr;

public:
	static udp_sender* get_instance();
	virtual ~udp_sender();

	bool add(pack* pack);
	void stop();
	bool is_running();
	void set_addr(char* addr, int port); //only use for test case.
	void start();
	socket_client* get_socket();
	void set_socket(udp_socket_client* udp_sock);
	void send(std::vector<data_output*>& outs);
	void send(pack* pk);
};

} /* namespace spotter */

#endif /* SRC_UDPSENDER_H_ */
