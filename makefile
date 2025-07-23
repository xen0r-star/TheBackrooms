CC = gcc
WINDRES = windres

SRC_DIR = src
BUILD_DIR = build
MODULES_DIR = $(SRC_DIR)/modules
PUBLIC_DIR = public

# SDL2 installée via pacman (dans /mingw64)
CFLAGS = -I/mingw64/include/SDL2 -Isrc/modules -Wall -Wextra -O2
LDFLAGS = -L/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# Recherche tous les .c dans les bons dossiers
SRCS = $(wildcard $(MODULES_DIR)/**/*.c $(MODULES_DIR)/*.c $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

RES = $(BUILD_DIR)/resource.o
TARGET = $(BUILD_DIR)/TheBackrooms.exe

all: $(TARGET)

$(TARGET): $(OBJS) $(RES)
	@echo Linking object files to create the executable: $(TARGET)
	$(CC) -o $@ $(OBJS) $(RES) $(LDFLAGS)
	@echo Executable created at: $(TARGET)

# Compilation des .c en .o (crée aussi les dossiers intermédiaires si nécessaires)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $< to $@
	$(CC) $(CFLAGS) -c $< -o $@

# Resource file
$(RES): $(SRC_DIR)/resources/resource.rc
	@mkdir -p $(dir $@)
	$(WINDRES) $< -O coff -o $@

# Copie des fichiers publics (assets, etc.)
dev: all
	@mkdir -p $(BUILD_DIR)
	@cp -r $(PUBLIC_DIR)/* $(BUILD_DIR)/

clean:
	rm -rf $(BUILD_DIR)

debug:
	@echo "Sources: $(SRCS)"
	@echo "Objects: $(OBJS)"

.PHONY: all clean dev debug
