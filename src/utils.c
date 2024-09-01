#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

VariaveisAmbiente *varAmbiente = NULL;

void carregar_variaveis_env(const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo .env");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), fp)) {

        if (linha[0] == '\n' || linha[0] == '#') continue;

        linha[strcspn(linha, "\n")] = 0;

        char *igual = strchr(linha, '=');
        if (igual != NULL) {
            *igual = '\0';
            char *chave = linha;
            char *valor = igual + 1;

            if (setenv(chave, valor, 1) != 0) {
                perror("Erro ao definir a variável de ambiente");
            }
        }
    }

    fclose(fp);
}

/*TAMANHO_POPULACAO=10
PERCENTUAL_CRIANCAS=15.5
PERCENTUAL_ADOLESCENTES=20.0
PERCENTUAL_ADULTOS=40.5
PERCENTUAL_IDOSOS=24.0
PERCENTUAL_COMORBIDADE=10.0
PERCENTUAL_HOMENS=48.0
PERCENTUAL_MULHERES=52.0
TAXA_TRANSMISSAO=1.2
TAXA_MORTALIDADE=0.05
TAXA_CONTATO_SOCIAL=0.7
PERCENTUAL_VACINADOS=75.0
AGRAVANTE_HOMEM=2.0
AGRAVANTE_MULHER=1.0
AGRAVANTE_IDADE=1.5
AGRAVANTE_COMORBIDADE=2.0*/

int inicializarVariaveisAmbiente() {
    carregar_variaveis_env("data.env");

    // Aloca memória para varAmbiente
    varAmbiente = malloc(sizeof(VariaveisAmbiente));
    if (varAmbiente == NULL) {
        perror("Erro ao alocar memória para varAmbiente");
        exit(EXIT_FAILURE);
    }

    char *valor;

    valor = getenv("TAMANHO_POPULACAO");
    if (valor) varAmbiente->tamanhoPopulacao = atoi(valor);

    valor = getenv("MAXIMO_PESSOAS_CIRCULO_SOCIAL");
    if (valor) varAmbiente->maxPessoasCirculoSocial = atoi(valor);

    valor = getenv("REINFECCAO");
    if (valor) varAmbiente->reinfeccao = atoi(valor);

    valor = getenv("PERCENTUAL_CRIANCAS");
    if (valor) varAmbiente->percentualCriancas = atof(valor);

    valor = getenv("PERCENTUAL_ADOLESCENTES");
    if (valor) varAmbiente->percentualAdolescentes = atof(valor);

    valor = getenv("PERCENTUAL_ADULTOS");
    if (valor) varAmbiente->percentualAdultos = atof(valor);

    valor = getenv("PERCENTUAL_IDOSOS");
    if (valor) varAmbiente->percentualIdosos = atof(valor);

    valor = getenv("PERCENTUAL_COMORBIDADE");
    if (valor) varAmbiente->percentualComorbidade = atof(valor);

    valor = getenv("PERCENTUAL_HOMENS");
    if (valor) varAmbiente->percentualHomens = atof(valor);

    valor = getenv("PERCENTUAL_MULHERES");
    if (valor) varAmbiente->percentualMulheres = atof(valor);

    valor = getenv("TAXA_TRANSMISSAO");
    if (valor) varAmbiente->taxaTransmissao = atof(valor);

    valor = getenv("TAXA_MORTALIDADE");
    if (valor) varAmbiente->taxaMortalidade = atof(valor);

    valor = getenv("TAXA_LETALIDADE");
    if (valor) varAmbiente->taxaLetalidade = atof(valor);

    valor = getenv("PERCENTUAL_VACINADOS");
    if (valor) varAmbiente->percentualVacinados = atof(valor);

    valor = getenv("AGRAVANTE_HOMEM");
    if (valor) varAmbiente->agravanteHomem = atof(valor);

    valor = getenv("AGRAVANTE_MULHER");
    if (valor) varAmbiente->agravanteMulher = atof(valor);

    valor = getenv("AGRAVANTE_IDADE");
    if (valor) varAmbiente->agravanteIdade = atof(valor);

    valor = getenv("AGRAVANTE_COMORBIDADE");
    if (valor) varAmbiente->agravanteComorbidade = atof(valor);

    return 0;
}

void finalizaVariaveisAmbiente(){
    if(varAmbiente != NULL)
        free(varAmbiente);
}