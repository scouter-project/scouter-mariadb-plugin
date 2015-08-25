default:all
.PHONY:default

all:test_udp_cli.o test_tcp.o
	$(CC)  $(LIBPATH) -o test_udp_cli test_udp_cli.o  $(LIB)
	$(CC)  $(LIBPATH) -o test_tcp test_tcp.o  $(LIB)

test_udp_cli.o:test_udp_cli.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_udp_cli.o test_udp_cli.cc $(LIB)

test_tcp.o:test_tcp.cc
	$(CC) $(INCLUDE) $(LIBPATH) $(FLAGS) test_tcp.o test_tcp.cc $(LIB)
clean:
	rm -rf *.o test_udp_cli
.PHONY:clean
