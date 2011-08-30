#
# This makefile builds TestSPI_A2D project
# The target platform is SBOS running on SandBlaster DSP
#

# Executable file name
TARGET_FILE=testspi_a2d.sbx

# Source files; files of the following types are supported: *.c, *.s  
SOURCE_FILES= \
	main.c sbxspi.c

# All intermediate files created while building this project 
# are stored in the following directory: 
BUILD_DIR=build

# Location of SandBlaster C compiler, linker and other tools
CC="$(SBROOT)/bin/sbcc"
AS="$(SBROOT)/bin/sb-as"
LD="$(SBROOT)/bin/sbld"
AR="$(SBROOT)/bin/sb-ar"

# C compiler flags
CFLAGS=--march=sb3500 -DTX linker.at

ifeq "$(DEBUG)" "yes"
CFLAGS+=-g -O0 
endif

# All intermediate object files are stored to the $(BUILD_DIR) directory
OBJECT_FILES= $(addprefix $(BUILD_DIR)/, \
	$(subst .c,.o,$(subst .s,.o,$(subst .SS,.o,$(SOURCE_FILES)))))

all: $(BUILD_DIR) $(TARGET_FILE)

clean :
	-rm -r -f $(TARGET_FILE) $(OBJECT_FILES) $(BUILD_DIR)

$(BUILD_DIR):
	-mkdir $(BUILD_DIR)
	
$(TARGET_FILE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.s
	$(AS) -o $@ $<
