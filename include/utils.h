#ifndef _utils_h_
#define _utils_h_

typedef struct {
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
    double taxaMortalidade;
    double taxaLetalidade;

    double percentualVacinados;

    double agravanteHomem;
    double agravanteMulher;
    double agravanteIdade;
    double agravanteComorbidade;
}VariaveisAmbiente;

extern VariaveisAmbiente *varAmbiente;

int inicializarVariaveisAmbiente();
void finalizaVariaveisAmbiente();

#endif