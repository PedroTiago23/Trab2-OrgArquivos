/*
Módulo de I/O de Arquivos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.c"

void imprimeCampoInt(int campo) {
    if(campo != -1)
        printf("%d ", campo);
    else 
        printf("%s ", CAMPO_NULO);
    return;
}

// Imprime um registro na ordem certa a partir de uma struct 
// de registro lido na memória principal
void imprimirRegistro(REGISTRO* reg) {
    printf("%d %s ", reg->codEstacao, reg->nomeEstacao);
    
    imprimeCampoInt(reg->codLinha);

    if(reg->tamNomeLinha)
        printf("%s ", reg->nomeLinha);
    else printf("%s ", CAMPO_NULO);

    imprimeCampoInt(reg->codProxEstacao);
    imprimeCampoInt(reg->distProxEstacao);
    imprimeCampoInt(reg->codLinhaIntegra);

    // Não usando a função de impressão para não imprimir um espaço a mais no final
    if(reg->codEstIntegra != -1)
        printf("%d", reg->codEstIntegra);
    else printf("%s", CAMPO_NULO);
    printf("\r\n");

    return;
}


// Lê o cabeçalho do arquivo em binário e guarda os dados coletados
// em uma struct com os respectivos campos de um registro de cabeçalho
void lerCabecalhoBin(FILE* arqBIN, CABECALHO* cabecalho) {
    fseek(arqBIN, 0, SEEK_SET);
    
    fread(&cabecalho->status, sizeof(char), 1, arqBIN);
    
    #define LER_CAB_INT(campo) \
        fread(&cabecalho->campo, sizeof(int), 1, arqBIN)

    LER_CAB_INT(topo);
    LER_CAB_INT(proxRRN);
    LER_CAB_INT(nroEstacoes);
    LER_CAB_INT(nroParesEstacoes);

    #undef LER_CAB_INT
    return;
}

// Função para sobrescrever o registro de cabeçalho do arquivo binário com novos dados
void atualizarCabecalho(FILE* arqBIN, CABECALHO* regCabecalho) {
    fseek(arqBIN, 0, SEEK_SET);
    
    fwrite(&regCabecalho->status, sizeof(char), 1, arqBIN);
    
    #define ESCREVE_CAB_INT(campo) \
        fwrite(&regCabecalho->campo, sizeof(int), 1, arqBIN)

    ESCREVE_CAB_INT(topo);
    ESCREVE_CAB_INT(proxRRN);
    ESCREVE_CAB_INT(nroEstacoes);
    ESCREVE_CAB_INT(nroParesEstacoes);

    #undef ESCREVE_CAB_INT
    return;
}


// Lê um registro completo de dado RRN no arquivo em binário e armazena em uma struct registro especificada 
void LerRegistroBin(FILE* arqBIN, REGISTRO* reg, int PosicaoRRN) {
    fseek(arqBIN, 17 + (PosicaoRRN * 80), SEEK_SET); // Coloca o ponteiro em qual RRN estiver em PosicaoRRN
        
    fread(&reg->removido, sizeof(char), 1, arqBIN);
    
    #define LER_REG_INT(campo) \
        fread(&reg->campo, sizeof(int), 1, arqBIN)

    LER_REG_INT(proximo);
    LER_REG_INT(codEstacao);
    LER_REG_INT(codLinha);
    LER_REG_INT(codProxEstacao);
    LER_REG_INT(distProxEstacao);
    LER_REG_INT(codLinhaIntegra);
    LER_REG_INT(codEstIntegra);

    // Para os campos variáveis
    // No caso de nome estação, não nos preocupamos em checar por nulo já que nunca serão nulos
    LER_REG_INT(tamNomeEstacao);

    #undef LER_REG_INT

    // Leitura das strings de tamanho variável
    reg->nomeEstacao = (char*) malloc(reg->tamNomeEstacao + 1);
    fread(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
    reg->nomeEstacao[reg->tamNomeEstacao] = '\0';

    // Nome da linha
    fread(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    
    // Verificação de se é um campo nulo
    if(reg->tamNomeLinha) {
        reg->nomeLinha = (char*) malloc(reg->tamNomeLinha + 1);
        fread(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
    } else {
        reg->nomeLinha = malloc(LEN_CAMPO_NULO);
        strcpy(reg->nomeLinha, CAMPO_NULO);
    }
    return;
}

// Função para escrever um registro no arquivo binário a partir de uma estrutura registro
void EscreverRegistroBin(FILE *arqBIN, REGISTRO *reg) {

    // Contador usado para determinar quantos bytes de lixo escrever no final do registro
    int BytesEscritos = 0;

    
    #define ESCREVE_CHAR(campo) \
        fwrite(&reg->campo, sizeof(char), 1, arqBIN); \
        BytesEscritos += sizeof(char)

    #define ESCREVE_INT(campo) \
        fwrite(&reg->campo, sizeof(int), 1, arqBIN); \
        BytesEscritos += sizeof(int)

    // Sequência de writes na ordem certa
    ESCREVE_CHAR(removido);
    ESCREVE_INT(proximo);
    ESCREVE_INT(codEstacao);
    ESCREVE_INT(codLinha);
    ESCREVE_INT(codProxEstacao);
    ESCREVE_INT(distProxEstacao);
    ESCREVE_INT(codLinhaIntegra);
    ESCREVE_INT(codEstIntegra);
    ESCREVE_INT(tamNomeEstacao);

    #undef ESCREVE_CHAR
    #undef ESCREVE_INT

    
    fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
    BytesEscritos += reg->tamNomeEstacao;
    
    fwrite(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    BytesEscritos += sizeof(int);
    
    if (reg->tamNomeLinha > 0 && reg->nomeLinha != NULL) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
        BytesEscritos += reg->tamNomeLinha;
    }

    // Preenche o resto do registro exato com lixo
    for(int i = 0; i < TAM_REGISTRO - BytesEscritos; i++)
        fputc('$', arqBIN);
    
    return;
}

// Função que pula para um registro de RRN especificado e 
// marca seus campos de removido e proximo devidamente
void removerRegistro(FILE* arqBIN, int RRN, int proximo) {
    long OffsetRegAtual = 17 + (RRN * 80);
    fseek(arqBIN, OffsetRegAtual, SEEK_SET);
    char removido = '1'; 
    fwrite(&removido, sizeof(char), 1, arqBIN);
    fwrite(&proximo, sizeof(int), 1, arqBIN);
    return;
}

// Libera a memória das strings dos registros
void liberaStringsRegistro(REGISTRO* reg) {
    if (reg->nomeEstacao) {
        free(reg->nomeEstacao);
        reg->nomeEstacao = NULL;
    }
    if (reg->nomeLinha) {
        free(reg->nomeLinha);
        reg->nomeLinha = NULL;
    }
}