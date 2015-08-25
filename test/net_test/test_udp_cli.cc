/*
 * test_udp_data_sender.cc
 *
 *  Created on: 2015. 4. 29.
 *      Author: windfree
 */
#include "io/data_output.h"
#include "gtest/gtest.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../src/include/net/udp_sender.h"
#include "../../src/include/net/udp_socket_client.h"
namespace spotter {
class test_udp_data_sender : public ::testing::Test {
	protected:

	test_udp_data_sender() {
	}

	virtual ~test_udp_data_sender() {

	}

	virtual void SetUp() {

	}

	virtual void TearDown() {
	}
};

TEST_F(test_udp_data_sender,connect) {
	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	/*inet_pton(AF_INET,"165.243.31.38",&sock_addr.sin_addr);
	sock_addr.sin_port = htons(6100);*/
	/*udp_socket_client* udp = new udp_socket_client();
	udp->set_addr((struct sockaddr*)&sock_addr);*/
	udp_sender* sender = udp_sender::get_instance();
	char* addr = "165.243.31.37";
	sender->set_addr(addr,6200);
	sender->start();
	bool status = false;
	while(status == false) {
		status = sender->is_running();
		sleep(1);
	}
	EXPECT_EQ(true,status);

}

TEST_F(test_udp_data_sender,stop_thread) {
	sleep(1);
	struct sockaddr_in sock_addr;
	sock_addr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&sock_addr.sin_addr);
	sock_addr.sin_port = htons(3800);
	udp_sender* sender = udp_sender::get_instance();
	sender->stop();
	delete sender;
}

}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

