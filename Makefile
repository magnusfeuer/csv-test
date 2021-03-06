#
# Makefile for csv-test project
#

TARGETS=csv_convert csv_convert_test

OBJ=	csv_common.o \
	record.o \
	specification.o \
	emitter_json.o \
	emitter_yaml.o \
	emitter_csv.o \
	ingestion_csv.o

HDR=	specification.hh \
	csv_common.hh \
	record.hh \
	factory.hh \
	factory_impl.hh \
	emitter_iface.hh \
	emitter_factory_impl.hh \
	emitter_json.hh \
	emitter_yaml.hh \
	emitter_csv.hh \
	ingestion_iface.hh \
	ingestion_factory_impl.hh \
	ingestion_csv.hh

.PHONY=clean all

CXXFLAGS=-std=c++17 -ggdb

all: ${TARGETS}

doc:
	doxygen Doxyfile
csv_convert: ${OBJ} csv_convert.o
	${CXX} ${CXXFLAGS} $^ -o $@

csv_convert_test: ${OBJ} csv_convert_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

${OBJ} csv_convert.o csv_convert_test.o: ${HDR} 

clean:
	rm -f ${OBJ} ${TARGETS} csv_convert.o csv_convert_test.o
	rm -rf html
