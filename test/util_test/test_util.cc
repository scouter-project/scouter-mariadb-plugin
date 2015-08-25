#include "util/util.h"
#include "util/obj_name_util.h"
#include "gtest/gtest.h"
#include <iostream>
#include "util/hash_set.h"
using namespace std;
namespace spotter {
class test_util : public ::testing::Test {

protected:
	test_util() {

	}

	virtual ~test_util() {

	}

	virtual void SetUp() {

	}

	virtual void TearDown() {


	}
};

TEST_F(test_util,test_gen_hash) {
	char* str = "sradev1/3308";
	int hash = util::hash(str);
	cout<<"hash value for spotter:"<<hash<<endl;
	str = "hello my boy";
	hash = util::hash(str);
	cout<<"hash value:"<<hash<<endl;
	ASSERT_TRUE(1==1);

}

TEST_F(test_util,test_obj_util) {
	cout<<"obj name:"<<obj_name_util::get_instance()->object_name()<<endl;
	cout<<"obj_hash:"<<util::hash((char*)obj_name_util::get_instance()->object_name().c_str());
}

TEST_F(test_util,test_timeutil) {
	const char* time = "2015-06-18 15:00:01";
	const char* format = "%Y-%m-%d %H:%M:%S";
	time_t value = util::get_seconds(time,format);
	if(value == 0) {
		cout<<"incorrect format"<<endl;
	} else {
		cout<<"time value:"<<value<<endl;
	}
}
}
int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
