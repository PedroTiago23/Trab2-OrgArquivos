// Funcionalidade 7 - Criação do Índice

#include "BTree.h"
#include "ArquivoIO.h"
#include "fornecidas.h"

// Inicializa o arquivo de índice zerado e com status inconsistente ('0')
void criarIndiceVazio(FILE* arqIndice) {
    CABECALHO_ARVOREB cabecalho;
    cabecalho.status = '0';
    cabecalho.noRaiz = -1;
    cabecalho.topo = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroNos = 0;
    
    fseek(arqIndice, 0, SEEK_SET);

    #define ESCREVE_CAB(item) \
        fwrite(&cabecalho.item, sizeof(char), 1, arqIndice);
    
    ESCREVE_CAB(status);
    ESCREVE_CAB(noRaiz);
    ESCREVE_CAB(topo);
    ESCREVE_CAB(proxRRN);
    ESCREVE_CAB(nroNos);
}

void CREATE_INDEX() {
    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb");
    FILE* arqIndice = fopen(arqIndiceNome, "wb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do Arquivo.\n");
        return;
    }

    CABECALHO cabDados;
    lerCabecalhoBin(arqDados, &cabDados);

    // Escreve o cabeçalho inicial vazio no byte 0 do índice
    criarIndiceVazio(arqIndice);

    REGISTRO reg;
    int rrnDados = 0;

    // percorre sequencialmente o arquivo de dados (estacoes.bin)
    while (rrnDados < cabDados.proxRRN) {
        LerRegistroBin(arqDados, &reg, rrnDados);

        // Apenas registros ativos são indexados
        if (reg.removido == '0') {
            // Aqui fica a parte de inserir na árvore 
        }

        liberaStringsRegistro(&reg); // Importante para não dar memory leak
        rrnDados++;
    }

    // Marca o índice como consistente e fecha tudo
    fseek(arqIndice, 0, SEEK_SET);
    char statusOk = '1';
    fwrite(&statusOk, sizeof(char), 1, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);

    BinarioNaTela(arqIndiceNome);
}