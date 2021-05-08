COMPILER := g++
FLAGS := -O3
SRC := src
OBJ := obj
SHARED_OBJ := shared
BIN := bin
EXECUTABLE := ${BIN}/polygonGenerator
SHARED_LIBRARY := ${BIN}/polygonGenerator.so
LIBS := -lm -lpopt
OBJS := ${OBJ}/WKT_writer.o ${OBJ}/Algorithms.o ${OBJ}/Driver.o ${OBJ}/Polygon.o ${OBJ}/space_partition.o 
SHARED_OBJS = ${SHARED_OBJ}/WKT_writer.o ${SHARED_OBJ}/Algorithms.o

$(shell mkdir -p ${BIN} ${OBJ} ${SHARED_OBJ})

ifeq ($(shared), 1)
	FLAGS := ${FLAGS} -shared --compiler-options="-fPIC -pie"
endif

ifeq ($(debug), 1)
	FLAGS := ${FLAGS} -g
endif

polygonGenerator: ${OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS} && echo "Compiled Successfully!! Run the program using ./bin/polygonGenerator"

polygonGenerator_SharedLibrary: ${SHARED_OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS} && echo "Compiled Successfully!!"

%/WKT_writer.o: ${SRC}/WKT_writer.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

%/Algorithms.o: ${SRC}/Algorithms.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

%/Driver.o: ${SRC}/Driver.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

%/Polygon.o: ${SRC}/Polygon.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

%/space_partition.o: ${SRC}/space_partition.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

clean:
	rm -rf ${OBJ} ${SHARED_OBJ} ${BIN}