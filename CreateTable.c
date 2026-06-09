/*
Implementação da funcionalidade 1 de Criação de Tabela
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "Estruturas.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função usada para ler um único campo de inteiro no arquivo .csv
void lerIntCSV(int* campoRegistro, char** separationPtr)
{
    char* campo;
    campo = strsep(separationPtr, ",");
    // Apenas armazena caso seja um campo totalmente válido, senão considera-se um campo nulo
    *campoRegistro = (campo && *campo != '\0' && *campo != '\n' && *campo != '\r') ? atoi(campo) : -1;
    return;
}

// Função para ler e armazenar um único registro de uma linha no formato .csv
void LerRegistroCSV(char *linha, REGISTRO *reg)
{
    // String da linha inteira
    char *ptr = linha;
    // String de cada campo da linha que foi separado das vírgulas usando strsep()
    char *campo;
    
    // Não é necessário o uso da função já que sabemos que codEstacao nunca será nulo, como diz a especificação
    campo = strsep(&ptr, ",");  
    reg->codEstacao = atoi(campo);  
    
    // Aqui, lógica para o nome da estação
    campo = strsep(&ptr, ",");
    reg->tamNomeEstacao = strlen(campo);
    reg->nomeEstacao = malloc(reg->tamNomeEstacao + 1);
    strcpy(reg->nomeEstacao, campo);
    
    lerIntCSV(&reg->codLinha, &ptr);
    
    campo = strsep(&ptr, ",");
    // Verificação segura do ponteiro e da string
    if(campo && *campo != '\0' && *campo != '\n' && *campo != '\r')
    {
        reg->tamNomeLinha = strlen(campo);
        reg->nomeLinha = malloc(reg->tamNomeLinha + 1);
        strcpy(reg->nomeLinha, campo);
    }
    else    
    {
        reg->tamNomeLinha = 0;  
        reg->nomeLinha = NULL; // Garante que não aponta para lixo
    }
    
    lerIntCSV(&reg->codProxEstacao, &ptr);    
    lerIntCSV(&reg->distProxEstacao, &ptr);
    lerIntCSV(&reg->codLinhaIntegra, &ptr);
    lerIntCSV(&reg->codEstIntegra, &ptr);

    return;
}

// Trecho principal para funcionalidade de criar uma tabela em binário a partir de um arquivo .csv
void CREATE_TABLE()
{
    char arqCSV_nome[32], arqBIN_nome[32];
    scanf("%s %s", arqCSV_nome, arqBIN_nome);

    FILE* arqCSV = fopen(arqCSV_nome, "r");
    FILE* arqBIN = fopen(arqBIN_nome, "wb+");
    
    if(arqCSV == NULL || arqBIN == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Inicializando o cabeçalho para o arquivo binário.
    CABECALHO cabecalhoInicial;
    cabecalhoInicial.status = '0';
    cabecalhoInicial.topo = -1;
    cabecalhoInicial.proxRRN = 0;
    cabecalhoInicial.nroEstacoes = 0;
    cabecalhoInicial.nroParesEstacoes = 0;
    atualizarCabecalho(arqBIN, &cabecalhoInicial);

    REGISTRO RegTransferencia;
    RegTransferencia.removido = '0';
    RegTransferencia.proximo = -1;
    
    char linha[TAM_MAX_LINHA_CSV];
    
    // Primeira chamada de fgets() para consumir e ignorar a primeira linha contendo a ordem dos campos.
    fgets(linha, TAM_MAX_LINHA_CSV, arqCSV); 

    // Loop para ler o arquivo inteiro
    while(fgets(linha, TAM_MAX_LINHA_CSV, arqCSV) != NULL)
    {
        // Evita que em algum momento armazene uma quebra de linha no final de uma linha ou no último registro do .csv
        linha[strcspn(linha, "\r\n")] = '\0';

        LerRegistroCSV(linha, &RegTransferencia);
        EscreverRegistroBin(arqBIN, &RegTransferencia);
        cabecalhoInicial.proxRRN++;

        // Libera a memória das strings recém-lidas do CSV nesta iteração
        free(RegTransferencia.nomeEstacao);
        if (RegTransferencia.nomeLinha != NULL)
            free(RegTransferencia.nomeLinha);
    }
    
    // Varremos o arquivo binário para definir os campos nroEstacoes e nroParesEstacoes do cabeçalho.
    recalcularContadores(arqBIN, &cabecalhoInicial);
    
    cabecalhoInicial.status = '1';
    // Gravar o cabeçalho de volta no disco para consolidar as alterações
    atualizarCabecalho(arqBIN, &cabecalhoInicial);

    fclose(arqCSV);
    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);

    return;
}