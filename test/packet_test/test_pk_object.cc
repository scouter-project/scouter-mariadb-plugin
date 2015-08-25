#include "net/net_proxy.h"
#include "util/util.h"
#include "gtest/gtest.h"
#include <iostream>
#include "data_pack/object_pack.h"
#include "util/obj_name_util.h"
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
	object_pack* opack = new object_pack();
	opack->set_obj_type(obj_name_util::get_instance()->object_type());
	opack->set_obj_name(obj_name_util::get_instance()->object_name());
	opack->set_obj_hash(obj_name_util::get_instance()->object_hash());
	opack->set_version(std::string("1.0"));
	//opack->put(std::string("db.ver"),opack->get_version() );
	net_proxy::get_instance()->send_pack(opack);
//
	//delete pack;
	//delete proxy;


}


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

