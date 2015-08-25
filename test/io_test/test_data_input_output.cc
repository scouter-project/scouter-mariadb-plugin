#include "io/data_output.h"
#include "io/data_input.h"
#include "gtest/gtest.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
namespace spotter {
class data_in_out_test : public ::testing::Test {
		protected:

		data_output* output;

		data_in_out_test() {
			output = NULL;

		}

		virtual ~data_in_out_test() {

		}

		virtual void SetUp() {
			output = new data_output();

		}

		virtual void TearDown() {
			delete output;
		}


};

	/*TEST_F(data_in_out_test,write_short_test) {

	}*/
TEST_F(data_in_out_test,write_int16_value_p) {
	int16_t value = 128;
	output->write_int16(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_int16());
	delete input;
}

TEST_F(data_in_out_test,write_int16_value_n) {
	int16_t value = -20;
	output->write_int16(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_int16());
	delete input;
}

TEST_F(data_in_out_test,write_uint16_value) {
	uint16_t value = pow(2, 16) -1;
	output->write_uint16(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_uint16());
	delete input;
}

TEST_F(data_in_out_test,write_int32_value_p) {
	int32_t value = 20178765;
	output->write_int32(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_int32());
	delete input;
}

TEST_F(data_in_out_test,write_int32_value_n) {
	int32_t value = -20178765;
	output->write_int32(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_int32());
	delete input;
}

TEST_F(data_in_out_test,write_uint32_value) {
	uint32_t value = pow(2,32) -1;
	output->write_uint32(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_uint32());
	delete input;
}

TEST_F(data_in_out_test,write_uint64_value) {
	uint64_t value = pow(2,64) -1;
	output->write_uint64(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_uint64());
	delete input;
}

TEST_F(data_in_out_test,write_int64_value) {
	int64_t value = 0 - (pow(2,31) -1);
	output->write_int64(value);
	char* buffer = output->to_byte_array();
	data_input* input = new data_input(buffer);

	EXPECT_EQ(value,input->read_int64());
	delete input;
}

TEST_F(data_in_out_test,write_string_value) {
	output->write_uint8(50);
	output->write_string("maria");
	char* value = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	output->write_string(value);
	char* buffer = output->to_byte_array();
	//data_input* input = new data_input(buffer);

	//EXPECT_EQ(value,input->read_int64());
	//delete input;
}


}
int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
