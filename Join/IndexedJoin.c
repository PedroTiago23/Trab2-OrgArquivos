/*
Implementação da funcionalidade de Junção de Loop Único.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "JoinHeaders.h"

void INDEXED_JOIN()
{
    char nomeArq1[32], nomeCampo1[32], nomeArq2[32], nomeCampo2[32], nomeArq2BTree[32];
    scanf("%s %s %s %s %s", nomeArq1, nomeCampo1, nomeArq2, nomeCampo2, nomeArq2BTree);

    if(strcmp(nomeCampo1, "codProxEst") || strcmp(nomeCampo2, "codEstacao"))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE* arq1 = fopen(nomeArq1, "rb");
    FILE* arq2 = fopen(nomeArq2, "rb");
    FILE* arq2arv = fopen(nomeArq2BTree, "rb");

    if (arq1 == NULL || arq2 == NULL || arq2arv == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Como sabemos pela especificação, ambos arquivos serão o mesmo, logo mesmo cabecalho, dispensa outra leitura.
    CABECALHO cabecalho;
    CABECALHO_ARVOREB cabecalhoBTree;
    lerCabecalhoBin(arq1, &cabecalho);
    lerCabecalhoIndice(arq2arv, &cabecalhoBTree);

    if(cabecalho.status == '0' || cabecalhoBTree.status == '0')
    {
        printf("Arquivos inconsistente.");
        return;
    }

    REGISTRO registroArq1, registroArq2;
    for(int i = 0; i < cabecalho.proxRRN; i++)
    {
        LerRegistroBin(arq1, &registroArq1, i);
        if(registroArq1.removido == '1' || registroArq1.codProxEstacao == -1)
            continue;
  
        int proxEstOffset = buscaChave(registroArq1.codProxEstacao, cabecalhoBTree.noRaiz, arq2);
        if(proxEstOffset == -1)
            continue;   // Não existe registro com tal codEstacao na árvore-B.
        int proxEstRRN = (proxEstOffset - 17)/80;
        LerRegistroBin(arq2, &registroArq2, proxEstRRN);

        liberaStringsRegistro(&registroArq1);
    }

    return;
}