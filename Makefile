CC        = g++
FLAGS     = -std=c++20 -g

SRC_DIR   = src
INC_DIR   = include
OBJ_DIR   = build
OUT_DIR   = bin

SRCS      = $(wildcard $(SRC_DIR)/*.cpp)
OBJS      = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
HDRS      = $(wildcard $(INC_DIR)/*.hpp)

all: directories ${OUT_DIR}/jckasm

${OBJ_DIR}/%.o: $(SRC_DIR)/%.cpp ${HDRS}
	${CC} ${FLAGS} -I$(INC_DIR) -c $< -o $@

${OUT_DIR}/jckasm: ${OBJS}
	${CC} ${FLAGS} -I$(INC_DIR) -o $@ $^

.PHONY: directories

directories: ${OUT_DIR} ${OBJ_DIR} 

${OUT_DIR}:
	mkdir -p ${OUT_DIR}

${OBJ_DIR}:
	mkdir -p ${OBJ_DIR}

clean:
	rm -r bin
	rm -r build