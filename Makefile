CC          := g++
FLAGS       := -std=c++20 -g

SRC_DIR     := src
INC_DIR     := include
OBJ_DIR     := build
OUT_DIR     := bin
SRC_DIR_TST := test/src
OBJ_DIR_TST := test/build
OUT_DIR_TST := test/bin

SRCS        := $(wildcard $(SRC_DIR)/*.cpp)
OBJS        := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HDRS        := $(wildcard $(INC_DIR)/*.hpp)
TSTS        := $(filter-out test/src/catch_amalgamated.cpp, $(wildcard $(SRC_DIR_TST)/*.cpp))
TST_OBJS    := $(TSTS:$(SRC_DIR_TST)/%.cpp=${OBJ_DIR_TST}/%.o)
TST_EXES    := $(TSTS:$(SRC_DIR_TST)/%.cpp=${OUT_DIR_TST}/%)

TST_EXECUTE_CMDS := $(TST_EXES:${OUT_DIR_TST}/%=./${OUT_DIR_TST}/%;)

all: directories ${OUT_DIR}/jckasm

${OBJ_DIR}/%.o: $(SRC_DIR)/%.cpp ${HDRS}
	${CC} ${FLAGS} -I$(INC_DIR) -c $< -o $@

${OUT_DIR}/jckasm: ${OBJS}
	${CC} ${FLAGS} -I$(INC_DIR) -o $@ $^

.PHONY: directories

directories: ${OUT_DIR} ${OBJ_DIR} ${OBJ_DIR_TST} ${OUT_DIR_TST}

${OUT_DIR}:
	mkdir -p ${OUT_DIR}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

${OBJ_DIR_TST}:
	mkdir -p ${OBJ_DIR_TST}

${OUT_DIR_TST}:
	mkdir -p ${OUT_DIR_TST}

clean:
	rm -r bin
	rm -r build

.PHONY: test

test: directories ${TST_EXES} ${OBJ_DIR_TST}/catch_amalgamated.o

${OBJ_DIR_TST}/catch_amalgamated.o: ${SRC_DIR_TST}/catch_amalgamated.cpp
	${CC} ${FLAGS} -I${INC_DIR} -I${SRC_DIR_TST} -c $< -o $@

${OBJ_DIR_TST}/%.o: ${SRC_DIR_TST}/%.cpp
	${CC} ${FLAGS} -I${INC_DIR} -I${SRC_DIR_TST} -c $< -o $@

${OUT_DIR_TST}/%: ${filter-out build/main.o, ${OBJS}} ${OBJ_DIR_TST}/%.o ${OBJ_DIR_TST}/catch_amalgamated.o
	${CC} ${FLAGS} -I${INC_DIR} -I${SRC_DIR_TST} -o $@ $^

test_run: test
	${TST_EXECUTE_CMDS}