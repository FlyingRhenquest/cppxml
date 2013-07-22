
objs = run_cppunit.o kml_placemark_test.o kml_folder_test.o kml_point_test.o kml_coordinates_test.o kml_document_test.o kml_object_test.o engine_test.o xml_node_test.o node_engine_test.o
# Rebuild objects if any of these change
obj_deps = engine.hpp
TIME_HOME = ${HOME}/sandbox/time
COORDINATES_HOME = ${HOME}/sandbox/coordinates
CPPUNIT_CONTRIB=${HOME}/sandbox/cppunit_contrib
CPPFLAGS += --std=c++11 -I.. -I${CPPUNIT_CONTRIB} -I${TIME_HOME} -I${COORDINATES_HOME} -g -Wall -c
EXTERNAL_LIBS += -lcppunit -ldl -lexpat
executable = run_cppunit

%.o : %.cpp
	g++ ${CPPFLAGS} $<


all: ${objs}
	g++ -o ${executable} ${objs} ${EXTERNAL_LIBS}

clean:
	rm -f ${objs} core* *~ ${executable} 

