# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra -g

# Nome do executável
TARGET = epidemia

# Diretório de saída para os arquivos objeto
OBJDIR = obj

# Lista de arquivos fonte
SOURCES = src/main.c src/populacao.c src/relacoes.c src/epidemia.c src/utils.c

# Lista de cabeçalhos
HEADERS = include/populacao.h include/relacoes.h include/epidemia.h include/utils.h

# Gera os nomes dos arquivos objeto
OBJECTS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Regra principal para construir o executável
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -lm -o $(TARGET)

# Regra para construir arquivos objeto a partir de arquivos fonte
$(OBJDIR)/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(OBJECTS) $(TARGET)

# Crie o diretório de objetos se não existir
$(shell mkdir -p $(OBJDIR))
