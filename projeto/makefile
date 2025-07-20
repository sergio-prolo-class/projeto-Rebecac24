CC=gcc			
CFLAGS=-Wall -g
                                                    
TARGET=mercearia

SRCS=mercearia.c estoque_funcoes.c util.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)