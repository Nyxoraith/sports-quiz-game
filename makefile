ifeq ($(OS),Windows_NT)
	INCLUDE = -I ./raylib/include -L ./raylib/libwin
	EXTRA_FLAGS = -Wall -Werror -Wextra -std=c99 -Wno-missing-braces -lraylib -lm -lopengl32 -lgdi32 -lwinmm -mwindows
	BIN = game.exe
	RM = del /Q
else
	INCLUDE = -I ./raylib/include -L ./raylib/lib
	EXTRA_FLAGS = -Wall -Werror -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	BIN = ./game
	RM = rm -f
endif

SRC = src/*.c

all:
	gcc $(SRC) -g -lm $(EXTRA_FLAGS) $(INCLUDE) -o $(BIN)

run:
	$(BIN)

clean:
	rm -f $(BIN)