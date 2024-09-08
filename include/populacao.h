#ifndef _populacao_h_
#define _populacao_h_


typedef struct {
    unsigned char vivo:1;
    unsigned char sexo:1;
    unsigned char doente:1;
    unsigned char vacinado:1;
    unsigned char possuiCondicaoAgravante:1;
    unsigned char recuperado:1;
    unsigned char imunidade:3;
    unsigned char idade:7;
    unsigned char latencia:8;
    unsigned char diasDoente:5;
    unsigned char nivelInteracoesSociais:3;
}Individuo, Individuos;

typedef struct {
    unsigned tamanhoPopulacao;
    Individuos *I;
}Populacao;

typedef struct {
    unsigned tamanhoPopulacaoAtual;
    unsigned numeroDoentes;
    unsigned numeroRecuperados;
    unsigned numeroMortosTotal;

    unsigned numCriancas;
    unsigned numAdolescentes;
    unsigned numAdultos;
    unsigned numIdosos;
    unsigned numHomens;
    unsigned numMulheres;

    double percentualCriancas;
    double percentualAdolescentes;
    double percentualAdultos;
    double percentualIdosos;
    double percentualHomens;
    double percentualMulheres;
} StatusPopulacao;

Populacao *criaPopulacao();
void destroiPopulacao(Populacao *P);
void printaPopulacao(Populacao *P);

StatusPopulacao *criaStatusPopulacao(Populacao *Pop);
void destroiStatusPopulacao(StatusPopulacao *sPop);
void printaStatusPopulacao(StatusPopulacao *sPop);
void atualizaStatusPopulacao(StatusPopulacao *sPop, Individuo morto);

#endif