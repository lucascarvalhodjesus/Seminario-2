# Lista de Exercícios Resolvidos

**Disciplina:** Estrutura de Dados II · **Tema:** Algoritmos de Busca

**Grupo:** Lucas Santana · Lucas Carvalho · Vitor · Samuel

Os testes de mesa abaixo são impressos exatamente assim pelo programa `teste_de_mesa` (`make mesa`).

---

## Exercício 1: Busca clássica e armadilhas de implementação

### Enunciado

Considere o vetor ordenado de 11 posições:

| Índice | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Valor | 4 | 9 | 15 | 22 | 31 | 40 | 48 | 57 | 63 | 79 | 85 |

- **Parte A:** faça o teste de mesa completo da busca binária para encontrar o valor 63.
- **Parte B:** faça o teste de mesa para o valor 25, que não está no vetor. Explique o que aconteceria se o programador tivesse escrito `dir = meio` no lugar de `dir = meio - 1`.

### Resolução da Parte A (alvo = 63)

| Passo | esq | dir | meio | V[meio] | Decisão | Novo intervalo |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 0 | 10 | 0 + (10 - 0) / 2 = 5 | 40 | 40 < 63, então esq = 6 | [6, 10] |
| 2 | 6 | 10 | 6 + (10 - 6) / 2 = 8 | 63 | 63 = 63, encontrado | devolve 8 |

**Resultado:** índice 8, com apenas 2 comparações. A busca linear precisaria de 9.

### Resolução da Parte B (alvo = 25)

| Passo | esq | dir | meio | V[meio] | Decisão | Novo intervalo |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 0 | 10 | 5 | 40 | 40 > 25, então dir = 4 | [0, 4] |
| 2 | 0 | 4 | 2 | 15 | 15 < 25, então esq = 3 | [3, 4] |
| 3 | 3 | 4 | 3 | 22 | 22 < 25, então esq = 4 | [4, 4] |
| 4 | 4 | 4 | 4 | 31 | 31 > 25, então dir = 3 | vazio |
| Fim | 4 | 3 | | | esq > dir, o laço termina | devolve -1 |

O intervalo encolheu de 11 elementos para 5, depois 2, depois 1 e, por fim, ficou vazio.

**O erro `dir = meio`:** com esse erro, o caminho muda um pouco, mas o intervalo também chega em [4, 4]:

| Passo | esq | dir | meio | V[meio] | Decisão com o bug |
| --- | --- | --- | --- | --- | --- |
| 1 | 0 | 10 | 5 | 40 | dir = meio = 5 |
| 2 | 0 | 5 | 2 | 15 | esq = 3 |
| 3 | 3 | 5 | 4 | 31 | dir = meio = 4 |
| 4 | 3 | 4 | 3 | 22 | esq = 4 |
| 5 | 4 | 4 | 4 | 31 | dir = meio = 4 |
| 6 | 4 | 4 | 4 | 31 | dir = meio = 4 |
| ... | 4 | 4 | 4 | 31 | repete para sempre |

Em [4, 4], meio = 4 e V[4] = 31 é maior que 25, então o código faz dir = 4 de novo. Nada muda: esq continua 4, dir continua 4, e a condição `esq <= dir` nunca deixa de ser verdadeira. É um **loop infinito**. A correção é `dir = meio - 1`: o elemento do meio já foi testado e precisa sair do intervalo.

---

## Exercício 2: Busca binária no espaço de resposta

### Enunciado

Um sistema de backup precisa transferir 5 arquivos com tamanhos, em GB, `[10, 20, 30, 40, 50]`. A ordem não pode ser alterada, nenhum arquivo pode ser dividido e tudo deve ser transferido em no máximo **3 dias**. Qual é a **menor capacidade diária** do link que cumpre o prazo?

### Ideia

Não procuramos um elemento em um vetor. Procuramos a melhor resposta dentro do intervalo de capacidades possíveis, usando uma função de verificação:

- `eh_viavel(capacidade)`: enche cada dia com os arquivos, em ordem, e abre um novo dia quando o próximo não cabe. Responde se foram usados até 3 dias. Custo O(n).
- A verificação é **monótona**: se uma capacidade funciona, qualquer capacidade maior também funciona. Por isso a busca binária pode ser aplicada.

### Fronteiras do espaço de resposta

- **esq = 50:** o maior arquivo precisa caber em um único dia.
- **dir = 150:** a soma de todos (10 + 20 + 30 + 40 + 50) caberia em um único dia.

Intervalo inicial: **[50, 150]**, com 101 capacidades possíveis.

### Resolução

| Passo | esq | dir | meio | Divisão por dia | Dias | Resultado |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | 50 | 150 | 100 | [10+20+30+40] [50] | 2 | viável: resp = 100, dir = 99 |
| 2 | 50 | 99 | 74 | [10+20+30] [40] [50] | 3 | viável: resp = 74, dir = 73 |
| 3 | 50 | 73 | 61 | [10+20+30] [40] [50] | 3 | viável: resp = 61, dir = 60 |
| 4 | 50 | 60 | 55 | [10+20] [30] [40] [50] | 4 | inviável: esq = 56 |
| 5 | 56 | 60 | 58 | [10+20] [30] [40] [50] | 4 | inviável: esq = 59 |
| 6 | 59 | 60 | 59 | [10+20] [30] [40] [50] | 4 | inviável: esq = 60 |
| 7 | 60 | 60 | 60 | [10+20+30] [40] [50] | 3 | viável: resp = 60, dir = 59 |

O laço termina quando esq (60) passa de dir (59).

**Resposta: 60 GB por dia.** A busca binária encontrou o ponto de virada, entre 59 GB (falha) e 60 GB (funciona), em 7 testes. Uma varredura linear pode precisar de até 101 testes, dependendo de onde a resposta estiver.

**Complexidade:** O(n · log S), em que n = 5 arquivos e S = 101 capacidades possíveis.
