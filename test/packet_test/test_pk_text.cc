/*
 * test_pk_text.cc
 *
 *  Created on: 2015. 6. 18.
 *      Author: windfree
 */




#include "net/net_proxy.h"
#include "util/util.h"
#include "gtest/gtest.h"
#include <iostream>
#include "data_pack/text_pack.h"

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
	char* text = "hello world";
	int32_t hash = util::hash(text);

	proxy->send_text(text,hash);
	char* text2="SELECT C_FIRST, C_MIDDLE, C_ID, C_STREET_1, C_STREET_2, C_CITY, C_STATE, C_ZIP, C_PHONE, C_CREDIT, C_CREDIT_LIM, C_DISCOUNT, C_BALANCE, C_YTD_PAYMENT, C_PAYMENT_CNT, C_SINCE FROM CUSTOMER WHERE C_W_ID = 1 AND C_D_ID = 10 AND C_LAST = 'OUGHTOUGHTPRES' ORDER BY C_FIRST";
	int32_t hash2 = util::hash(text2);
	proxy->send_text(text2,hash2);
	//
	//delete pack;"
	//delete proxy;
   sleep(2);

}


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}

