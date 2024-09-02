#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/relacoes.h"
#include "../include/populacao.h"
#include "../include/epidemia.h"
#include "../include/utils.h"


int main(){
    srand(time(NULL));
    inicializarVariaveisAmbiente();

    SimuladorEpidemia *se = criaSimuladorEpidemia();

    iniciaEpidemia(se, 10);
    resetarProgVacinacao();

    for(unsigned i = 0; i < varAmbiente->qtdDiasSimulacao; i ++){
        printf("DIA : %u\n", i);
        printaStatusPopulacao(se->statusPop);
        progrideEpidemia(se);
        progrideVacinacao(se);
    }

    destroiSimuladorEpidemia(se);
    finalizaVariaveisAmbiente();
}