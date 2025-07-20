CC=gcc			
CFLAGS=-Wall -g

# Nome do executável
TARGET=mercearia

# Fontes do projeto
SRCS=mercearia.c estoque_funcoes.c util.c

# Regra padrão
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)