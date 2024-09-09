#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/relacoes.h"
#include "../include/populacao.h"
#include "../include/epidemia.h"
#include "../include/utils.h"

unsigned numMortesDia = 0;
unsigned numCasosDia = 0;

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

void transmite(SimuladorEpidemia *se, unsigned individuo) {
    if (!se->pop->I[individuo].vivo || se->pop->I[individuo].doente){
        return;
    }
    if (varAmbiente->reinfeccao) {   
        if(se->pop->I[individuo].recuperado){
            if(!se->pop->I[individuo].latencia){
                se->pop->I[individuo].doente = 1;
                se->statusPop->numeroDoentes++;
                numCasosDia++;
            }
        }
    }
    if (se->pop->I[individuo].recuperado)
        return;
    se->pop->I[individuo].doente = 1;
    se->statusPop->numeroDoentes++;
    numCasosDia++;
}


unsigned qtdVacinados;
void resetarProgVacinacao(){
    qtdVacinados = 0;
}

void progrideVacinacao(SimuladorEpidemia *se) {
    if(se->diasPassados < 50)
        return;
    unsigned numTotalVacinados = (se->pop->tamanhoPopulacao * varAmbiente->percentualVacinados) / (varAmbiente->qtdDiasSimulacao - 50);
    if (qtdVacinados >= se->pop->tamanhoPopulacao - 1)
        return;

    unsigned numRestantes = numTotalVacinados - qtdVacinados;
    unsigned vacinasHoje = (numTotalVacinados/30);
    if (vacinasHoje > numRestantes) {
        vacinasHoje = numRestantes;
    }
    
    for (unsigned i = 0; i < vacinasHoje; i++) {
        if (qtdVacinados >= se->pop->tamanhoPopulacao)
            break;
        se->pop->I[qtdVacinados].vacinado = 1;
        qtdVacinados ++;
    }
}

void progrideEpidemia(SimuladorEpidemia *se) {
    double percentual;
    double agravante;
    double probabilidadeMorte;
    double taxaTransmissaoBase = varAmbiente->taxaTransmissao;

    numMortesDia = 0;
    numCasosDia = 0;

    for (unsigned individuoAtual = 0; individuoAtual < se->pop->tamanhoPopulacao; individuoAtual++) {
        if (!se->pop->I[individuoAtual].vivo)
            continue;

        if (se->pop->I[individuoAtual].doente) {
            double taxaTransmissao = taxaTransmissaoBase * (se->pop->I[individuoAtual].vacinado ? 
                               (1.0 - (varAmbiente->eficaciaVacina / 100.0)) : 1.0);

            taxaTransmissao *= (1.0 - (se->pop->I[individuoAtual].imunidade * 0.03));

            if (se->pop->I[individuoAtual].diasDoente > 2)
                taxaTransmissao /= 19.0;

            for (unsigned relacaoAtual = 0; relacaoAtual < se->listRel->LR[individuoAtual].numRelacoes; relacaoAtual++) {
                percentual = (rand() % 100000) / 1000.0;

                double fator_interacao = 1.0 + (se->pop->I[individuoAtual].nivelInteracoesSociais / 7.0);

                double probabilidadeInfeccao = taxaTransmissao * fator_interacao;

                if (percentual <= probabilidadeInfeccao) {
                    transmite(se, se->listRel->LR[individuoAtual].indicesIndividuos[relacaoAtual]);
                }
            }

            if (se->pop->I[individuoAtual].diasDoente >= (rand() % 7 + 10)) {
                percentual = (rand() % 10000) / 100.0;

                agravante = (varAmbiente->agravanteIdade * se->pop->I[individuoAtual].idade) 
                            * (se->pop->I[individuoAtual].sexo ? varAmbiente->agravanteHomem : varAmbiente->agravanteMulher) 
                            * (varAmbiente->agravanteComorbidade * (se->pop->I[individuoAtual].possuiCondicaoAgravante ? 2.5 : 1.0));

                probabilidadeMorte = ((varAmbiente->taxaLetalidade / 100) * agravante * 
                                     (se->pop->I[individuoAtual].vacinado ? (1.0 - (varAmbiente->eficaciaVacina / 100.0)) : 1.0)) * 2.3;

                if (percentual <= probabilidadeMorte) {
                    se->pop->I[individuoAtual].vivo = 0;
                    se->listRel->LR[individuoAtual].vivo = 0;
                    se->statusPop->numeroDoentes--;
                    numMortesDia++;
                    atualizaStatusPopulacao(se->statusPop, se->pop->I[individuoAtual]);
                    continue;
                }

                se->pop->I[individuoAtual].diasDoente = 0;
                se->pop->I[individuoAtual].doente = 0;
                se->pop->I[individuoAtual].recuperado = 1;
                se->pop->I[individuoAtual].latencia = varAmbiente->reincidencia;
                se->statusPop->numeroRecuperados++;
                se->statusPop->numeroDoentes--;
            }

            se->pop->I[individuoAtual].diasDoente++;
        }
        if(se->pop->I[individuoAtual].recuperado && se->pop->I[individuoAtual].latencia){
            se->pop->I[individuoAtual].latencia --;
        }
    }

    adicionaInfoMortesDiaria(numMortesDia, se->diasPassados);
    adicionaInfoNovosCasosDiario(numCasosDia, se->diasPassados);
    adicionaInfoTotalCasos(se->statusPop->numeroDoentes, se->diasPassados);
    se->diasPassados++;
}
