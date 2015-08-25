export INCLUDE=-I/home/windfree/workspace/ws-mariadb/spotter/src/include \
        -I/home/windfree/workspace/gtest-1.7.0/include \
        -I/home/windfree/workspace/ws-mariadb/mariadb-10.0.20/sql 
export FLAGS= -m64 -c -o 
export LIBPATH=-L/home/windfree/workspace/gtest-1.7.0/output \
    -L/home/windfree/workspace/ws-mariadb/mariadb-10.0.20/plugin/spotter/src \
    -L/home/windfree/workspace/ws-mariadb/mariadb-10.0.20/libmysqld 
export LIB=-lgtest -lpthread -lspotter -lmysqld
export CC=g++ -g
#DIRS=io_test net_test value_test packet_test util_test
DIRS=io_test util_test net_test value_test packet_test
default:all
.PHONY:default
all:
	@for dir in $(DIRS) ; do \
	make -C $$dir || exit $?; \
	done
	
clean:
	
	@for dir in $(DIRS) ; do \
	make -C $$dir clean ; \
	done
