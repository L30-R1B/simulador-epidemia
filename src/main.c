#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/relacoes.h"
#include "../include/populacao.h"
#include "../include/epidemia.h"
#include "../include/utils.h"


int main(){
    srand(time(NULL));
    inicializarVariaveisAmbiente();

    SimuladorEpidemia *se = criaSimuladorEpidemia();


    iniciaEpidemia(se, 5);

    printf("DIA : 0\n");
    printaStatusPopulacao(se->statusPop);

    for(unsigned i = 0; i < 1000; i ++){
        progrideEpidemia(se);
    }

    printf("DIA : 1000\n");
    printaStatusPopulacao(se->statusPop);

    destroiSimuladorEpidemia(se);
    finalizaVariaveisAmbiente();
}