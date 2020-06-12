#
# Makefile for csv-test project
#

TARGET=csv_test

OBJ=	csv_test.o \
	csv_common.o \
	dataset.o \
	emitter_json.o \
	ingestion_csv.o  \

HDR=	csv_spec.hh \
	csv_common.hh \
	dataset.hh \
	factory.hh \
	factory_impl.hh \
	emitter_iface.hh \
	emitter_factory_impl.hh \
	emitter_json.hh \
	ingestion_iface.hh \
	ingestion_factory_impl.hh \
	ingestion_csv.hh

.PHONY=clean

CXXFLAGS=-std=c++17 -ggdb

${TARGET}: ${OBJ} 
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJ}: ${HDR} 

clean:
	rm -f ${OBJ} ${TARGET}
