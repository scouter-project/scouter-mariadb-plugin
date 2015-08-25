#include "util/util.h"
#include "util/obj_name_util.h"
#include "common/spotter_var.h"
#include "gtest/gtest.h"
#include <iostream>
#include "util/hash_set.h"
#include <pthread.h>
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

TEST_F(test_util,test_sleep) {
	int i = 3;
	pthread_cond_init(&spotter_cond,NULL);
	util::sleep(3);
	ASSERT_TRUE(1==1);

}

}
int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
