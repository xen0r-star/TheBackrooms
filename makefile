# Platform detection and configuration
# Usage: make PLATFORM=windows or make PLATFORM=linux
# Default to current OS if not specified
UNAME_S := $(shell uname -s)
ifeq ($(PLATFORM),)
    ifeq ($(UNAME_S),Linux)
        PLATFORM = linux
    else
        PLATFORM = windows
    endif
endif

CC = gcc
WINDRES = windres

SRC_DIR = src
BUILD_DIR = build
MODULES_DIR = $(SRC_DIR)/modules
PUBLIC_DIR = public

# Platform-specific configuration
ifeq ($(PLATFORM),windows)
    # Windows (MinGW) configuration
    CFLAGS = -I/mingw64/include/SDL2 -Isrc/modules -Wall -Wextra -O2
    LDFLAGS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
    TARGET_EXT = .exe
    RES_OBJ = $(BUILD_DIR)/resource.o
else ifeq ($(PLATFORM),linux)
    # Linux configuration
    CFLAGS = $(shell pkg-config --cflags sdl2) -Isrc/modules -Wall -Wextra -O2
    LDFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm
    TARGET_EXT = 
    RES_OBJ = 
else
    $(error Unsupported platform: $(PLATFORM). Use 'windows' or 'linux')
endif

# Recherche tous les .c dans les bons dossiers
SRCS = $(wildcard $(MODULES_DIR)/**/*.c $(MODULES_DIR)/*.c $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/TheBackrooms$(TARGET_EXT)

all: $(TARGET)

$(TARGET): $(OBJS) $(RES_OBJ)
	@echo "Building for $(PLATFORM)..."
	@echo "Linking object files to create the executable: $(TARGET)"
	$(CC) -o $@ $(OBJS) $(RES_OBJ) $(LDFLAGS)
	@echo "Executable created at: $(TARGET)"

# Compilation des .c en .o (crée aussi les dossiers intermédiaires si nécessaires)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $< to $@
	$(CC) $(CFLAGS) -c $< -o $@

# Resource file (Windows only)
ifeq ($(PLATFORM),windows)
$(BUILD_DIR)/resource.o: $(SRC_DIR)/resources/resource.rc
	@mkdir -p $(dir $@)
	@echo "Compiling Windows resource file: $<"
	$(WINDRES) $< -O coff -o $@
endif

# Platform-specific targets
windows:
	$(MAKE) PLATFORM=windows

linux:
	$(MAKE) PLATFORM=linux

# Development build (copies assets)
dev: all
	@mkdir -p $(BUILD_DIR)
	@cp -r $(PUBLIC_DIR)/* $(BUILD_DIR)/

clean:
	rm -rf $(BUILD_DIR)

debug:
	@echo "Platform: $(PLATFORM)"
	@echo "Target: $(TARGET)"
	@echo "Sources: $(SRCS)"
	@echo "Objects: $(OBJS)"
	@echo "Resource Object: $(RES_OBJ)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"

help:
	@echo "TheBackrooms Build System"
	@echo "========================="
	@echo "Usage:"
	@echo "  make                    - Build for current platform (auto-detected)"
	@echo "  make windows           - Build for Windows"
	@echo "  make linux             - Build for Linux"
	@echo "  make PLATFORM=windows  - Explicit Windows build"
	@echo "  make PLATFORM=linux    - Explicit Linux build"
	@echo "  make dev               - Build and copy assets"
	@echo "  make clean             - Clean build directory"
	@echo "  make debug             - Show build configuration"
	@echo ""
	@echo "Requirements:"
	@echo "  Windows: MinGW with SDL2 installed in /mingw64"
	@echo "  Linux:   sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev"

.PHONY: all clean dev debug windows linux help
