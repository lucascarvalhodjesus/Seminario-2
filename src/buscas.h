/*
 * buscas.h
 *
 * Seminario de Estrutura de Dados II: Algoritmos de Busca
 * Grupo: Lucas Santana, Lucas Carvalho, Vitor e Samuel
 *
 * Declaracoes de todas as funcoes de busca do projeto.
 *
 * Convencoes usadas em todo o codigo:
 *   - Vetores de entrada recebem o qualificador const: a busca nunca
 *     altera os dados que esta consultando.
 *   - O meio do intervalo e sempre calculado como esq + (dir - esq) / 2,
 *     que nunca estoura o limite de um int (ver README, secao Overflow).
 *   - Quando a variavel global busca_trace vale 1, as funcoes imprimem
 *     o teste de mesa (esq, dir, meio) a cada iteracao.
 */

#ifndef BUSCAS_H
#define BUSCAS_H

#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Controle do teste de mesa (definido em trace.c)                     */
/* ------------------------------------------------------------------ */

/* 0 = execucao silenciosa, 1 = imprime cada iteracao no terminal. */
extern int busca_trace;

void trace_cabecalho(const char *titulo);
void trace_linha(int passo, int esq, int dir, int meio, int valor, const char *acao);
void trace_rodape(const char *mensagem);

/* ------------------------------------------------------------------ */
/* busca_linear.c                                                      */
/* ------------------------------------------------------------------ */

/* Percorre o vetor do inicio ao fim. Nao exige ordenacao. O(n). */
int busca_linear(const int v[], int n, int alvo);

/* ------------------------------------------------------------------ */
/* busca_binaria_classica.c                                            */
/* ------------------------------------------------------------------ */

/* Versao iterativa: tempo O(log n), memoria extra O(1). */
int busca_binaria_iterativa(const int v[], int n, int alvo);

/* Versao recursiva: tempo O(log n), memoria extra O(log n) na pilha.
 * Chamada inicial: busca_binaria_recursiva(v, 0, n - 1, alvo). */
int busca_binaria_recursiva(const int v[], int esq, int dir, int alvo);

/* ------------------------------------------------------------------ */
/* variantes_avancadas.c                                               */
/* ------------------------------------------------------------------ */

/* Primeiro indice i com v[i] >= alvo. Devolve n se nao existir. */
int lower_bound(const int v[], int n, int alvo);

/* Primeiro indice i com v[i] > alvo. Devolve n se nao existir. */
int upper_bound(const int v[], int n, int alvo);

/* Quantas vezes alvo aparece no vetor ordenado, em O(log n). */
int contar_ocorrencias(const int v[], int n, int alvo);

/* Quantos dias sao necessarios para transferir os arquivos, em ordem,
 * com a capacidade diaria informada. Se lotes != NULL, escreve nele
 * a divisao por dia, por exemplo "[10+20+30] [40] [50]". */
int contar_dias(const int arq[], int n, int capacidade, char *lotes, size_t tam);

/* 1 se for possivel transferir tudo em ate k dias com essa capacidade. */
int eh_viavel(const int arq[], int n, int capacidade, int k);

/* Menor capacidade diaria que transfere todos os arquivos em ate k dias.
 * Busca binaria no espaco de resposta: O(n log S), onde S e o tamanho
 * do intervalo [maior arquivo, soma de todos]. */
int busca_espaco_resposta(const int arq[], int n, int k);

#endif
