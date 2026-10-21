/*
 * testes_automatizados.c
 *
 * Confere automaticamente se todas as buscas devolvem o resultado certo.
 * A busca linear, que e simples e obviamente correta, serve de referencia
 * para conferir a busca binaria em milhares de vetores gerados ao acaso.
 *
 * Compilar e rodar:  make testes
 */

#include <stdio.h>
#include <stdlib.h>
#include "buscas.h"

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

static int total = 0;
static int falhas = 0;

/* Registra um teste: se obtido != esperado, mostra onde falhou. */
static void confere(const char *nome, int obtido, int esperado) {
    total++;
    if (obtido != esperado) {
        falhas++;
        printf("FALHOU: %s (obtido %d, esperado %d)\n", nome, obtido, esperado);
    }
}

/* Referencias lineares para lower e upper bound. */
static int lower_linear(const int v[], int n, int alvo) {
    for (int i = 0; i < n; i++) if (v[i] >= alvo) return i;
    return n;
}
static int upper_linear(const int v[], int n, int alvo) {
    for (int i = 0; i < n; i++) if (v[i] > alvo) return i;
    return n;
}

static void testes_dos_slides(void) {
    const int v[] = {4, 9, 15, 22, 31, 40, 48, 57, 63, 79, 85};
    int n = TAM(v);

    confere("linear 63", busca_linear(v, n, 63), 8);
    confere("linear ausente", busca_linear(v, n, 25), -1);
    confere("iterativa 63", busca_binaria_iterativa(v, n, 63), 8);
    confere("iterativa 25", busca_binaria_iterativa(v, n, 25), -1);
    confere("recursiva 63", busca_binaria_recursiva(v, 0, n - 1, 63), 8);
    confere("recursiva 25", busca_binaria_recursiva(v, 0, n - 1, 25), -1);
    confere("primeiro elemento", busca_binaria_iterativa(v, n, 4), 0);
    confere("ultimo elemento", busca_binaria_iterativa(v, n, 85), 10);
    confere("menor que todos", busca_binaria_iterativa(v, n, 1), -1);
    confere("maior que todos", busca_binaria_iterativa(v, n, 99), -1);

    const int rep[] = {10, 20, 20, 20, 30};
    confere("lower_bound 20", lower_bound(rep, 5, 20), 1);
    confere("upper_bound 20", upper_bound(rep, 5, 20), 4);
    confere("contagem 20", contar_ocorrencias(rep, 5, 20), 3);

    const int status[] = {200, 200, 200, 404, 404, 404, 404, 500, 500};
    confere("contagem 404", contar_ocorrencias(status, 9, 404), 4);
    confere("contagem 302", contar_ocorrencias(status, 9, 302), 0);

    const int arquivos[] = {10, 20, 30, 40, 50};
    confere("dias com 60", contar_dias(arquivos, 5, 60, NULL, 0), 3);
    confere("dias com 59", contar_dias(arquivos, 5, 59, NULL, 0), 4);
    confere("viavel 60", eh_viavel(arquivos, 5, 60, 3), 1);
    confere("viavel 59", eh_viavel(arquivos, 5, 59, 3), 0);
    confere("exercicio 2", busca_espaco_resposta(arquivos, 5, 3), 60);
    confere("exercicio 2, 1 dia", busca_espaco_resposta(arquivos, 5, 1), 150);
    confere("exercicio 2, 5 dias", busca_espaco_resposta(arquivos, 5, 5), 50);
}

static void testes_de_borda(void) {
    const int vazio[1] = {0};
    const int um[] = {7};

    confere("vetor vazio", busca_binaria_iterativa(vazio, 0, 7), -1);
    confere("vetor vazio recursiva", busca_binaria_recursiva(vazio, 0, -1, 7), -1);
    confere("um elemento, achou", busca_binaria_iterativa(um, 1, 7), 0);
    confere("um elemento, nao achou", busca_binaria_iterativa(um, 1, 8), -1);
    confere("lower_bound vazio", lower_bound(vazio, 0, 7), 0);
    confere("upper_bound alem do fim", upper_bound(um, 1, 7), 1);
}

/* Gera vetores ordenados aleatorios (com repeticoes) e compara a busca
 * binaria com a busca linear para varios alvos. */
static void testes_aleatorios(void) {
    int v[200];
    srand(2026);

    for (int rodada = 0; rodada < 2000; rodada++) {
        int n = rand() % 200;
        int valor = rand() % 10;
        for (int i = 0; i < n; i++) {
            valor += rand() % 4;          /* passos de 0 geram repeticoes */
            v[i] = valor;
        }

        for (int alvo = -2; alvo <= valor + 2; alvo++) {
            int pos_it = busca_binaria_iterativa(v, n, alvo);
            int pos_rec = busca_binaria_recursiva(v, 0, n - 1, alvo);
            int existe = busca_linear(v, n, alvo) != -1;

            /* Com repeticoes, qualquer posicao com o valor certo vale. */
            confere("aleatorio iterativa", existe ? (pos_it >= 0 && v[pos_it] == alvo) : (pos_it == -1), 1);
            confere("aleatorio recursiva", existe ? (pos_rec >= 0 && v[pos_rec] == alvo) : (pos_rec == -1), 1);
            confere("aleatorio lower", lower_bound(v, n, alvo), lower_linear(v, n, alvo));
            confere("aleatorio upper", upper_bound(v, n, alvo), upper_linear(v, n, alvo));
        }
    }
}

int main(void) {
    busca_trace = 0;   /* testes rodam em silencio */

    testes_dos_slides();
    testes_de_borda();
    testes_aleatorios();

    if (falhas == 0) {
        printf("OK: %d testes passaram.\n", total);
        return 0;
    }
    printf("%d de %d testes falharam.\n", falhas, total);
    return 1;
}
