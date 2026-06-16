/*
Funcionalidade 7 - Criação do Índice
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "CreateIndex.h"

// Trecho principal para a funcionalidade de criar um arquivo de índice para
// o arquivo de dados do trabalho introdutório. 
void CREATE_INDEX() 
{
    // Leitura do arquivo de dados e criação/leitura do arquivo de índice.
    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb");
    FILE* arqIndice = fopen(arqIndiceNome, "wb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalhoDados;
    lerCabecalhoBin(arqDados, &cabecalhoDados);

    // Inicializa o arquivo de índice zerado e com status inconsistente ('0').
    CABECALHO_ARVOREB cabecalhoTree;
    cabecalhoTree.status = '0';
    cabecalhoTree.noRaiz = -1;
    cabecalhoTree.topo = -1;
    cabecalhoTree.proxRRN = 0;
    cabecalhoTree.nroNos = 0;
    
    atualizaCabecalhoIndice(&cabecalhoTree, arqIndice);

    REGISTRO reg;
    int rrnDados = 0;

    // Percorre sequencialmente o arquivo de dados.
    while (rrnDados < cabecalhoDados.proxRRN) {
        LerRegistroBin(arqDados, &reg, rrnDados);

        // Apenas registros ativos são indexados.
        if (reg.removido == '0')
            inserirNaArvoreB(arqIndice, &cabecalhoTree, reg.codEstacao, rrnDados);

        liberaStringsRegistro(&reg); // Para não dar memory leak.
        rrnDados++;
    }

    // Atualiza os dados no cabeçalho do arquivo de índice.
    cabecalhoTree.status = '1';
    atualizaCabecalhoIndice(&cabecalhoTree, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);

    BinarioNaTela(arqIndiceNome);
}