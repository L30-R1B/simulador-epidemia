#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/populacao.h"
#include "../include/utils.h"


Populacao *criaPopulacao(){
    Populacao *P = (Populacao*) malloc(sizeof(Populacao));

    P->tamanhoPopulacao = varAmbiente->tamanhoPopulacao;
    P->I = (Individuos*) malloc(varAmbiente->tamanhoPopulacao * sizeof(Individuo));

    double probabilidade;
    for(unsigned i = 0; i < varAmbiente->tamanhoPopulacao; i ++){
        P->I[i].vivo = 1;
        P->I[i].doente = 0;
        P->I[i].imunidade = rand() % 8;
        P->I[i].recuperado = 0;
        P->I[i].vacinado = 0;
        P->I[i].latencia = varAmbiente->reincidencia;
        P->I[i].nivelInteracoesSociais = rand() % 8;
        P->I[i].diasDoente = 0;
        
        probabilidade = (rand() % 10000) / 100.0;
        P->I[i].sexo = probabilidade <= varAmbiente->percentualHomens ? 1 : 0;

        probabilidade = (rand() % 10000) / 100.0;
        if(probabilidade < varAmbiente->percentualCriancas)
            P->I[i].idade = rand() % 11;
        else if(probabilidade < varAmbiente->percentualCriancas + varAmbiente->percentualAdolescentes)
            P->I[i].idade = (rand() % 8) + 10;
        else if(probabilidade < varAmbiente->percentualCriancas + varAmbiente->percentualAdolescentes + varAmbiente->percentualAdultos)
            P->I[i].idade = (rand() % 42) + 18;
        else
            P->I[i].idade = (rand() % 30) + 60;

        probabilidade = (rand() % 10000) / 100.0;
        P->I[i].possuiCondicaoAgravante = probabilidade <= varAmbiente->percentualComorbidade? 1 : 0;
    }

    return P;
}

void printaPopulacao(Populacao *P){
    printf("________________________________________________________________________________\n");
    for(unsigned i = 0; i < P->tamanhoPopulacao; i ++){
        printf("%.6u - Vivo(%u), Sx(%u), Doente(%u), Vac(%u), Rec(%u), CondAgr(%u), IntSocial(%u), Imunid(%u), Idade(%u), DiasDoent(%u)\n", 
                i, P->I[i].vivo, P->I[i].sexo, P->I[i].doente, P->I[i].vacinado, 
                P->I[i].recuperado, P->I[i].possuiCondicaoAgravante, 
                P->I[i].nivelInteracoesSociais, P->I[i].imunidade, P->I[i].idade, P->I[i].diasDoente);
    }
    printf("________________________________________________________________________________\n");
}

void destroiPopulacao(Populacao *P){
    if(P == NULL)
        return;
    if(P->I == NULL){
        free(P);
        return;
    }
    free(P->I);
    free(P);
    return;
}

StatusPopulacao *criaStatusPopulacao(Populacao *P){
    StatusPopulacao *sPop = (StatusPopulacao*) malloc(sizeof(StatusPopulacao));
    sPop->tamanhoPopulacaoAtual = P->tamanhoPopulacao;
    sPop->numeroMortosTotal = 0;
    sPop->numeroDoentes = 0;
    sPop->numeroRecuperados = 0;

    sPop->numCriancas = 0;
    sPop->numAdolescentes = 0;
    sPop->numAdultos = 0;
    sPop->numIdosos = 0;
    sPop->numHomens = 0;
    sPop->numMulheres = 0;
    
    for(unsigned i = 0; i < P->tamanhoPopulacao; i++) {
        if(!P->I[i].vivo)
            continue;

        if(P->I[i].sexo) {
            sPop->numHomens++;
        } else {
            sPop->numMulheres++;
        }
        
        if(P->I[i].idade < 11) {
            sPop->numCriancas++;
        } else if(P->I[i].idade > 10 && P->I[i].idade < 18) {
            sPop->numAdolescentes++;
        } else if(P->I[i].idade > 17 && P->I[i].idade < 60) {
            sPop->numAdultos++;
        } else {
            sPop->numIdosos++;
        }
    }

    sPop->percentualCriancas = (sPop->numCriancas / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualAdolescentes = (sPop->numAdolescentes / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualAdultos = (sPop->numAdultos / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualIdosos = (sPop->numIdosos / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualMulheres = (sPop->numMulheres / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualHomens = (sPop->numHomens / (double) sPop->tamanhoPopulacaoAtual) * 100.0;

    return sPop;
}

void destroiStatusPopulacao(StatusPopulacao *sPop){
    if(sPop != NULL)
        free(sPop);
}

void printaStatusPopulacao(StatusPopulacao *sPop){
    if(sPop == NULL)
        return;
    printf("------------------------------------------------\n");
    printf("Tamanho população (Atual) : %u\n\n", sPop->tamanhoPopulacaoAtual);
    printf("Número de mortos          : %u\n", sPop->numeroMortosTotal);
    printf("Número de doentes         : %u\n", sPop->numeroDoentes);
    printf("Número de recuperados     : %u\n\n", sPop->numeroRecuperados);
    printf("Percentual crianças       : %.3lf\n", sPop->percentualCriancas);
    printf("Percentual adolescentes   : %.3lf\n", sPop->percentualAdolescentes);
    printf("Percentual adultos        : %.3lf\n", sPop->percentualAdultos);
    printf("Percentual idosos         : %.3lf\n\n", sPop->percentualIdosos);
    printf("Percentual homens         : %.3lf\n", sPop->percentualHomens);
    printf("Percentual mulheres       : %.3lf\n\n", sPop->percentualMulheres);
    printf("Taxa de mortalidade       : %.3lf\n", (sPop->numeroMortosTotal / (double) (sPop->numeroDoentes + sPop->numeroRecuperados)) * 100.0);
    if(sPop->numeroMortosTotal + sPop->tamanhoPopulacaoAtual >= 100000)
        printf("Taxa de letalidade        : %.3lf\n", (100000.0 * sPop->numeroMortosTotal) / (sPop->tamanhoPopulacaoAtual + sPop->numeroMortosTotal));
    printf("------------------------------------------------\n");
}

void atualizaStatusPopulacao(StatusPopulacao *sPop, Individuo morto){
    if(morto.vivo)
        return;
    sPop->tamanhoPopulacaoAtual --;
    sPop->numeroMortosTotal ++;

    morto.sexo? sPop->numHomens -- : sPop->numMulheres --;

    if(morto.idade < 11) {
            sPop->numCriancas--;
    } else if(morto.idade > 10 && morto.idade < 18) {
        sPop->numAdolescentes--;
    } else if(morto.idade > 17 && morto.idade < 60) {
        sPop->numAdultos--;
    } else {
        sPop->numIdosos--;
    }

    sPop->percentualCriancas = (sPop->numCriancas / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualAdolescentes = (sPop->numAdolescentes / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualAdultos = (sPop->numAdultos / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualIdosos = (sPop->numIdosos / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualMulheres = (sPop->numMulheres / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
    sPop->percentualHomens = (sPop->numHomens / (double) sPop->tamanhoPopulacaoAtual) * 100.0;
}