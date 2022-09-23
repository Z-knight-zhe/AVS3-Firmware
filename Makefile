#INCLUDE := -Iinc
#SRC_PATH = src
#SRC = $(wildcard $(SRC_PATH)/*.c)
#OBJ = $(SRC:%.c=%.o)
INCLUDE := -I./
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

CC := gcc

CFLAGS := -Wall -g $(INCLUDE) 

TARGET := avs

target: $(OBJ)
	@echo $(OBJ)
	$(CC) -o $(TARGET) $(notdir $(OBJ)) -lm
	
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)
	
.PHONY:clean
clean:
	-rm -rf $(notdir $(OBJ)) $(TARGET)