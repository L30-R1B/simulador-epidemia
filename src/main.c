#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../include/relacoes.h"
#include "../include/populacao.h"
#include "../include/epidemia.h"
#include "../include/utils.h"

void realizaSimulacao(){
    SimuladorEpidemia *se = criaSimuladorEpidemia();
    iniciaArqMortesDiarias();
    iniciaArqDoentesDiarios();
    iniciaArqTotalInfectados();

    iniciaEpidemia(se, 25);
    salvarVariaveisAmbiente();
    if(se->pop->tamanhoPopulacao <= 1000){
        salvaListaRelacoes(se->listRel);
        calculaCentralidade();
    }
    salvarEstatisticaEpidemia(se->statusPop, "1_estatisticasIniciais.txt");
    resetarProgVacinacao();
    for(unsigned i = 0; i < varAmbiente->qtdDiasSimulacao; i ++){
        progrideEpidemia(se);
        progrideVacinacao(se);
    }
    salvarEstatisticaEpidemia(se->statusPop, "2_estatisticasFinais.txt");

    finalizaVariaveisAmbiente();
    plotarGraficos();

    destroiSimuladorEpidemia(se);
}

int main(){
    char dirNome[64];
    scanf("%s", dirNome);
    inicializarVariaveisAmbiente();
    iniciaDir(dirNome);

    srand(time(NULL));
    realizaSimulacao();
}