CHECK := $(shell g++ --version >/dev/null 2>&1 || (echo "Failed to search for g++ with error: $$"))
ifeq (,${CHECK})
    COMPILER := g++
$(info C++ compiler found: g++)
$(info )

else # Check for clang if g++ is unavailable
    CHECK2 := $(shell clang --version >/dev/null 2>&1 || (echo "Failed to search for clang with error: $$?"))
	ifeq (,${CHECK2})
$(info C++ compiler found: clang)
$(info )
		COMPILER := clang
		LIBS := -lstdc++
	else
$(error No C++ compilers found.)
	endif
endif

FLAGS := -O3 -DNDEBUG
LIBS  := -lm -lpopt -lglut -lGLU -lGL -fopenmp -lpthread ${LIBS}
BUILD := build
SRC := src
OBJ := ${BUILD}/obj
SHARED_OBJ := ${BUILD}/shared
BIN := ${BUILD}/bin

EXECUTABLE := ${BIN}/polygonGenerator
SHARED_LIBRARY := ${BIN}/polygonGenerator.so

SRCS := $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SHARED_OBJS := $(patsubst $(SRC)/%.cpp, $(SHARED_OBJ)/%.o, $(SRCS))
SUBDIRECTORIES := $(sort $(dir $(wildcard $(SRC)/*/)))
DIRECTORIES := $(patsubst $(SRC)/%, $(SHARED_OBJ)/%, $(SUBDIRECTORIES)) $(patsubst $(SRC)/%, $(OBJ)/%, $(SUBDIRECTORIES)) ${BIN}

$(shell mkdir -p ${DIRECTORIES})

polygonGenerator: ${OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS}
	@echo
	@echo "Compiled Successfully!! Run the program using ${EXECUTABLE}"

debug: FLAGS := -g -Wall
debug: ${OBJS}
	${COMPILER} ${FLAGS} -o ${EXECUTABLE} $^ ${LIBS}
	@echo
	@echo "Compiled Successfully!! Run the program using ${EXECUTABLE}"

polygonGenerator_SharedLibrary: FLAGS := ${FLAGS} -shared -fPIC -pie
polygonGenerator_SharedLibrary: ${SHARED_OBJS}
	${COMPILER} ${FLAGS} -o ${SHARED_LIBRARY} $^ ${LIBS}
	@echo
	@echo "Compiled Successfully!! The Shared Library is available as" ${SHARED_LIBRARY}

${OBJ}/%.o: ${SRC}/%.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@ 

${SHARED_OBJ}/%.o: ${SRC}/%.cpp
	${COMPILER} ${FLAGS} -c $^ -o $@

clean:
	rm -rf ${OBJ} ${SHARED_OBJ} ${BIN}