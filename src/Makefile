CC=g++ -g
DEF= -DNO_EMBEDDED_ACCESS_CHECKS -DMYSQL_DYNAMIC_PLUGIN
SRCS = $(wildcard *.cc task/*.cc util/*.cc io/*.cc data_pack/*.cc net/*.cc value/*.cc config/*.cc manager/*.cc)
OBJS = $(SRCS:.cc=.o)
LIB=
INC=-I/home/windfree/workspace/ws-mariadb/mariadb-10.0.20/include \
	-I/home/windfree/workspace/ws-mariadb/mariadb-10.0.20/sql \
	-I./include


.SUFFIXES : .o .cc
.cc.o:
	$(CC) $(INC) -fPIC -O2 -Wall  -c   $<

.phony : spotter
all:spotter
	$(CC) $(INC) -shared -Wl,-soname libspotter.so $(OBJS)
.phony : spotter
spotter: $(OBJS) 
	$(CC) $(INC) -fPIC -O2 -Wl -Wall $@ $^ 

.phony : clean
clean:
	@echo $(SRCS)
	@echo $(INC)
	@echo $(CC)
	
