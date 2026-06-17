/*
Funcionalidade 9 - Inserção usando o arquivo de índice.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "IntoInsert.h"

void INSERT_INDEX() {

    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb+");
    FILE* arqIndice = fopen(arqIndiceNome, "rb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalhoDados;
    lerCabecalhoBin(arqDados, &cabecalhoDados);
    
    CABECALHO_ARVOREB cabecalhoIndice;
    lerCabecalhoIndice(arqIndice, &cabecalhoIndice);

    // Defini como incosistente o arquivo antes de começar a mexer
    cabecalhoDados.status = '0';
    
    atualizarCabecalho(&cabecalhoDados, arqDados);

    cabecalhoIndice.status = '0';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    // loop para inserir 
    int qtdInsercoes;
    scanf("%d", &qtdInsercoes);

    for (int i = 0; i < qtdInsercoes; i++) {
        REGISTRO novoReg;

        // função do trabalho 1 de ler o registro do terminal
        lerRegistroTerminal(&novoReg); 

        int rrnInserido = -1; 

        // escrevendo no disco
        if (cabecalhoDados.topo == -1) {
            // Se não tem registro removido, insere no final
            rrnInserido = cabecalhoDados.proxRRN;
            fseek(arqDados, 17 + (80 * rrnInserido), SEEK_SET);
            EscreverRegistroBin(arqDados, &novoReg);
            
            cabecalhoDados.proxRRN++;
        } else {
            rrnInserido = cabecalhoDados.topo;
            
            // Calcula o byte onde o registro removido começa
            long offsetRemovido = 17 + (rrnInserido * 80);
            fseek(arqDados, offsetRemovido, SEEK_SET);

            // Ler o RRN removido que estava salvo lá.
            char statusRemovido;
            fread(&statusRemovido, sizeof(char), 1, arqDados);
            int proximoRRN;
            fread(&proximoRRN, sizeof(int), 1, arqDados);

            // Atualiza os dados do cabeçalho
            cabecalhoDados.topo = proximoRRN;
            
            // Sobrescreve o registro no rrnInserido com o novoReg.
            fseek(arqDados, offsetRemovido, SEEK_SET);
            EscreverRegistroBin(arqDados, &novoReg);
        }

        // Atualiza o índice
        inserirNaArvoreB(arqIndice, &cabecalhoIndice, novoReg.codEstacao, rrnInserido);

        // limpa a memória
        liberaStringsRegistro(&novoReg);
    }

    // Fecha tudo e deixa como status consistente 
    cabecalhoDados.status = '1';
    recalcularContadores(arqDados, &cabecalhoDados);
    atualizarCabecalho(&cabecalhoDados, arqDados);

    cabecalhoIndice.status = '1';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);

    BinarioNaTela(arqDadosNome);
    BinarioNaTela(arqIndiceNome);
}