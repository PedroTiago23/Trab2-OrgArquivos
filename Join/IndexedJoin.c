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

    if(strcmp(nomeCampo1, "codProxEstacao") || strcmp(nomeCampo2, "codEstacao"))
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
        printf("Falha no processamento do arquivo.");
        return;
    }

    bool existe_um = false; // Flag para reportarmos "Registro inexistente" ou não.
    REGISTRO registroArq1, registroArq2;
    for(int i = 0; i < cabecalho.proxRRN; i++)
    {
        LerRegistroBin(arq1, &registroArq1, i);
        if(registroArq1.removido == '1' || registroArq1.codProxEstacao == -1)
        {
            liberaStringsRegistro(&registroArq1);
            continue;
        }
        int proxEstOffset = buscaChave(registroArq1.codProxEstacao, cabecalhoBTree.noRaiz, arq2arv);
        if(proxEstOffset == -1)
            continue;   // Não existe registro com tal codEstacao na árvore-B do arq2.
        int proxEstRRN = (proxEstOffset - 17)/80;

        LerRegistroBin(arq2, &registroArq2, proxEstRRN);
        if(registroArq2.removido == '1')    // Na teoria buscaChave() pode retornar um reg removido.
        {
            liberaStringsRegistro(&registroArq2);
            continue;
        }
        
        printf("%d %s %s %d %s\n", registroArq1.codEstacao, registroArq1.nomeEstacao, registroArq1.nomeLinha, registroArq2.codEstacao, registroArq2.nomeEstacao);
        existe_um = true;

        liberaStringsRegistro(&registroArq1);
        liberaStringsRegistro(&registroArq2);
    }

    if(!existe_um)
        printf("Registro inexistente.\n");

    fclose(arq1);
    fclose(arq2);
    fclose(arq2arv);

    return;
}