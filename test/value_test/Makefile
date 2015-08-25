default:all
.PHONY:default

all:test_value.o
	$(CC)  $(LIBPATH) -o test_value test_value.o  $(LIB)

test_value.o:test_value.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_value.o test_value.cc $(LIB)

clean:
	rm -rf *.o *.io
.PHONY:clean
