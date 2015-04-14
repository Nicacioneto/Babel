NAME = voidcrawlers.a

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
LIB_DIR = lib
BIN_DIR = bin

CC = g++
AR = ar

ARFLAGS = rcs
CFLAGS = -W -Wall -pedantic -ansi -std=c++11 -MMD -g3
INCLUDES = -Iinclude `sdl2-config --cflags`
LIBS = `sdl2-config --libs` lib/ijengine.a

TARGET = $(LIB_DIR)/$(NAME)

GAME = $(BIN_DIR)/voidcrawlers
GAME_MAIN = $(SRC_DIR)/*.cpp

SRC = ${wildcard $(SRC_DIR)/*.cpp}
OBJ = ${addprefix $(OBJ_DIR)/, ${notdir ${SRC:.cpp=.o}}} 

.PHONY: clean depend

all: 
	@mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
	@make $(TARGET)
	@make $(GAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

$(GAME): $(GAME_MAIN) $(TARGET)
	$(CC) $(CFLAGS) $(INCLUDES) $(GAME_MAIN) $(TARGET) -o $@ $(LIBS)

clean:
	@find . -name *.o -exec rm {} \;
	@find . -name *.d -exec rm {} \;
	@rm -rf *~ *.o bin out.txt obj lib/voidcrawlers.a

-include $(OBJ:.o=.d)
