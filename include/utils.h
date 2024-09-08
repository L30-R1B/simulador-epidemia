#ifndef _utils_h_
#define _utils_h_

#include <stdlib.h>
#include "relacoes.h"

typedef struct {
    unsigned qtdDiasSimulacao;
    unsigned tamanhoPopulacao;
    unsigned maxPessoasCirculoSocial;
    unsigned char reinfeccao;

    double percentualCriancas;
    double percentualAdolescentes;
    double percentualAdultos;
    double percentualIdosos;
    double percentualComorbidade;
    double percentualHomens;
    double percentualMulheres;

    double taxaTransmissao;
    double taxaLetalidade;

    double percentualVacinados;
    double eficaciaVacina;
    int reincidencia;

    double agravanteHomem;
    double agravanteMulher;
    double agravanteIdade;
    double agravanteComorbidade;
}VariaveisAmbiente;

extern VariaveisAmbiente *varAmbiente;

extern FILE *arqOutput;

void iniciaDir(char *dir);
void iniciaArqMortesDiarias();
void iniciaArqDoentesDiarios();
void iniciaArqTotalInfectados();

void adicionaInfoMortesDiaria(unsigned numMortes, unsigned dia);
void adicionaInfoNovosCasosDiario(unsigned numNovosCasos, unsigned dia);
void adicionaInfoTotalCasos(unsigned numNovosCasos, unsigned dia);

void salvaListaRelacoes(void *list);
void salvarEstatisticaEpidemia(void *info, char *fileName);
void salvarVariaveisAmbiente();

void plotarGraficos();

int inicializarVariaveisAmbiente();
void finalizaVariaveisAmbiente();

#endif