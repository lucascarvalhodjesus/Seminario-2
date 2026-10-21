CC     = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -Isrc
SRC    = src/trace.c src/busca_linear.c src/busca_binaria_classica.c src/variantes_avancadas.c

all: teste_de_mesa testes_automatizados

teste_de_mesa: $(SRC) tests/teste_de_mesa_logger.c src/buscas.h
	$(CC) $(CFLAGS) -o $@ $(SRC) tests/teste_de_mesa_logger.c

testes_automatizados: $(SRC) tests/testes_automatizados.c src/buscas.h
	$(CC) $(CFLAGS) -o $@ $(SRC) tests/testes_automatizados.c

mesa: teste_de_mesa
	./teste_de_mesa

testes: testes_automatizados
	./testes_automatizados

clean:
	rm -f teste_de_mesa testes_automatizados teste_de_mesa.exe testes_automatizados.exe

.PHONY: all mesa testes clean
