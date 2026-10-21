# Algoritmos de Busca: Pesquisa Linear vs. Pesquisa Binária

Seminário da disciplina **Estrutura de Dados II**.

**Grupo:** Lucas Santana · Lucas Carvalho · Vitor · Samuel

Implementações próprias em C da busca linear e da busca binária (iterativa e recursiva), das variantes Lower Bound e Upper Bound e da busca binária no espaço de resposta. Cada algoritmo imprime o teste de mesa com a evolução do intervalo de busca (`esq`, `dir` e `meio`) a cada iteração.

## Estrutura

```text
src/
  buscas.h                   declarações de todas as funções
  trace.c                    impressão do teste de mesa no terminal
  busca_linear.c             busca linear
  busca_binaria_classica.c   busca binária iterativa e recursiva
  variantes_avancadas.c      lower_bound, upper_bound e busca no espaço de resposta
tests/
  teste_de_mesa_logger.c     imprime os testes de mesa dos dois exercícios
  testes_automatizados.c     confere os resultados de todas as buscas
docs/
  lista_de_exercicios.md     os dois exercícios com enunciado e resolução
Makefile
```

## Como compilar e rodar

Com `gcc` e `make` (Linux, macOS ou Windows com MSYS2/WSL):

```bash
make mesa      # compila e mostra os testes de mesa
make testes    # compila e roda os testes automatizados
```

Sem `make` (por exemplo, no Windows com MinGW):

```bash
gcc -Wall -Wextra -std=c99 -Isrc -o teste_de_mesa src/*.c tests/teste_de_mesa_logger.c
gcc -Wall -Wextra -std=c99 -Isrc -o testes_automatizados src/*.c tests/testes_automatizados.c
./teste_de_mesa
./testes_automatizados
```

### Exemplo de saída (Exercício 1, Parte A)

```text
Busca binaria iterativa
  passo |  esq |  dir | meio | valor | acao
  ------+------+------+------+-------+------------------------------------------
      1 |    0 |   10 |    5 |    40 | 40 < 63, esq = 6  -> [6, 10]
      2 |    6 |   10 |    8 |    63 | 63 == 63, encontrado
  => devolve o indice do meio
```

## Complexidade

| Algoritmo | Tempo (pior caso) | Memória extra | Exige ordenação? |
| --- | --- | --- | --- |
| Busca linear | O(n) | O(1) | Não |
| Busca binária iterativa | O(log n) | O(1) | Sim |
| Busca binária recursiva | O(log n) | O(log n), pilha de chamadas | Sim |
| Lower Bound / Upper Bound | O(log n) | O(1) | Sim |
| Busca no espaço de resposta | O(n · log S) | O(1) | Não, exige uma verificação monótona |

S é o tamanho do intervalo de respostas possíveis (no Exercício 2, de 50 a 150).

**Quando ordenar compensa:** ordenar custa O(n log n). Para uma única busca, a busca linear O(n) é mais barata. Se forem feitas k buscas nos mesmos dados, ordenar uma vez e usar a busca binária passa a compensar quando k fica maior que aproximadamente log n.

## Armadilhas evitadas no código

| Armadilha | O que acontece | Como o código evita |
| --- | --- | --- |
| Overflow no meio | `(esq + dir) / 2` estoura o `int` quando os índices passam de cerca de 1 bilhão e gera um índice negativo | `meio = esq + (dir - esq) / 2` em todas as funções |
| Erro de uma unidade | Com `esq < dir`, o último elemento restante nunca é testado | `while (esq <= dir)` com intervalo fechado |
| Loop infinito | Com `esq = meio` ou `dir = meio`, o intervalo pode parar de diminuir | `esq = meio + 1` e `dir = meio - 1` |

O programa `teste_de_mesa` demonstra o overflow com números reais e mostra o loop infinito acontecendo em uma versão de propósito errada (interrompida após 8 passos).

**Boas práticas:** os vetores de entrada usam `const`, então a busca nunca altera os dados consultados.

## Exercícios

A lista completa, com enunciados e testes de mesa resolvidos, está em [docs/lista_de_exercicios.md](docs/lista_de_exercicios.md).

1. **Busca clássica e armadilhas:** rastrear a busca de 63 e de 25 no vetor `[4, 9, 15, 22, 31, 40, 48, 57, 63, 79, 85]` e explicar o loop infinito causado por `dir = meio`.
2. **Busca no espaço de resposta:** menor capacidade diária para transferir os arquivos `[10, 20, 30, 40, 50]` em até 3 dias. Resposta: **60 GB por dia**, em 7 testes.

## Referências

- CORMEN, T. H. et al. *Algoritmos: Teoria e Prática*. Cap. 2.
- SEDGEWICK, R.; WAYNE, K. *Algorithms*. 4. ed. Seção 1.4.
- KNUTH, D. E. *The Art of Computer Programming*, Vol. 3. Seção 6.2.1.
- BENTLEY, J. *Programming Pearls*. 1986.
- BLOCH, J. *Extra, Extra: Read All About It: Nearly All Binary Searches and Mergesorts are Broken*. Google Research Blog, 2006.
- HALIM, S. *Competitive Programming 4*.
- LAAKSONEN, A. *Guide to Competitive Programming*. Springer. Cap. 3.
