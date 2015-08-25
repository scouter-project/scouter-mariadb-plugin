/*
 * command_manager.h
 *
 *  Created on: 2015. 7. 28.
 *      Author: windfree
 */

#ifndef SRC_MANAGER_COMMAND_MANAGER_H_
#define SRC_MANAGER_COMMAND_MANAGER_H_
#include <pthread.h>
#include <netinet/in.h>
namespace spotter {
class tcp_socket_client;
class net_proxy;
class util;
class command_manager {
private:
	command_manager();
	static pthread_mutex_t mutex;
	tcp_socket_client* tcp_sock;
	static command_manager* instance;
	pthread_t thread_id;
	pthread_t retry_thread_id;
	int execute(void* p	);
	static void* start_thread(void* p);
	static bool connected;
	struct sockaddr_in sock_addr;
	bool connect();
	void read_command();
	void send_object_hash();
	void handle_command(char* buff);
	void handle_disconnect();
	int read(char* buff, int32_t size);

public:
	static command_manager* get_instance();
	virtual ~command_manager();
	void run();
	bool is_connected();
	void stop();


};

} /* namespace argos */

#endif /* SRC_MANAGER_COMMAND_MANAGER_H_ */
