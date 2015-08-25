/*
 * test_decimal_value.cc
 *
 *  Created on: 2015. 5. 13.
 *      Author: windfree
 */
#include "value/decimal_value.h"
#include "value/map_value.h"
#include "value/text_value.h"
#include "value/list_value.h"
#include "io/data_output.h"
#include "io/data_input.h"
#include "gtest/gtest.h"
#include <iostream>

namespace spotter {
class Test_value : public ::testing::Test {
protected:
	Test_value() {};
	virtual ~Test_value() {}

public:


};


TEST(Test_value, test_decimal_value) {
	decimal_value* value1 = new decimal_value(15023404000);
	data_output* out = new data_output();
	//value1->write(out);
	out->write_value(value1);
	char* buffer = out->to_byte_array();
	data_input* in = new data_input(buffer);
	decimal_value* value2 = (decimal_value*)in->read_value();
	ASSERT_EQ(value1->get_value() ,value2->get_value());



	delete value2;
	delete value1;
	delete out;
	delete in;


}

TEST(Test_value, test_text_value) {
	text_value* value1 = new text_value("test value");
	std::cout<<"val1->value:"<<value1->get_value()<<std::endl;
	data_output* out = new data_output();
	//value1->write(out);
	out->write_value(value1);
	char* buffer = out->to_byte_array();
	data_input* in = new data_input(buffer);
	text_value* value2 = (text_value*)in->read_value();
	std::cout<<"val2->value:"<<value2->get_value()<<std::endl;
	ASSERT_EQ(*(value1->get_value()) ,*(value2->get_value()));

	delete value2;
	delete value1;
	delete out;
	delete in;

}


TEST(Test_value, test_list_value) {
	list_value* lvalue1 = new list_value();
	decimal_value*  dvalue1 = new decimal_value(500);
	decimal_value*  dvalue2 = new decimal_value(600);

	lvalue1->add((dvalue1));
	lvalue1->add((dvalue2));

  data_output* out = new data_output();
	out->write_value(lvalue1);
	char* buffer = out->to_byte_array();
	data_input* in = new data_input(buffer);
	list_value* lvalue2 = (list_value*)in->read_value();

	decimal_value* dval1 = (decimal_value*)(lvalue2->get(0));
	ASSERT_EQ(500,dval1->get_value());

	decimal_value* dval2= (decimal_value*)lvalue2->get(1);
	ASSERT_EQ(600,dval2->get_value());

	delete lvalue1;
	delete lvalue2;

	delete in;
}

/*
TEST(Test_value,test_map_value) {
	decimal_value* dvalue = new decimal_value(100);
	map_value* mvalue = new map_value();
	mvalue->insert("test",dvalue);
	data_output* out = new data_output();
	out->write_value(mvalue);
	char* buffer = out->to_byte_array();
	data_input* in = new data_input(buffer);
	map_value* mvalue2 = (map_value*)in->read_value();
	decimal_value* dvalue2 = (decimal_value*)mvalue2->get_value("test");
	ASSERT_EQ(dvalue->get_value(),dvalue2->get_value());

		delete dvalue2;
		delete dvalue;
		delete out;
		delete in;
		delete mvalue;
		delete mvalue2;

}
*/

}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
