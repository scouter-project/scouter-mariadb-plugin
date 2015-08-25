#include "net/net_proxy.h"
#include "util/util.h"
#include "gtest/gtest.h"
#include <iostream>
#include "data_pack/perf_counter_pack.h"

#include "value/decimal_value.h"
namespace spotter {

class TestGlobalStatusSend : public ::testing::Test {
protected:
	TestGlobalStatusSend() {

	}

	virtual ~TestGlobalStatusSend() {

	}

};

TEST(TestGlobalStatusSend,test_send_global_status) {
	net_proxy* proxy = net_proxy::get_instance();
	perf_counter_pack* pack = new perf_counter_pack();
	pack->insert("key1",new decimal_value(10));
	pack->insert("key2",new decimal_value(20));
	pack->insert("key3",new decimal_value(30));
	proxy->send_pack(pack);
	//
	//delete pack;
	//delete proxy;


}


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

