/*
 * variantes_avancadas.c
 *
 * Variantes da busca binaria:
 *   - lower_bound e upper_bound: tratam valores repetidos e filtros de faixa;
 *   - busca no espaco de resposta: encontra o menor valor que resolve
 *     um problema de otimizacao, mesmo sem existir um vetor ordenado.
 */

#include <stdio.h>
#include <string.h>
#include "buscas.h"

/* ------------------------------------------------------------------ */
/* Lower Bound                                                         */
/* Primeiro indice i tal que v[i] >= alvo. Se nenhum servir, devolve n. */
/*                                                                     */
/* Diferenca para a busca classica: quando v[meio] serve como resposta, */
/* nao paramos. Guardamos o candidato e continuamos procurando mais a   */
/* esquerda, para achar a PRIMEIRA posicao valida.                      */
/* ------------------------------------------------------------------ */
int lower_bound(const int v[], int n, int alvo) {
    int esq = 0, dir = n - 1;
    int resposta = n;             /* n significa: depois do fim do vetor */
    int passo = 0;
    char acao[96];

    trace_cabecalho("Lower Bound (primeiro valor >= alvo)");

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        passo++;

        if (v[meio] >= alvo) {
            /* Candidato valido: guarda e tenta achar outro mais a esquerda. */
            snprintf(acao, sizeof acao, "%d >= %d, resposta = %d, dir = %d",
                     v[meio], alvo, meio, meio - 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            resposta = meio;
            dir = meio - 1;
        } else {
            snprintf(acao, sizeof acao, "%d < %d, esq = %d", v[meio], alvo, meio + 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            esq = meio + 1;
        }
    }

    snprintf(acao, sizeof acao, "Lower Bound = %d", resposta);
    trace_rodape(acao);
    return resposta;
}

/* ------------------------------------------------------------------ */
/* Upper Bound                                                         */
/* Primeiro indice i tal que v[i] > alvo (estritamente maior).         */
/* O codigo e identico ao lower_bound, trocando >= por >.              */
/* ------------------------------------------------------------------ */
int upper_bound(const int v[], int n, int alvo) {
    int esq = 0, dir = n - 1;
    int resposta = n;
    int passo = 0;
    char acao[96];

    trace_cabecalho("Upper Bound (primeiro valor > alvo)");

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        passo++;

        if (v[meio] > alvo) {
            snprintf(acao, sizeof acao, "%d > %d, resposta = %d, dir = %d",
                     v[meio], alvo, meio, meio - 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            resposta = meio;
            dir = meio - 1;
        } else {
            snprintf(acao, sizeof acao, "%d <= %d, esq = %d", v[meio], alvo, meio + 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            esq = meio + 1;
        }
    }

    snprintf(acao, sizeof acao, "Upper Bound = %d", resposta);
    trace_rodape(acao);
    return resposta;
}

/* ------------------------------------------------------------------ */
/* Contagem de ocorrencias em O(log n)                                 */
/* Andar para os lados a partir de um elemento encontrado custaria     */
/* O(n) no pior caso. Com as duas buscas, a contagem continua O(log n). */
/* ------------------------------------------------------------------ */
int contar_ocorrencias(const int v[], int n, int alvo) {
    return upper_bound(v, n, alvo) - lower_bound(v, n, alvo);
}

/* ------------------------------------------------------------------ */
/* Busca binaria no espaco de resposta                                 */
/*                                                                     */
/* Problema (Exercicio 2): transferir arquivos em ordem, sem dividir    */
/* nenhum, em ate k dias. Qual a menor capacidade diaria necessaria?    */
/* ------------------------------------------------------------------ */

/* Simula a transferencia de forma gulosa: enche o dia atual com os
 * arquivos, em ordem, e abre um novo dia quando o proximo nao cabe.
 * Custo O(n). Supoe capacidade >= maior arquivo. */
int contar_dias(const int arq[], int n, int capacidade, char *lotes, size_t tam) {
    int dias = 1;
    int carga = 0;
    size_t usado = 0;

    if (lotes != NULL && tam > 0) {
        lotes[0] = '\0';
        usado += (size_t)snprintf(lotes, tam, "[");
    }

    for (int i = 0; i < n; i++) {
        if (carga + arq[i] > capacidade) {
            /* Nao cabe mais hoje: fecha o dia e comeca outro. */
            dias++;
            carga = 0;
            if (lotes != NULL && usado < tam) {
                usado += (size_t)snprintf(lotes + usado, tam - usado, "] [");
            }
        } else if (i > 0 && lotes != NULL && usado < tam) {
            usado += (size_t)snprintf(lotes + usado, tam - usado, "+");
        }
        carga += arq[i];
        if (lotes != NULL && usado < tam) {
            usado += (size_t)snprintf(lotes + usado, tam - usado, "%d", arq[i]);
        }
    }

    if (lotes != NULL && usado < tam) {
        snprintf(lotes + usado, tam - usado, "]");
    }
    return dias;
}

/* Funcao de viabilidade (predicado monotono): se funciona com uma
 * capacidade x, funciona com qualquer capacidade maior que x. */
int eh_viavel(const int arq[], int n, int capacidade, int k) {
    return contar_dias(arq, n, capacidade, NULL, 0) <= k;
}

int busca_espaco_resposta(const int arq[], int n, int k) {
    /* Fronteiras do espaco de resposta:
     *   esq = maior arquivo (nenhum arquivo pode ser dividido);
     *   dir = soma de todos (tudo cabe em um unico dia). */
    int esq = 0, dir = 0;
    for (int i = 0; i < n; i++) {
        if (arq[i] > esq) esq = arq[i];
        dir += arq[i];
    }

    int resposta = dir;
    int passo = 0;
    char lotes[128];
    char acao[200];

    trace_cabecalho("Busca no espaco de resposta (valor = dias usados)");

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;     /* capacidade testada */
        int dias = contar_dias(arq, n, meio, lotes, sizeof lotes);
        passo++;

        if (dias <= k) {
            /* Viavel: guarda e tenta uma capacidade menor. */
            snprintf(acao, sizeof acao, "%s viavel, resp = %d, dir = %d",
                     lotes, meio, meio - 1);
            trace_linha(passo, esq, dir, meio, dias, acao);
            resposta = meio;
            dir = meio - 1;
        } else {
            /* Inviavel: precisa de mais capacidade. */
            snprintf(acao, sizeof acao, "%s inviavel, esq = %d",
                     lotes, meio + 1);
            trace_linha(passo, esq, dir, meio, dias, acao);
            esq = meio + 1;
        }
    }

    snprintf(acao, sizeof acao, "capacidade minima = %d (em %d testes)", resposta, passo);
    trace_rodape(acao);
    return resposta;
}
