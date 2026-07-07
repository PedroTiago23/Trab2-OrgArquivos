/*
Implementação da funcionalidade de Junção de Loop Aninhado.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "JoinHeaders.h"

void NESTED_JOIN()
{
    char nomeArq1[32], nomeCampo1[32], nomeArq2[32], nomeCampo2[32];
    scanf("%s %s %s %s", nomeArq1, nomeCampo1, nomeArq2, nomeCampo2);

    if(strcmp(nomeCampo1, "codProxEst") || strcmp(nomeCampo2, "codEstacao"))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE* arq1 = fopen(nomeArq1, "rb");
    FILE* arq2 = fopen(nomeArq2, "rb");

    if (arq1 == NULL || arq2 == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }


    return;
}