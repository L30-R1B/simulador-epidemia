#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/relacoes.h"

int pesquisaRelacao(ListaRelacoes *listRel, unsigned i, unsigned j){
    for(int k = 0; k < listRel->LR[i].numRelacoes; k ++){
        if(listRel->LR[i].indicesIndividuos[k] == j)
            return k;
    }
    return -1;
}

int verificaExistenciaRelacao(ListaRelacoes *listRel, unsigned i, unsigned j){
    for(int k = 0; k < listRel->LR[i].numRelacoes; k ++){
        if(listRel->LR[i].indicesIndividuos[k] == j)
            return 1;
    }
    return 0;
}

int adicionaRelacao(ListaRelacoes *listRel, unsigned i, unsigned j){
    listRel->LR[i].indicesIndividuos[listRel->LR[i].numRelacoes] = j;
    listRel->LR[i].numRelacoes ++;
    listRel->numTotalRelacoes ++;
    return 1;
}

int removeRelacao(ListaRelacoes *listRel, unsigned i, unsigned j){
    int indice = pesquisaRelacao(listRel, i, j);
    if(indice == -1)
        return 0;
    
    for(int k = indice; k < listRel->LR[i].numRelacoes - 1; k ++){
        listRel->LR[i].indicesIndividuos[k] = listRel->LR[i].indicesIndividuos[k + 1];
    }
    listRel->LR[i].numRelacoes --;
    listRel->numTotalRelacoes --;
    return 1;
}

int verificaEscolha(ListaRelacoes *listRel, unsigned individuo, unsigned escolha){
    if(!listRel->LR[escolha].vivo)
        return 0;
    else if(listRel->LR[individuo].numRelacoes > (listRel->LR[individuo].maxRelacoes - 1))
        return 0;
    else if(listRel->LR[escolha].numRelacoes > (listRel->LR[escolha].maxRelacoes - 1))
        return 0;
    else if(individuo == escolha)
        return 0;
    else if(verificaExistenciaRelacao(listRel, individuo, escolha))
        return 0;
    return 1;
}

unsigned escolheRelacao(ListaRelacoes *listRel, unsigned i){
    unsigned escolha;
    for(unsigned k = 0; k < listRel->LR[i].maxRelacoes; k ++){
        escolha = rand() % listRel->tamanhoPopulacao;
        if(verificaEscolha(listRel, i, escolha))
            return escolha;
    }
    return listRel->tamanhoPopulacao + 1;
}

void atribuiRelacoes(ListaRelacoes *listRel){
    unsigned escolha = 0;
    for(unsigned i = 0; i < listRel->tamanhoPopulacao; i ++){
        for(unsigned j = 0; j < listRel->LR[i].maxRelacoes; j ++){

            escolha = escolheRelacao(listRel, i);

            if(escolha == listRel->tamanhoPopulacao + 1)
                break;

            adicionaRelacao(listRel, i, escolha);
            adicionaRelacao(listRel, escolha, i);
        }
    }
}

ListaRelacoes *criaListaRelacoes(Populacao P){
    ListaRelacoes *listRel = (ListaRelacoes *) malloc(sizeof(ListaRelacoes));
    listRel->numTotalRelacoes = 0;
    listRel->LR = (LinhaRelacoes *) malloc(P.tamanhoPopulacao * sizeof(LinhaRelacoes));
    listRel->tamanhoPopulacao = P.tamanhoPopulacao;

    for(unsigned i = 0; i < P.tamanhoPopulacao; i ++){
        listRel->LR[i].vivo = 1;
        listRel->LR[i].maxRelacoes = NUM_RELACOES_INDIVIDUO(P.I[i].nivelInteracoesSociais);
        listRel->LR[i].numRelacoes = 0;
        listRel->LR[i].indicesIndividuos = (unsigned*) malloc(listRel->LR[i].maxRelacoes * sizeof(unsigned));
        listRel->numTotalRelacoes += listRel->LR[i].numRelacoes;
    }

    atribuiRelacoes(listRel);

    return listRel;
}

void trocaRelacoesIndividuo(ListaRelacoes *listRel, unsigned individuo){
    unsigned indiceRemocao = listRel->LR[individuo].numRelacoes / 2;
    for(unsigned i = indiceRemocao; i < listRel->LR[individuo].numRelacoes; i ++){
        removeRelacao(listRel, individuo, listRel->LR[individuo].indicesIndividuos[indiceRemocao]);
        removeRelacao(listRel, listRel->LR[individuo].indicesIndividuos[indiceRemocao], individuo);
        removeRelacao(listRel, individuo, listRel->LR[individuo].indicesIndividuos[indiceRemocao]);
        removeRelacao(listRel, listRel->LR[individuo].indicesIndividuos[indiceRemocao], individuo);
        removeRelacao(listRel, individuo, listRel->LR[individuo].indicesIndividuos[indiceRemocao]);
        removeRelacao(listRel, listRel->LR[individuo].indicesIndividuos[indiceRemocao], individuo);
        removeRelacao(listRel, individuo, listRel->LR[individuo].indicesIndividuos[indiceRemocao]);
        removeRelacao(listRel, listRel->LR[individuo].indicesIndividuos[indiceRemocao], individuo);
    }

    unsigned indiceIndividuoTroca = individuo;
    unsigned indiceTroca = individuo;
    unsigned char quebra;
    for(unsigned i = indiceRemocao; i < listRel->LR[individuo].maxRelacoes; i ++){
        quebra = 1;
        for(unsigned j = 0; j < 10; j ++){
            indiceIndividuoTroca = rand() % listRel->tamanhoPopulacao;
            if(!listRel->LR[indiceIndividuoTroca].numRelacoes)
                continue;
            indiceTroca = rand() % listRel->LR[indiceIndividuoTroca].numRelacoes;            
            if(!listRel->LR[individuo].vivo)
                continue;
            else if(listRel->LR[individuo].numRelacoes > (listRel->LR[individuo].maxRelacoes - 1))
                continue;
            else if(individuo == listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca])
                continue;
            else if(verificaExistenciaRelacao(listRel, individuo, listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca]))
                continue;
            quebra = 0;
            break;
        }
        if(quebra)
            break;

        removeRelacao(listRel, listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca], indiceIndividuoTroca);
        adicionaRelacao(listRel, individuo, listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca]);
        adicionaRelacao(listRel, listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca], individuo);
        removeRelacao(listRel, indiceIndividuoTroca, listRel->LR[indiceIndividuoTroca].indicesIndividuos[indiceTroca]);
    }
}

void destroiListaRelacoes(ListaRelacoes *listRel){
    if(listRel == NULL)
        return;
    if(listRel->LR == NULL){
        free(listRel);
        return;
    }
    for(unsigned i = 0; i < listRel->tamanhoPopulacao; i ++){
        if(listRel->LR[i].indicesIndividuos == NULL)
            continue;
        free(listRel->LR[i].indicesIndividuos);
    }

    free(listRel->LR);
    free(listRel);
}

void printaListaRelacoes(ListaRelacoes *listRel){
    printf("------------------------------------------------------------\n");
    printf("TOTAL RELAÇÔES : %lu\n\n", listRel->numTotalRelacoes);
    for(unsigned i = 0; i < listRel->tamanhoPopulacao; i ++){
        printf("%.2u <<%.2u<<%.2u<< ", i, listRel->LR[i].maxRelacoes, listRel->LR[i].numRelacoes);
        for(unsigned j = 0; j < listRel->LR[i].numRelacoes; j ++){
            printf(" %.2u ", listRel->LR[i].indicesIndividuos[j]);
        }
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
}