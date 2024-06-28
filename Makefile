TARGET = ./build/NonSecureCopy

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj

SOURCES  = $(wildcard *.c)
OBJECTS  = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))

$(TARGET): $(OBJECTS)
	gcc -o $@ $^

$(OBJ_DIR)/%.o: %.c
	#@[ -d $(OBJ_DIR) ]
	@mkdir -p $(dir $@)
	gcc -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)



