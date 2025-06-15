# Variavel de configuração do compilador e linker
CC = gcc
CFLAGS = -I C:/raylib/raylib/include   # Flags para compilar (.c -> .o)
LDFLAGS = -L C:/raylib/raylib/lib -lraylib -lwinmm -lgdi32 -lopengl32  # Flags para linkar (.o -> .exe)

# Diretorios
SRC_DIR = src

# Variaveis para listar arquivos fontes e obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)
TARGET = game.exe

# Regra padrão
all: $(TARGET)

# Linkagem do programa .o -> .exe
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compila cada arquivo fonte em um objeto .c -> .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

run:
	.\game.exe