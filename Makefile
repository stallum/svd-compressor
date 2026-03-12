# Variáveis de diretórios
BIN_DIR = bin
OBJ_DIR = build
SRC_DIR = src

# Variáveis de arquivos
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/compressor.exe  #.exe só no windows, no linux tem que tirar
# cd /c/Users/user/Documents/GitHub/compressor  para achar o caminho no ucrt(windows)
# GSK_RENDERER=cairo  mudar o renderezidor, se n mudar da erro

# Configurações da GSL, GTK4 e Compilador
GSL_FLAGS = $(shell gsl-config --cflags)
GSL_LIBS  = $(shell gsl-config --libs | sed 's/-lgslcblas//')
GTK_CFLAGS = $(shell pkg-config --cflags gtk4)
GTK_LIBS   = $(shell pkg-config --libs gtk4)

CC = gcc

CFLAGS = -Wall -Wextra -O3 -march=native -fopenmp -g -Iinclude $(GSL_FLAGS) $(GTK_CFLAGS)

LDFLAGS = -lopenblas $(GSL_LIBS) -ljpeg -lpng -lpthread -lm -fopenmp $(GTK_LIBS)

# Regra principal
all: $(TARGET)

# Linkagem do executável final
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compilação dos objetos (.o)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) testar image_io

# Regra para rodar os testes
testar: testar.c $(SRC_DIR)/matrix.c
	$(CC) $(CFLAGS) testar.c $(SRC_DIR)/matrix.c -o testar $(LDFLAGS)