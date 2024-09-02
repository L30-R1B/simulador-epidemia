#ifndef _epidemia_h_
#define _epidemia_h_

#include "../include/populacao.h"
#include "../include/relacoes.h"

typedef struct {
    unsigned diasPassados;
    Populacao *pop;
    StatusPopulacao *statusPop;
    ListaRelacoes *listRel;
}SimuladorEpidemia;

SimuladorEpidemia *criaSimuladorEpidemia();
void destroiSimuladorEpidemia(SimuladorEpidemia *sE);

void resetarProgVacinacao();
void progrideVacinacao(SimuladorEpidemia *se);
void iniciaEpidemia(SimuladorEpidemia *se, unsigned numInicialInfectados);
void progrideEpidemia(SimuladorEpidemia *se);

#endif