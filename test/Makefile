objs = run_cppunit.o engine_test.o xml_node_test.o node_engine_test.o
# Rebuild objects if any of these change
obj_deps = engine.hpp
CPPFLAGS += --std=c++11 -I.. -g -Wall -c
EXTERNAL_LIBS += -lcppunit -ldl -lexpat
executable = run_cppunit

%.o : %.cpp
	g++ ${CPPFLAGS} $<


all: ${objs}
	g++ -o ${executable} ${objs} ${EXTERNAL_LIBS}

clean:
	rm -f ${objs} core* *~ ${executable} 

