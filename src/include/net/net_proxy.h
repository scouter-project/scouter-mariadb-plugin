/*
 * Netproxy.h
 *
 *  Created on: 2015. 4. 23.
 *      Author: windfree
 */

#ifndef SRC_NET_NET_PROXY_H_
#define SRC_NET_NET_PROXY_H_
#include "net/udp_sender.h"
#include <stdint.h>
namespace spotter {
class pack;
class hash_set;
class net_proxy {
private:
	static net_proxy* instance;
	net_proxy();
	static pthread_mutex_t mutex;
	udp_sender* sender;
	hash_set* string_hash;
	char local_addr[16];
public:
	static net_proxy* get_instance();
	void send_text(const char* text, int32_t text_hash);
	void send_pack(pack* p);
	virtual ~net_proxy();
	char* get_local_addr();
	bool is_available();
	void clear_hash_set();
};

} /* namespace spotter */

#endif /* SRC_NET_NET_PROXY_H_ */
