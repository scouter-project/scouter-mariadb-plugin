/*
 * test_tcp.cc
 *
 *  Created on: 2015. 7. 30.
 *      Author: windfree
 */
#include "gtest/gtest.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../src/include/manager/command_manager.h"
#include "../../src/include/net/tcp_socket_client.h"
#include "../../src/include/io/data_output.h"

namespace spotter {
class test_tcp : public ::testing::Test {
	protected:
	test_tcp() {
	}
	virtual ~test_tcp() {
	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

TEST_F(test_tcp,cmd_manager_connect) {
	command_manager* mgr = command_manager::get_instance();
	const int TCP_AGENT = 0xCAFE1001;
	mgr->run();
	sleep(1);


	EXPECT_EQ(true,true);
}


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
