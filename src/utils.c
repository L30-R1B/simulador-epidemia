#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "../include/utils.h"
#include "../include/relacoes.h"
#include "../include/populacao.h"

#define TAMANHO_BUFFER 256

VariaveisAmbiente *varAmbiente = NULL;
FILE *arqOutput = NULL;

char dirNome[128];
char *arqMortesNome = "numMortesDiariamente.txt";
char *arqInfeccoesNome = "numNovosDoentesDiariamente.txt";
char *arqDoentesTotalNome = "numTotalDoentes.txt";
char *buffer = NULL;

void iniciaDir(char *dir){
    if (strlen(dir) >= sizeof(dirNome)) {
        fprintf(stderr, "Erro: Nome do diretório muito grande!\n");
        exit(1);
    }
    
    strncpy(dirNome, dir, sizeof(dirNome) - 1);
    dirNome[sizeof(dirNome) - 1] = '\0';
    
    buffer = (char*) malloc((strlen(dirNome) + 10) * sizeof(char));
    if (buffer == NULL) {
        perror("Erro ao alocar memória para buffer");
        exit(1);
    }
    
    sprintf(buffer, "mkdir %s", dirNome);
    system(buffer);
    free(buffer);
}

void iniciaArqMortesDiarias(){
    buffer = (char*) malloc(1024);
    if (buffer == NULL) {
        perror("Erro ao alocar memória para buffer");
        exit(1);
    }

    sprintf(buffer, "%s/%s", dirNome, arqMortesNome);
    
    arqOutput = fopen(buffer, "w");
    if (arqOutput == NULL) {
        perror("Erro ao abrir arquivo de mortes diárias");
        free(buffer);
        exit(1);
    }

    sprintf(buffer, "%s/5_graficoMortesTotal.png\nRED\n", dirNome);
    fprintf(arqOutput, "%s", buffer);
    fprintf(arqOutput, "Número de Mortes por Dia\n");
    fprintf(arqOutput, "Dia\n");
    fprintf(arqOutput, "Quantidade de Mortes\n");
    if (varAmbiente != NULL) {
        fprintf(arqOutput, "%u\n", varAmbiente->qtdDiasSimulacao);
    } else {
        fprintf(stderr, "Erro: varAmbiente não inicializado!\n");
        fclose(arqOutput);
        free(buffer);
        exit(1);
    }

    fclose(arqOutput);
    free(buffer);
}

void iniciaArqDoentesDiarios(){
    buffer = (char*) malloc(1024);
    if (buffer == NULL) {
        perror("Erro ao alocar memória para buffer");
        exit(1);
    }

    sprintf(buffer, "%s/%s", dirNome, arqInfeccoesNome);
    arqOutput = fopen(buffer, "w");
    if (arqOutput == NULL) {
        perror("Erro ao abrir arquivo de doentes diários");
        free(buffer);
        exit(1);
    }

    sprintf(buffer, "%s/3_graficoNovosCasosDiario.png\nGREEN\n", dirNome);
    fprintf(arqOutput, "%s", buffer);
    fprintf(arqOutput, "Número de Novos Casos por Dia\n");
    fprintf(arqOutput, "Dia\n");
    fprintf(arqOutput, "Quantidade de Novos Casos\n");
    if (varAmbiente != NULL) {
        fprintf(arqOutput, "%u\n", varAmbiente->qtdDiasSimulacao);
    } else {
        fprintf(stderr, "Erro: varAmbiente não inicializado!\n");
        fclose(arqOutput);
        free(buffer);
        exit(1);
    }

    fclose(arqOutput);
    free(buffer);
}

