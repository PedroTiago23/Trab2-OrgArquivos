/*
Implementação da funcionalidade 2 de Select 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include "Estruturas.c"

// Trecho principal da funcionalidade de Select From,
// onde todos os registros de um arquivo são impressos

void SELECT(){
    char arqBIN_nome[32];
    scanf("%s",arqBIN_nome);
    
    FILE* arqBIN;
    
    if(!(arqBIN = fopen(arqBIN_nome,"rb")))   // Abre para leitura em binário
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    // Índice de qual registros estamos lendo no momento
    int PosicaoRRN = 0;    
    REGISTRO reg;

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);
    
    // Caso não existam registros no arquivo.
    if(cabecalho.proxRRN == 0)
    {
        printf("Registro inexistente.");
        return;
    }

    // Flag para definir se todos os registros não estão removidos
    bool existe_um = false;

    // Loop de leitura para o arquivo inteiro
    while(PosicaoRRN < cabecalho.proxRRN){
        LerRegistroBin(arqBIN, &reg, PosicaoRRN);
        if(reg.removido == '0')
        {
            imprimirRegistro(&reg);
            existe_um = true;
        }
        PosicaoRRN++;
    }
    if(!existe_um)
        printf("Registro inexistente.");

    fclose(arqBIN); 
}