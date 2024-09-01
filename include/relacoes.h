#ifndef _relacoes_h_
#define _relacoes_h_

#include "../include/populacao.h"
#include "../include/utils.h"

#include <math.h>

#define NUM_RELACOES_INDIVIDUO(x) (int)(((((double)varAmbiente->maxPessoasCirculoSocial - 2) / (1 + exp(-1.2 * ((x) - 3.5)))) + (rand() % 3)) + 1)

typedef struct {
    unsigned char vivo;
    unsigned char maxRelacoes;
    unsigned char numRelacoes;
    unsigned *indicesIndividuos;
}LinhaRelacoes;

typedef struct {
    long unsigned tamanhoPopulacao;
    long unsigned numTotalRelacoes;
    LinhaRelacoes *LR;
}ListaRelacoes;

ListaRelacoes *criaListaRelacoes(Populacao P);
void destroiListaRelacoes(ListaRelacoes *listRel);
void printaListaRelacoes(ListaRelacoes *listRel);
void trocaRelacoesIndividuo(ListaRelacoes *listRel, unsigned individuo);

#endif