void iniciaArqTotalInfectados(){
    buffer = (char*) malloc(1024); // +2 para '/' e '\0'
    if (buffer == NULL) {
        perror("Erro ao alocar memória para buffer");
        exit(1);
    }

    sprintf(buffer, "%s/%s", dirNome, arqDoentesTotalNome);
    arqOutput = fopen(buffer, "w");
    if (arqOutput == NULL) {
        perror("Erro ao abrir arquivo de doentes diários");
        free(buffer);
        exit(1);
    }

    sprintf(buffer, "%s/4_graficoCasosTotal.png\nBLUE\n", dirNome);
    fprintf(arqOutput, "%s", buffer);
    fprintf(arqOutput, "Total de Casos\n");
    fprintf(arqOutput, "Dia\n");
    fprintf(arqOutput, "Número de Casos\n");
    if (varAmbiente != NULL) {
        fprintf(arqOutput, "%u\n", varAmbiente->qtdDiasSimulacao);
    } else {
        fprintf(stderr, "Erro: varAmbiente não inicializado!\n");
        fclose(arqOutput);
        free(buffer);
        exit(1);
    }

    fclose(arqOutput);
    free(buffer);
}

void adicionaInfoMortesDiaria(unsigned numMortes, unsigned dia){
    buffer = (char*) malloc((strlen(dirNome) + 64) * sizeof(char));
    sprintf(buffer, "%s/%s", dirNome, arqMortesNome);
    arqOutput = (FILE*) fopen(buffer, "a");
    fprintf(arqOutput, "%u %u\n", dia, numMortes);
    fclose(arqOutput);
    free(buffer);
}

void adicionaInfoNovosCasosDiario(unsigned numNovosCasos, unsigned dia){
    buffer = (char*) malloc((strlen(dirNome) + 64) * sizeof(char));
    sprintf(buffer, "%s/%s", dirNome, arqInfeccoesNome);
    arqOutput = (FILE*) fopen(buffer, "a");
    fprintf(arqOutput, "%u %u\n", dia, numNovosCasos);
    fclose(arqOutput);
    free(buffer);
}

void adicionaInfoTotalCasos(unsigned numNovosCasos, unsigned dia){
    buffer = (char*) malloc((strlen(dirNome) + 64) * sizeof(char));
    sprintf(buffer, "%s/%s", dirNome, arqDoentesTotalNome);
    arqOutput = (FILE*) fopen(buffer, "a");
    fprintf(arqOutput, "%u %u\n", dia, numNovosCasos);
    fclose(arqOutput);
    free(buffer);
}

void plotarGraficos(){
    buffer = (char*) malloc(1024 * sizeof(char));

    sprintf(buffer, "./grafico-plot < %s/%s", dirNome, arqInfeccoesNome);
    system(buffer);
    sprintf(buffer, "rm %s/%s", dirNome, arqInfeccoesNome);
    system(buffer);

    sprintf(buffer, "./grafico-plot < %s/%s", dirNome, arqMortesNome);
    system(buffer);
    sprintf(buffer, "rm %s/%s", dirNome, arqMortesNome);
    system(buffer);

    sprintf(buffer, "./grafico-plot < %s/%s", dirNome, arqDoentesTotalNome);
    system(buffer);
    sprintf(buffer, "rm %s/%s", dirNome, arqDoentesTotalNome);
    system(buffer);

    free(buffer);
}

int compressFile(const char *input_filename) {
    char *buffer = (char*) malloc(1024 * sizeof(char));
    if (buffer == NULL) {
        perror("Erro ao alocar memória para compressão");
        return -1;
    }

    char output_filename[128];
    sprintf(output_filename, "%s.gz", input_filename);

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Erro ao abrir o arquivo de entrada");
        free(buffer);
        return -1;
    }

    gzFile output_file = gzopen(output_filename, "wb");
    if (!output_file) {
        perror("Erro ao abrir o arquivo compactado de saída");
        fclose(input_file);
        free(buffer);
        return -1;
    }

    int bytes_read;
    while ((bytes_read = fread(buffer, 1, 1024, input_file)) > 0) { 
        if (gzwrite(output_file, buffer, bytes_read) != bytes_read) {
            perror("Erro ao escrever no arquivo gz");
            gzclose(output_file);
            fclose(input_file);
            free(buffer);
            return -1;
        }
    }

    sprintf(buffer, "rm %s", input_filename);
    system(buffer);

    gzclose(output_file);
    fclose(input_file);
    free(buffer);

    return 0;
}

