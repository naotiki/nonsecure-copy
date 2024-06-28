COMPILER  = gcc
LINKS     =
CFLAGS    = -g -Wall -O2

BUILD_DIR = ./build
OBJ_DIR   = $(BUILD_DIR)/obj
SRC_DIR   = ./src

TARGET    = $(BUILD_DIR)/ncp

SOURCES   = $(wildcard $(SRC_DIR)/*.c)
OBJECTS   = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

###############################################

$(TARGET): $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LINKS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)



