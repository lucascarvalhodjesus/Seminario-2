/*
 * busca_binaria_classica.c
 *
 * Busca Binaria classica, nas versoes iterativa e recursiva.
 *
 * Pre-condicoes:
 *   1. O vetor esta ordenado em ordem crescente.
 *   2. Acesso direto a qualquer posicao em O(1) (vetor contiguo).
 *
 * Invariante: se o alvo existe no vetor, ele esta dentro do intervalo
 * fechado [esq, dir]. A cada passo comparamos com o elemento do meio e
 * descartamos a metade onde o alvo nao pode estar.
 *
 * Tres cuidados que evitam os bugs classicos:
 *   1. while (esq <= dir): quando esq == dir ainda sobra um elemento,
 *      e ele precisa ser testado (com esq < dir ele seria ignorado).
 *   2. meio = esq + (dir - esq) / 2: a forma (esq + dir) / 2 pode
 *      estourar o limite do int quando os indices sao grandes.
 *   3. esq = meio + 1 e dir = meio - 1: o meio ja foi testado; se ele
 *      ficar no intervalo, o intervalo pode parar de diminuir e o laco
 *      nunca termina.
 */

#include <stdio.h>
#include "buscas.h"

/* ------------------------------------------------------------------ */
/* Versao iterativa                                                    */
/* Tempo O(log n), memoria extra O(1): so usa esq, dir e meio.         */
/* ------------------------------------------------------------------ */
int busca_binaria_iterativa(const int v[], int n, int alvo) {
    int esq = 0;
    int dir = n - 1;              /* intervalo fechado [esq, dir] */
    int passo = 0;
    char acao[96];

    trace_cabecalho("Busca binaria iterativa");

    while (esq <= dir) {                      /* inclui o caso esq == dir */
        int meio = esq + (dir - esq) / 2;     /* calculo seguro do meio   */
        passo++;

        if (v[meio] == alvo) {
            snprintf(acao, sizeof acao, "%d == %d, encontrado", v[meio], alvo);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            trace_rodape("devolve o indice do meio");
            return meio;
        }

        if (v[meio] < alvo) {
            /* O alvo e maior: descarta a metade esquerda e o proprio meio. */
            snprintf(acao, sizeof acao, "%d < %d, esq = %d  -> [%d, %d]",
                     v[meio], alvo, meio + 1, meio + 1, dir);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            esq = meio + 1;
        } else {
            /* O alvo e menor: descarta a metade direita e o proprio meio. */
            snprintf(acao, sizeof acao, "%d > %d, dir = %d  -> [%d, %d]",
                     v[meio], alvo, meio - 1, esq, meio - 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            dir = meio - 1;
        }
    }

    /* esq passou de dir: o intervalo ficou vazio, o alvo nao existe. */
    snprintf(acao, sizeof acao, "esq (%d) > dir (%d), intervalo vazio, devolve -1", esq, dir);
    trace_rodape(acao);
    return -1;
}

/* ------------------------------------------------------------------ */
/* Versao recursiva                                                    */
/* Tempo O(log n), memoria extra O(log n): cada chamada ocupa um novo  */
/* quadro na pilha de execucao (Call Stack) com seus parametros e o    */
/* endereco de retorno. A linguagem C nao garante otimizacao de        */
/* recursao em cauda, entao esse custo e real.                         */
/* ------------------------------------------------------------------ */

/* Funcao auxiliar: o parametro prof guarda a profundidade da pilha,
 * apenas para mostrar no teste de mesa quantas chamadas estao abertas. */
static int recursiva_aux(const int v[], int esq, int dir, int alvo, int prof) {
    char acao[96];

    /* Caso base 1: intervalo vazio, o alvo nao existe. */
    if (esq > dir) {
        snprintf(acao, sizeof acao,
                 "esq (%d) > dir (%d), devolve -1 com %d chamadas na pilha",
                 esq, dir, prof);
        trace_rodape(acao);
        return -1;
    }

    int meio = esq + (dir - esq) / 2;

    /* Caso base 2: encontrou o alvo. */
    if (v[meio] == alvo) {
        snprintf(acao, sizeof acao, "%d == %d, encontrado (pilha com %d chamadas)",
                 v[meio], alvo, prof);
        trace_linha(prof, esq, dir, meio, v[meio], acao);
        return meio;
    }

    /* Passo recursivo: chama a si mesma com a metade que sobrou. */
    if (v[meio] < alvo) {
        snprintf(acao, sizeof acao, "%d < %d, chama com [%d, %d]",
                 v[meio], alvo, meio + 1, dir);
        trace_linha(prof, esq, dir, meio, v[meio], acao);
        return recursiva_aux(v, meio + 1, dir, alvo, prof + 1);
    }

    snprintf(acao, sizeof acao, "%d > %d, chama com [%d, %d]",
             v[meio], alvo, esq, meio - 1);
    trace_linha(prof, esq, dir, meio, v[meio], acao);
    return recursiva_aux(v, esq, meio - 1, alvo, prof + 1);
}

int busca_binaria_recursiva(const int v[], int esq, int dir, int alvo) {
    trace_cabecalho("Busca binaria recursiva (passo = profundidade da pilha)");
    return recursiva_aux(v, esq, dir, alvo, 1);
}
