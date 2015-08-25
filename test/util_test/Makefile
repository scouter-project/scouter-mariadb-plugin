default:all
.PHONY:default
 
all:test_sleep.o  test_hashset.o test_util.o 
	$(CC)  $(LIBPATH) -o test_util test_util.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_hashset test_hashset.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_sleep test_sleep.o  $(LIB)

test_hashset.o:test_hashset.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_hashset.o test_hashset.cc $(LIB)
test_util.o:test_util.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_util.o test_util.cc $(LIB)
test_sleep.o:test_sleep.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_sleep.o test_sleep.cc $(LIB)
clean:
	rm -rf *.o test_util test_hashset test_sleep
.PHONY:clean
