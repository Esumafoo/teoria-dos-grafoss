#ifndef IO_H
#define IO_H

#include "grafo.h"

typedef struct {
    GrafoMatriz *grafo;
    int ponderado;
} EntradaPratica10;

int ler_entrada_pratica10(EntradaPratica10 *entrada);
void liberar_entrada_pratica10(EntradaPratica10 *entrada);
void imprimir_sequencia(const char *rotulo, const int *sequencia, size_t n, int fechar_ciclo);
void executar_demonstracao_e_experimento(void);

#endif
