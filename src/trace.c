/*
 * trace.c
 *
 * Funcoes de apoio para imprimir o teste de mesa no terminal.
 * Elas so imprimem algo quando busca_trace vale 1, assim as buscas
 * continuam silenciosas nos testes automatizados.
 */

#include <stdio.h>
#include "buscas.h"

int busca_trace = 0;

void trace_cabecalho(const char *titulo) {
    if (!busca_trace) return;
    printf("\n%s\n", titulo);
    printf("  passo |  esq |  dir | meio | valor | acao\n");
    printf("  ------+------+------+------+-------+------------------------------------------\n");
}

void trace_linha(int passo, int esq, int dir, int meio, int valor, const char *acao) {
    if (!busca_trace) return;
    printf("  %5d | %4d | %4d | %4d | %5d | %s\n", passo, esq, dir, meio, valor, acao);
}

void trace_rodape(const char *mensagem) {
    if (!busca_trace) return;
    printf("  => %s\n", mensagem);
}
