/*
Implementação da funcionalidade 4 de Insert 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "Estruturas.c"

// Função usada para ler todos os campos na ordem certa diretamente do terminal onde o 
// código está sendo rodado
void lerRegistroTerminal(REGISTRO* novoRegistro)
{
    novoRegistro->removido = '0';
    novoRegistro->proximo = -1;

    char strAtual[50] = "";

    #define LE_INT(StringAtual, campo) \
        ScanQuoteString(StringAtual); \
        leIntCampoBusca(&novoRegistro->campo, StringAtual);

    #define LE_STRING(StringAtual, campo1, campo2) \
        ScanQuoteString(StringAtual); \
        leStringCampoBusca(&novoRegistro->campo1, &novoRegistro->campo2, StringAtual);
    
        
    LE_INT(strAtual, codEstacao);
    LE_STRING(strAtual, tamNomeEstacao, nomeEstacao);
    
    LE_INT(strAtual, codLinha);
    LE_STRING(strAtual, tamNomeLinha, nomeLinha)
    
    LE_INT(strAtual, codProxEstacao);
    LE_INT(strAtual, distProxEstacao);
    LE_INT(strAtual, codLinhaIntegra);
    LE_INT(strAtual, codEstIntegra);

    #undef LE_INT
    #undef LE_STRING

    return;
}

// Trecho principal da funcionalidade de Insert, que adiciona um novo registro
// determinado pelo terminal em um arquivo binário, a partir da abordagem dinâmica 
// de reaproveitamento de espaço.
void INSERT()
{
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"rb+")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    rewind(arqBIN);

    // Define o arquivo como inconsistente após abri-lo para escrita
    char status = '0';
    fwrite(&status, sizeof(char), 1, arqBIN);

    // Define quantos registros iremos inserir
    int qtdInserts;
    scanf("%d", &qtdInserts);

    int contador = 0;

    while(contador < qtdInserts)
    {
        REGISTRO registroInserido;
        lerRegistroTerminal(&registroInserido);
        
        // Caso não exista nenhum registro já removido, apenas inserimos na posição proxRRN
        if(cabecalho.topo == -1)
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.proxRRN*TAM_REGISTRO, SEEK_SET);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.proxRRN++;
        }
        else    // Caso exista um removido, inserimos naquele espaço e desempilhamos um registro do campo topo no cabeçalho
        {
            fseek(arqBIN, TAM_CABECALHO+cabecalho.topo*TAM_REGISTRO+1, SEEK_SET);
            int regRemovidoProximo;
            fread(&regRemovidoProximo, sizeof(int), 1, arqBIN);
            fseek(arqBIN, -5, SEEK_CUR);
            EscreverRegistroBin(arqBIN, &registroInserido);
            cabecalho.topo = regRemovidoProximo;
        }

        free(registroInserido.nomeEstacao);
        free(registroInserido.nomeLinha);

        contador++;
    }
    cabecalho.status = '1';

    recalcularContadores(arqBIN, &cabecalho);

    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);

    BinarioNaTela(arqBIN_nome);
    
    return;
}