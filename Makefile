COMPILER := g++

FLAGS := -O3 -Wall -DNDEBUG
LIBS := -lm -lpopt -lglut -lGLU -lGL

SRC := src
OBJ := obj
SHARED_OBJ := shared
BIN := bin

EXECUTABLE := ${BIN}/polygonGenerator
SHARED_LIBRARY := ${BIN}/polygonGenerator.so

SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SHARED_OBJS = $(patsubst $(SRC)/%.cpp, $(SHARED_OBJ)/%.o, $(SRCS))

$(shell mkdir -p ${BIN} ${OBJ} ${SHARED_OBJ})

polygonGenerator: ${OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS} && echo "Compiled Successfully!! Run the program using ./bin/polygonGenerator"

debug: FLAGS := -g -Wall
debug: ${OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS} && echo "Compiled Successfully!! Run the program using ./bin/polygonGenerator"

polygonGenerator_SharedLibrary: FLAGS := ${FLAGS} -shared --compiler-options="-fPIC -pie"
polygonGenerator_SharedLibrary: ${SHARED_OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS} && echo "Compiled Successfully!!"

${OBJ}/%.o: ${SRC}/%.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS} 

${SHARED_OBJ}/%.o: ${SRC}/%.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ ${LIBS}

clean:
	rm -rf ${OBJ} ${SHARED_OBJ} ${BIN}