void salvaListaRelacoes(void *list){
    ListaRelacoes *listRel = (ListaRelacoes*) list;

    buffer = (char*) malloc((strlen(dirNome) + 64) * sizeof(char));
    if (buffer == NULL) {
        perror("Erro ao alocar memória");
        return;
    }

    sprintf(buffer, "%s/6_listaAdjacencia.txt", dirNome);
    arqOutput = fopen(buffer, "w");
    if (arqOutput == NULL) {
        perror("Erro ao abrir arquivo para salvar lista de relações");
        free(buffer);
        return;
    }

    for(unsigned i = 0; i < listRel->tamanhoPopulacao; i++) {
        for(unsigned j = 0; j < listRel->LR[i].numRelacoes; j++) {
            fprintf(arqOutput, "%u %u\n", i, listRel->LR[i].indicesIndividuos[j]);
        }
    }

    fclose(arqOutput);
    
    compressFile(buffer);
    
    free(buffer);
}

void calculaCentralidade(){
    printf("INI\n");
    buffer = (char*) malloc(1024);
    sprintf(buffer, "./analise-redes %s/6_listaAdjacencia.txt.gz %s/centralidade data", dirNome, dirNome);
    system(buffer);
    printf("FIM\n");

    sprintf(buffer, "rm %s/6_listaAdjacencia.txt.gz", dirNome);
    system(buffer);

    free(buffer);
}

void salvarEstatisticaEpidemia(void *info, char *fName) {
    StatusPopulacao *sPop = (StatusPopulacao*) info;


    char *fileName = (char*) malloc(1024 * sizeof(char));
    sprintf(fileName, "%s/%s", dirNome, fName);

    if (sPop == NULL) {
        fprintf(stderr, "Erro: Informações da população não fornecidas!\n");
        return;
    }

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para salvar informações da epidemia");
        return;
    }

    fprintf(file, "------------------------------------------------\n");
    fprintf(file, "Tamanho população         : %u\n\n", sPop->tamanhoPopulacaoAtual);
    fprintf(file, "Número de mortos          : %u\n", sPop->numeroMortosTotal);
    fprintf(file, "Número de doentes         : %u\n", sPop->numeroDoentes);
    fprintf(file, "Número de recuperações    : %u\n\n", sPop->numeroRecuperados);
    fprintf(file, "Percentual crianças       : %.3lf\n", sPop->percentualCriancas);
    fprintf(file, "Percentual adolescentes   : %.3lf\n", sPop->percentualAdolescentes);
    fprintf(file, "Percentual adultos        : %.3lf\n", sPop->percentualAdultos);
    fprintf(file, "Percentual idosos         : %.3lf\n\n", sPop->percentualIdosos);
    fprintf(file, "Percentual homens         : %.3lf\n", sPop->percentualHomens);
    fprintf(file, "Percentual mulheres       : %.3lf\n\n", sPop->percentualMulheres);
    fprintf(file, "Taxa de mortalidade       : %.3lf\n", 
        (sPop->numeroMortosTotal / (double) (sPop->numeroDoentes + sPop->numeroRecuperados)) * 100.0);

    if (sPop->numeroMortosTotal + sPop->tamanhoPopulacaoAtual >= 100000) {
        fprintf(file, "Taxa de letalidade        : %.3lf\n", 
            (100000.0 * sPop->numeroMortosTotal) / (sPop->tamanhoPopulacaoAtual + sPop->numeroMortosTotal));
    }

    fprintf(file, "------------------------------------------------\n");

    free(fileName);
    fclose(file);
}

