#include "BTree.h"

// Sobrescreve todos os dados do registro de cabeçalho do arquivo de índice.
void atualizaCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice)
{
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&cabecalho->status, sizeof(char), 1, arqIndice);
    fwrite(&cabecalho->noRaiz, sizeof(int), 1, arqIndice);
    fwrite(&cabecalho->topo, sizeof(int), 1, arqIndice);
    fwrite(&cabecalho->proxRRN, sizeof(int), 1, arqIndice);
    fwrite(&cabecalho->nroNos, sizeof(int), 1, arqIndice);
    return;
}

// Recupera os dados do cabeçalho de um arquivo de índice e armazena em
// uma struct de cabeçalho de índice.
void lerCabecalhoIndice(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho)
{
    fseek(arqIndice, 0, SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, arqIndice);
    fread(&cabecalho->noRaiz, sizeof(int), 1, arqIndice);
    fread(&cabecalho->topo, sizeof(int), 1, arqIndice);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arqIndice);
    fread(&cabecalho->nroNos, sizeof(int), 1, arqIndice);
    return;
}

void lerRegistroIndice(NO_ARVOREB* registroNo, int RRN, FILE* arqIndice)
{
    int paginaOffset = 17 + (RRN * 53);
    fseek(arqIndice, paginaOffset, SEEK_SET);

    fread(&registroNo->removido, sizeof(char), 1, arqIndice);
    
    #define leCampoNo(item) \
        fread(&registroNo->item, sizeof(int), 1, arqIndice)

    leCampoNo(proximo);
    leCampoNo(tipoNo);
    leCampoNo(nroChaves);
    leCampoNo(C[0]);
    leCampoNo(PR[0]);
    leCampoNo(C[1]);
    leCampoNo(PR[1]);
    leCampoNo(C[2]);
    leCampoNo(PR[2]);
    leCampoNo(P[0]);
    leCampoNo(P[1]);
    leCampoNo(P[2]);
    leCampoNo(P[3]);

    #undef leCampoNo

    return;
}

// Função principal de busca, que retorna o offset do registro
// com a chave encontrada no arquivo de dados.
int buscaChave(int chave, int noRRN, FILE* arqIndice)
{
    // Entrou em uma página inexistente.
    if (noRRN == -1) 
        return -1;

    // Lê a página atual.
    NO_ARVOREB registroNo;
    lerRegistroIndice(&registroNo, noRRN, arqIndice);

    int i = 0;
    while (i < ORDEM_ARVORE-1 && registroNo.C[i] != -1 && chave > registroNo.C[i]) 
        i++;

    if (i < ORDEM_ARVORE-1 && registroNo.C[i] == chave)
        return registroNo.PR[i];

    // Caso procuramos na página, não encontramos e não tem onde prosseguir.
    if(registroNo.P[i] == -1)
        return -1;

    // Se tivermos páginas com chaves maiores que a procurada para continuarmos.
    noRRN = registroNo.P[i]; 
    return buscaChave(chave, noRRN, arqIndice);
}