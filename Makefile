COMPILER  = gcc
LINKS     =
CFLAGS    = -g -Wall -O2

BUILD_DIR = ./build
OBJ_DIR   = $(BUILD_DIR)/obj
SRC_DIR   = ./src


SOURCES   = $(wildcard $(SRC_DIR)/*.c)
OBJECTS   = $(addprefix $(OBJ_DIR)/, $(SOURCES:$(SRC_DIR)/%.c=%.o))

BIN       = ncp
TARGET    = $(BUILD_DIR)/$(BIN)
###############################################

$(TARGET): $(OBJECTS)
	$(COMPILER) $(CFLAGS) -o $@ $^ $(LINKS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) -o $@ -c $<

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)