void salvarVariaveisAmbiente() {
    if (varAmbiente == NULL) {
        fprintf(stderr, "Erro: Variáveis do ambiente não fornecidas!\n");
        return;
    }

    char fileName[1024];
    sprintf(fileName, "%s/0_variaveisAmbienteUsadas.txt", dirNome);

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para salvar variáveis do ambiente");
        return;
    }

    fprintf(file, "------------------------------------------------\n");
    fprintf(file, "Quantidade de Dias da Simulação : %u\n", varAmbiente->qtdDiasSimulacao);
    fprintf(file, "Tamanho da População            : %u\n", varAmbiente->tamanhoPopulacao);
    fprintf(file, "Máximo de Pessoas no Círculo Social : %u\n", varAmbiente->maxPessoasCirculoSocial);
    fprintf(file, "Reinfecção Permitida (0 ou 1)   : %u\n", varAmbiente->reinfeccao);
    
    fprintf(file, "\n--- Percentuais por Faixa Etária ---\n");
    fprintf(file, "Percentual de Crianças          : %.3lf\n", varAmbiente->percentualCriancas);
    fprintf(file, "Percentual de Adolescentes      : %.3lf\n", varAmbiente->percentualAdolescentes);
    fprintf(file, "Percentual de Adultos           : %.3lf\n", varAmbiente->percentualAdultos);
    fprintf(file, "Percentual de Idosos            : %.3lf\n", varAmbiente->percentualIdosos);

    fprintf(file, "\n--- Outros Percentuais ---\n");
    fprintf(file, "Percentual de Pessoas com Comorbidade : %.3lf\n", varAmbiente->percentualComorbidade);
    fprintf(file, "Percentual de Homens                 : %.3lf\n", varAmbiente->percentualHomens);
    fprintf(file, "Percentual de Mulheres               : %.3lf\n", varAmbiente->percentualMulheres);
    
    fprintf(file, "\n--- Taxas ---\n");
    fprintf(file, "Taxa de Transmissão           : %.3lf\n", varAmbiente->taxaTransmissao);
    fprintf(file, "Taxa de Letalidade            : %.3lf\n", varAmbiente->taxaLetalidade);

    fprintf(file, "\n--- Vacinação ---\n");
    fprintf(file, "Percentual de Vacinados       : %.3lf\n", varAmbiente->percentualVacinados);
    fprintf(file, "Eficácia da Vacina            : %.3lf\n", varAmbiente->eficaciaVacina);

    fprintf(file, "\n--- Agravantes ---\n");
    fprintf(file, "Agravante para Homens         : %.3lf\n", varAmbiente->agravanteHomem);
    fprintf(file, "Agravante para Mulheres       : %.3lf\n", varAmbiente->agravanteMulher);
    fprintf(file, "Agravante por Idade           : %.3lf\n", varAmbiente->agravanteIdade);
    fprintf(file, "Agravante por Comorbidade     : %.3lf\n", varAmbiente->agravanteComorbidade);

    fprintf(file, "------------------------------------------------\n");

    fclose(file);
}

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

int inicializarVariaveisAmbiente() {
    carregar_variaveis_env("data.env");

    varAmbiente = malloc(sizeof(VariaveisAmbiente));
    if (varAmbiente == NULL) {
        perror("Erro ao alocar memória para varAmbiente");
        exit(EXIT_FAILURE);
    }

    char *valor;

    valor = getenv("QTD_DIAS_SIMULACAO");
    if (valor) varAmbiente->qtdDiasSimulacao = atoi(valor);

    valor = getenv("TAMANHO_POPULACAO");
    if (valor) varAmbiente->tamanhoPopulacao = atoi(valor);

    valor = getenv("MAXIMO_PESSOAS_CIRCULO_SOCIAL");
    if (valor) varAmbiente->maxPessoasCirculoSocial = atoi(valor);

    valor = getenv("REINFECCAO");
    if (valor) varAmbiente->reinfeccao = atoi(valor);

    valor = getenv("TEMPO_REINCIDENCIA");
    if (valor) varAmbiente->reincidencia = atoi(valor);

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

    valor = getenv("TAXA_LETALIDADE");
    if (valor) varAmbiente->taxaLetalidade = atof(valor);

    valor = getenv("PERCENTUAL_VACINADOS");
    if (valor) varAmbiente->percentualVacinados = atof(valor);

    valor = getenv("EFICACIA_VACINA");
    if (valor) varAmbiente->eficaciaVacina = atof(valor);

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