/*
 * teste_de_mesa_logger.c
 *
 * Imprime no terminal o teste de mesa de cada algoritmo: a evolucao de
 * esq, dir e meio a cada iteracao. A saida reproduz exatamente as
 * tabelas apresentadas nos slides do seminario.
 *
 * Compilar e rodar:  make mesa
 */

#include <stdio.h>
#include <stdint.h>
#include "buscas.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

static void titulo(const char *texto) {
    printf("\n==============================================================\n");
    printf(" %s\n", texto);
    printf("==============================================================\n");
}

/* ------------------------------------------------------------------ */
/* Versao COM BUG usada so para demonstrar a Parte B do Exercicio 1:    */
/* usa dir = meio em vez de dir = meio - 1. O parametro limite impede   */
/* que o programa trave de verdade durante a demonstracao.              */
/* NAO use esta funcao em codigo real.                                  */
/* ------------------------------------------------------------------ */
static int busca_com_bug(const int v[], int n, int alvo, int limite) {
    int esq = 0, dir = n - 1, passo = 0;
    char acao[96];

    trace_cabecalho("Busca binaria COM BUG (dir = meio)");

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        passo++;

        if (passo > limite) {
            snprintf(acao, sizeof acao,
                     "interrompido apos %d passos: o intervalo [%d, %d] nao muda mais (LOOP INFINITO)",
                     limite, esq, dir);
            trace_rodape(acao);
            return -2;
        }
        if (v[meio] == alvo) {
            trace_linha(passo, esq, dir, meio, v[meio], "encontrado");
            return meio;
        }
        if (v[meio] < alvo) {
            snprintf(acao, sizeof acao, "%d < %d, esq = %d", v[meio], alvo, meio + 1);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            esq = meio + 1;
        } else {
            snprintf(acao, sizeof acao, "%d > %d, dir = meio = %d  (BUG)", v[meio], alvo, meio);
            trace_linha(passo, esq, dir, meio, v[meio], acao);
            dir = meio;                       /* o erro: o meio continua no intervalo */
        }
    }
    return -1;
}

/* Simula o calculo do meio com inteiros de 32 bits para mostrar o
 * overflow. A soma e feita em unsigned para reproduzir o "dar a volta"
 * do hardware sem cair em comportamento indefinido do C. */
static void demonstrar_overflow(void) {
    int32_t esq = 1500000000;
    int32_t dir = 1800000000;

    uint32_t soma_bits = (uint32_t)esq + (uint32_t)dir;   /* 3.300.000.000 em 32 bits */
    int32_t soma = (int32_t)soma_bits;                     /* lido como int com sinal  */
    int32_t meio_inseguro = soma / 2;
    int32_t meio_seguro = esq + (dir - esq) / 2;

    printf("\nesq = %d, dir = %d (limite do int: 2147483647)\n", esq, dir);
    printf("  Inseguro: (esq + dir) / 2       -> soma vira %d, meio = %d  (indice negativo!)\n",
           soma, meio_inseguro);
    printf("  Seguro:   esq + (dir - esq) / 2 -> meio = %d\n", meio_seguro);
}

int main(void) {
    busca_trace = 1;   /* liga a impressao do teste de mesa */

    /* Vetor do Exercicio 1 (11 posicoes, indices 0 a 10). */
    const int v[] = {4, 9, 15, 22, 31, 40, 48, 57, 63, 79, 85};
    int n = TAM(v);

    titulo("Comparacao: busca linear procurando 63");
    printf("resultado: indice %d\n", busca_linear(v, n, 63));

    titulo("Exercicio 1, Parte A: procurando 63");
    printf("resultado iterativa: indice %d\n", busca_binaria_iterativa(v, n, 63));
    printf("resultado recursiva: indice %d\n", busca_binaria_recursiva(v, 0, n - 1, 63));

    titulo("Exercicio 1, Parte B: procurando 25 (ausente)");
    printf("resultado iterativa: %d\n", busca_binaria_iterativa(v, n, 25));
    printf("resultado recursiva: %d\n", busca_binaria_recursiva(v, 0, n - 1, 25));

    titulo("Exercicio 1, Parte B: o mesmo teste com o bug dir = meio");
    busca_com_bug(v, n, 25, 8);

    titulo("Armadilha: overflow no calculo do meio");
    demonstrar_overflow();

    titulo("Lower Bound e Upper Bound com valores repetidos");
    const int rep[] = {10, 20, 20, 20, 30};
    int lb = lower_bound(rep, TAM(rep), 20);
    int ub = upper_bound(rep, TAM(rep), 20);
    printf("ocorrencias de 20: %d - %d = %d\n", ub, lb, ub - lb);

    titulo("Contando erros 404 em um log");
    const int status[] = {200, 200, 200, 404, 404, 404, 404, 500, 500};
    lb = lower_bound(status, TAM(status), 404);
    ub = upper_bound(status, TAM(status), 404);
    printf("erros 404: %d - %d = %d\n", ub, lb, ub - lb);

    titulo("Filtro de faixa: transacoes entre 100 e 500 (valores em reais)");
    const int transacoes[] = {45, 102, 230, 499, 640};
    lb = lower_bound(transacoes, TAM(transacoes), 100);
    ub = upper_bound(transacoes, TAM(transacoes), 500);
    printf("transacoes no filtro: indices %d a %d ->", lb, ub - 1);
    for (int i = lb; i < ub; i++) printf(" %d", transacoes[i]);
    printf("\n");

    titulo("Exercicio 2: busca no espaco de resposta [50, 150], prazo 3 dias");
    const int arquivos[] = {10, 20, 30, 40, 50};
    int cap = busca_espaco_resposta(arquivos, TAM(arquivos), 3);
    printf("capacidade minima: %d GB por dia\n", cap);

    printf("\n");
    return 0;
}
