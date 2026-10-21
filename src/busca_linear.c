/*
 * busca_linear.c
 *
 * Busca Linear (sequencial).
 *
 * Ideia: olhar um elemento de cada vez, do indice 0 ate o fim, ate
 * encontrar o alvo ou esgotar o vetor.
 *
 * Complexidade de tempo:
 *   - Melhor caso: O(1)   (o alvo e o primeiro elemento)
 *   - Caso medio:  O(n)
 *   - Pior caso:   O(n)   (o alvo e o ultimo ou nao existe)
 * Memoria extra: O(1).
 *
 * Pre-condicao: nenhuma. Funciona com dados fora de ordem.
 */

#include <stdio.h>
#include "buscas.h"

int busca_linear(const int v[], int n, int alvo) {
    char acao[96];

    trace_cabecalho("Busca linear");

    for (int i = 0; i < n; i++) {
        /* Cada posicao e comparada uma unica vez. */
        if (v[i] == alvo) {
            snprintf(acao, sizeof acao, "%d == %d, encontrado", v[i], alvo);
            trace_linha(i + 1, i, n - 1, i, v[i], acao);
            trace_rodape("devolve o indice encontrado");
            return i;
        }
        snprintf(acao, sizeof acao, "%d != %d, avanca", v[i], alvo);
        trace_linha(i + 1, i, n - 1, i, v[i], acao);
    }

    trace_rodape("fim do vetor, devolve -1");
    return -1;
}
