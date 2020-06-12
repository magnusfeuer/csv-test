#
# Makefile for csv-test project
#

TARGETS=csv_convert csv_convert_test

OBJ=	csv_common.o \
	dataset.o \
	csv_spec.o \
	emitter_json.o \
	ingestion_csv.o

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

.PHONY=clean all

CXXFLAGS=-std=c++17 -ggdb

all: ${TARGETS}

csv_convert: ${OBJ} csv_convert.o
	${CXX} ${CXXFLAGS} $^ -o $@

csv_convert_test: ${OBJ} csv_convert_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJ}: ${HDR} 

clean:
	rm -f ${OBJ} ${TARGET}
