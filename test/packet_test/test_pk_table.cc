#include "net/net_proxy.h"
#include "util/util.h"
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <string>
#include "data_pack/table_pack.h"
#include "value/list_value.h"
namespace spotter {

class TestGlobalStatusSend : public ::testing::Test {
protected:
	TestGlobalStatusSend() {

	}

	virtual ~TestGlobalStatusSend() {

	}

};

TEST(TestGlobalStatusSend,test_send_table_pack) {
	table_pack* pack = new table_pack();
	pack->set_key("stmt_current");

	list_value *list_values[3] ;
	for(int i = 0 ; i < 3; i++	) {
		list_values[i] = new list_value();
		std::stringstream ss;
		ss<<i;
		std::string key = ss.str();
	 	pack->new_list_value(key, list_values[i]);
	}

	list_values[0]->add("test_value1");
	list_values[1]->add(100);
	list_values[2]->add(200);

	list_values[0]->add("test_value2");
	list_values[1]->add(300);
	list_values[2]->add(400);

	net_proxy::get_instance()->send_pack(pack);
	//
	//delete pack;
	//delete proxy;


}


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

