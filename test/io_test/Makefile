default:all
.PHONY:default

all:test_data_input_output.o
	$(CC)  $(LIBPATH) -o test.io test_data_input_output.o  $(LIB)

test_data_input_output.o:test_data_input_output.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_data_input_output.o test_data_input_output.cc $(LIB)

clean:
	rm -rf *.o *.io
.PHONY:clean
