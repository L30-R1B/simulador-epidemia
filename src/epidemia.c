#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/relacoes.h"
#include "../include/populacao.h"
#include "../include/epidemia.h"
#include "../include/utils.h"

SimuladorEpidemia *criaSimuladorEpidemia(){
    SimuladorEpidemia *sE = (SimuladorEpidemia*) malloc(sizeof(SimuladorEpidemia));

    sE->diasPassados = 0;
    sE->pop = criaPopulacao();
    sE->statusPop = criaStatusPopulacao(sE->pop);
    sE->listRel = criaListaRelacoes(*sE->pop);

    return sE;
}

void destroiSimuladorEpidemia(SimuladorEpidemia *sE){

    if(sE == NULL)
        return;

    destroiPopulacao(sE->pop);
    destroiStatusPopulacao(sE->statusPop);
    destroiListaRelacoes(sE->listRel);

    free(sE);
}

void iniciaEpidemia(SimuladorEpidemia *se, unsigned numInicialInfectados){
    unsigned indice;
    for(unsigned i = 0; i < numInicialInfectados; i ++){
        indice = rand() % (se->pop->tamanhoPopulacao);
        if(!se->pop->I[indice].doente)
            se->statusPop->numeroDoentes ++;
        se->pop->I[indice].doente = 1; 
    }
    printf("------------(%u)------------\n", se->statusPop->numeroDoentes);
}

void transmite(SimuladorEpidemia *se, unsigned individuo){
    if(!varAmbiente->reinfeccao && se->pop->I[individuo].recuperado)
        return;
    if(se->pop->I[individuo].vivo){
        se->pop->I[individuo].doente = 1;
        se->statusPop->numeroDoentes ++;
    }
}

void progrideEpidemia(SimuladorEpidemia *se){
    double percentual;
    double agravante;

    for(unsigned individuoAtual = 0; individuoAtual < se->pop->tamanhoPopulacao; individuoAtual ++){
        if(!se->pop->I[individuoAtual].vivo)
            continue;
        
        if(se->pop->I[individuoAtual].doente){
            for(unsigned relacaoAtual = 0; relacaoAtual < se->listRel->LR[individuoAtual].numRelacoes; relacaoAtual ++){
                percentual = rand() % 10000 / 100.0;
                if(percentual <= varAmbiente->taxaTransmissao){
                    transmite(se, se->listRel->LR[individuoAtual].indicesIndividuos[relacaoAtual]);
                }
            }
            if(se->pop->I[individuoAtual].diasDoente >= (rand() % 7 + 10)){
                se->pop->I[individuoAtual].diasDoente = 0;
                se->pop->I[individuoAtual].doente = 0;
                se->pop->I[individuoAtual].recuperado = 1;
                se->statusPop->numeroRecuperados ++;
                se->statusPop->numeroDoentes --;
                continue;
            }

            percentual = rand() % 10000 / 100.0;
            
            agravante = varAmbiente->taxaLetalidade;
            
            agravante = se->pop->I[individuoAtual].sexo? agravante * varAmbiente->agravanteHomem : agravante * varAmbiente->agravanteMulher;

            agravante *= se->pop->I[individuoAtual].idade * varAmbiente->agravanteIdade;
            agravante *= 1 + (se->pop->I[individuoAtual].possuiCondicaoAgravante * varAmbiente->agravanteComorbidade);

            if(percentual <= varAmbiente->taxaLetalidade){
                se->pop->I[individuoAtual].vivo = 0;
                se->listRel->LR[individuoAtual].vivo = 0;

                atualizaStatusPopulacao(se->statusPop, se->pop->I[individuoAtual]);
            }

            se->pop->I[individuoAtual].diasDoente ++;
        }

        

    }
}