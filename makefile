CC = gcc
CXX = g++

#MYFLAGS = -MMD -g -Wall -pthread
MYFLAGS = -g -Wall -O2 -MMD
INC_DIR = -I ./include/
LIB = 

TAR_DIR = ./bin/
SRC_DIR = ./src/
OBJ_DIR = ./obj/

TARGET = $(TAR_DIR)shmhashtest

OBJS = \
	   $(OBJ_DIR)main.o \
	   $(OBJ_DIR)shm_hash.o \
	   $(OBJ_DIR)util.o \
	   $(OBJ_DIR)gene_hashalg.o \

OBJ_SUB_DIRS = $(sort $(dir $(OBJS)))

DEPENDS = $(patsubst %.o, %.d, $(OBJS))

.PHONY: all clean init 


#compile start
all: init $(TARGET) $(T1)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIB) $(MYFLAGS)

$(T1): $(OBJS1)
	$(CXX) -o $@ $^ $(LIB) $(MYFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(MYFLAGS) -c -o $@ $(INC_DIR) $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(MYFLAGS) -c -o $@ $(INC_DIR) $<

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CXX) $(MYFLAGS) -c -o $@ $(INC_DIR) $<

clean:
	rm -f $(OBJS) $(TARGET) $(DEPENDS)

init:
	mkdir -p $(sort ./ $(TAR_DIR) $(SRC_DIR) $(OBJ_DIR) $(OBJ_SUB_DIRS))

-include $(DEPENDS)
