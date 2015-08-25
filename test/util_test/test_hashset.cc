#include "util/util.h"
#include "gtest/gtest.h"
#include <iostream>

#include "../../src/include/util/hash_set.h"
using namespace std;
namespace spotter {
class test_hash : public ::testing::Test {

protected:
	test_hash() {

	}

	virtual ~test_hash() {

	}

};

TEST(test_hash,test_string_hash) {
	hash_set* hset = new hash_set();
	char* str = "this is a test";
	int hash = util::hash(str);
	hset->add(hash);
	hset->add(hash);
	str = "anther string";
	hash = util::hash(str);
	hset->add(hash);
	EXPECT_EQ(2,hset->count());

	set<int32_t> value_set = hset->get_array();
	for(set<int32_t>::iterator it = value_set.begin(); it != value_set.end();++it) {
		std::cout<<"hash value in string set:"<<*it<<endl;
	}
	delete hset;

}

}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
