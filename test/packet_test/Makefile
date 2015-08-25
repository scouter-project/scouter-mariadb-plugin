default:all
.PHONY:default

all:test_pk_status.o test_pk_text.o test_pk_object.o test_pk_table.o
	$(CC)  $(LIBPATH) -o test_pk_status test_pk_status.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_pk_text test_pk_text.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_pk_object test_pk_object.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_pk_table test_pk_table.o  $(LIB)

test_pk_status.o:test_pk_status.cc 
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_pk_status.o test_pk_status.cc $(LIB)
test_pk_text.o : test_pk_text.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_pk_text.o test_pk_text.cc $(LIB)
	
test_pk_object.o : test_pk_object.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_pk_object.o test_pk_object.cc $(LIB)

test_pk_table.o : test_pk_table.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_pk_table.o test_pk_table.cc $(LIB)


clean:
	rm -rf *.o *.io
.PHONY:clean
