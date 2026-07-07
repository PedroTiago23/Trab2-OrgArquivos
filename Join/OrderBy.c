/*
Implementação da funcionalidade de Ordenação por Campo.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "JoinHeaders.h"

// Função de comparação para o qsort, relacionada ao codigo Estação
int compara_codEstacao(const void *a, const void *b) {
    REGISTRO *r1 = a;
    REGISTRO *r2 = b;
    
    // nulos aparecem depois dos valores não nulos
    if (r1->codEstacao == -1 && r2->codEstacao == -1) {
        return 0;
    }
    
    if (r1->codEstacao == -1) {
        return 1;
    }
    
    if (r2->codEstacao == -1) {
        return -1;
    }
    
    return r1->codEstacao - r2->codEstacao;
}

// Função de comparação para o qsort, mas agora relacionada ao codigo proxima Estação
int compara_codProxEstacao(const void *a, const void *b) {
    REGISTRO *r1 = a;
    REGISTRO *r2 = b;
    
    // Mesma lógica da função compara_codEstacao
    if (r1->codProxEstacao == -1 && r2->codProxEstacao == -1) {
        return 0;
    }
    
    if (r1->codProxEstacao == -1) {
        return 1;
    }
    
    if (r2->codProxEstacao == -1) {
        return -1;
    }
    
    return r1->codProxEstacao - r2->codProxEstacao;
}

void ORDER_BY()
{
    char arqEntradaNome[32];
    char campoOrd[32];
    char arqSaidaNome[32];
    
    scanf("%s %s %s", arqEntradaNome, campoOrd, arqSaidaNome);

    // Verifica os campos de ordenação 
    if (strcmp(campoOrd, "codEstacao") != 0 && strncmp(campoOrd, "codProxEst", 10) != 0) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Abre e verifica o arquivo de entrada que é o arquivo de dados
    FILE *arqEntrada = fopen(arqEntradaNome, "rb");
    if (arqEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // define e lê o cabeçalho do arquivo de dados e verifica seu status 
    // antes de fazer qualquer coisa
    CABECALHO cab;
    lerCabecalhoBin(arqEntrada, &cab);
    
    if (cab.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arqEntrada);
        return;
    }

    // Define o array de ordenação e joga os valores lidos do disco para a RAM
    // Só são válidos registros que não foram logicamente removidos
    REGISTRO *vetorOrdenacao = malloc(cab.proxRRN * sizeof(REGISTRO));
    int qtdValidos = 0;
    
    for (int i = 0; i < cab.proxRRN; i++) {
        REGISTRO reg;
        LerRegistroBin(arqEntrada, &reg, i);
        
        if (reg.removido == '0') {
            vetorOrdenacao[qtdValidos] = reg; 
            qtdValidos++;
        } else {
            // Se removido, limpamos memória das strings
            liberaStringsRegistro(&reg); 
        }
    }
    fclose(arqEntrada);

    // ordenando conforme o campo escolhido usando o quicksort da biblioteca stdlib
    // o quicksort usa as funções de comparação para saber qual é maior, menor ou nulo para ordenar
    if (strcmp(campoOrd, "codEstacao") == 0) {
        qsort(vetorOrdenacao, qtdValidos, sizeof(REGISTRO), compara_codEstacao);
    } else {
        qsort(vetorOrdenacao, qtdValidos, sizeof(REGISTRO), compara_codProxEstacao);
    }

    // Escrevendo o vetor ordenado no arquivo de saida(novo arquivo binário)
    FILE *arqSaida = fopen(arqSaidaNome, "wb");
    if (arqSaida == NULL) {
        printf("Falha no processamento do arquivo.\n");
        
        for(int i = 0; i < qtdValidos; i++) {
            liberaStringsRegistro(&vetorOrdenacao[i]);
        }
        free(vetorOrdenacao);
        
        return;
    }

    // Atualizando o cabecalho do novo arquivo de saida
    CABECALHO cabSaida;
    cabSaida.status = '0';
    cabSaida.topo = -1;
    cabSaida.proxRRN = qtdValidos; // No novo arquivo, proxRRN será a contagem contínua de válidos
    cabSaida.nroEstacoes = cab.nroEstacoes;
    cabSaida.nroParesEstacoes = cab.nroParesEstacoes;
    
    atualizarCabecalho(&cabSaida, arqSaida);

    // Escreve no arquivo de saida o vetor ordenado e libera as strings e o vetor depois 
    for (int i = 0; i < qtdValidos; i++) {
        EscreverRegistroBin(arqSaida, &vetorOrdenacao[i]);
        // Libera as strings após transferir para o disco
        liberaStringsRegistro(&vetorOrdenacao[i]); 
    }
    free(vetorOrdenacao);

    // Terminado todo o processo, atualiza o cabeçalho do arquivo de saida para consistente 
    // Fecha o arquivo e imprime o binário na tela
    cabSaida.status = '1';
    atualizarCabecalho(&cabSaida, arqSaida);
    fclose(arqSaida);

    BinarioNaTela(arqSaidaNome);
    
    return;
}