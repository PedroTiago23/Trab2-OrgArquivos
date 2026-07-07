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

    // Como sabemos pela especificação, ambos arquivos serão o mesmo, logo mesmo cabecalho, dispensa outra leitura.
    CABECALHO cabecalho;
    lerCabecalhoBin(arq1, &cabecalho);

    if(cabecalho.status == '0')
    {
        printf("Arquivos inconsistente.");
        return;
    }

    REGISTRO registroArq1, registroArq2;
    bool existe_um = false;
    for(int i = 0; i < cabecalho.proxRRN; i++)
    {
        LerRegistroBin(arq1, &registroArq1, i);
        // Ignoramos registros que estiverem removidos ou que tiverem codProxEst nulo.
        if(registroArq1.removido == '1' || registroArq1.codProxEstacao == -1)
            continue;

        for(int j = 0; j < cabecalho.proxRRN; j++)
        {
            LerRegistroBin(arq2, &registroArq2, j);
            if(registroArq2.removido == '1')    // Dispensa verificação já que codEstacao nunca será nulo.
                continue;

            if(registroArq1.codProxEstacao == registroArq2.codEstacao)
            {
                printf("%d %s %s %d %s\n", registroArq1.codEstacao, registroArq1.nomeEstacao, registroArq1.nomeLinha, registroArq2.codEstacao, registroArq2.nomeEstacao);
                existe_um = true;
            }
            liberaStringsRegistro(&registroArq2);
        }
        liberaStringsRegistro(&registroArq1);
    }

    if(!existe_um)
        printf("Registro inexistente.\n");

    fclose(arq1);
    fclose(arq2);

    return;
}