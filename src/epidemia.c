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
}

void transmite(SimuladorEpidemia *se, unsigned individuo){
    if(!varAmbiente->reinfeccao && se->pop->I[individuo].recuperado)
        return;
    if(se->pop->I[individuo].doente)
        return;
    if(se->pop->I[individuo].vivo){
        se->pop->I[individuo].doente = 1;
        se->statusPop->numeroDoentes ++;
    }
}

unsigned numVacinado;
void resetarProgVacinacao(){
    numVacinado = 0;
}

void progrideVacinacao(SimuladorEpidemia *se){
    unsigned numTotalVacinados = ((varAmbiente->percentualVacinados / 100) * se->pop->tamanhoPopulacao);
    if(numVacinado > (se->pop->tamanhoPopulacao - 1 - (numTotalVacinados/100)))
        return;
    unsigned individuoAtual;
    for(individuoAtual = numVacinado; individuoAtual < ((numTotalVacinados/100)+numVacinado); individuoAtual ++){
        se->pop->I[individuoAtual].vacinado = 1;
    }
    numVacinado = individuoAtual;
}

void progrideEpidemia(SimuladorEpidemia *se){
    double percentual;
    double agravante;
    double probabilidadeMorte;

    for(unsigned individuoAtual = 0; individuoAtual < se->pop->tamanhoPopulacao; individuoAtual ++){
        if(!se->pop->I[individuoAtual].vivo)
            continue;
        
        if(se->pop->I[individuoAtual].doente){
            double taxaTransmissao = (varAmbiente->taxaTransmissao * (se->pop->I[individuoAtual].vacinado ? (1.0 - (varAmbiente->eficaciaVacina/100.0)) : 1.0) * ( 1.0 - ((se->pop->I[individuoAtual].imunidade / 10.0)))) / (varAmbiente->taxaTransmissao * 1.2);

            for(unsigned relacaoAtual = 0; relacaoAtual < se->listRel->LR[individuoAtual].numRelacoes; relacaoAtual ++){
                percentual = (rand() % 100000) / 1000.0;
                if(percentual <= taxaTransmissao){
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

            percentual = (rand() % 10000) / 100.0;
    
            agravante = (varAmbiente->agravanteIdade * se->pop->I[individuoAtual].idade) * (se->pop->I[individuoAtual].sexo ? varAmbiente->agravanteHomem : varAmbiente->agravanteMulher) * (varAmbiente->agravanteComorbidade * (se->pop->I[individuoAtual].possuiCondicaoAgravante ? 2.5 : 1.0));
    
            probabilidadeMorte = varAmbiente->taxaMortalidade * agravante * (se->pop->I[individuoAtual].vacinado ? (1.0 - (varAmbiente->eficaciaVacina/100.0)) : 1.0);

            if(percentual <= probabilidadeMorte){
                se->pop->I[individuoAtual].vivo = 0;
                se->listRel->LR[individuoAtual].vivo = 0;
                se->statusPop->numeroDoentes --;

                atualizaStatusPopulacao(se->statusPop, se->pop->I[individuoAtual]);
                continue;
            }

            se->pop->I[individuoAtual].diasDoente ++;
        }
    }
}