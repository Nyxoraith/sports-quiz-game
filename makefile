# Variáveis
CC = gcc
CFLAGS = -I C:/raylib/raylib/include
LDFLAGS = -L C:/raylib/raylib/lib -lraylib -lwinmm -lgdi32 -lopengl32
SRC = main.c functions.c
OBJ = $(SRC:.c=.o)
TARGET = game.exe

# Regra padrão
all: $(TARGET)

# Regra para compilar o programa
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Regra para limpar os arquivos gerados
clean:
	del /Q $(TARGET) *.o 2>nul || exit 0
