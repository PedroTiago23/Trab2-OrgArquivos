// Funcionalidade 7 - Criação do Índice

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
void lerCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice)
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
// noRRN será usado para determinar a página onde inserir.
int buscaChave(int chave, int* noRRN, FILE* arqIndice)
{
    NO_ARVOREB registroNo;
    lerRegistroIndice(&registroNo, *noRRN, arqIndice);

    // Lógica para checar as 3 chaves e desviar para os descendentes. 
    for(int i = 0; i < 3; i++)
    {
        if(registroNo.C[i] == -1)
            break;

        if(chave == registroNo.C[i])
            return registroNo.PR[i];

        if(chave < registroNo.C[i])
        {
            *noRRN = registroNo.P[i];
            if(noRRN == -1) return -1;
            return buscaChave(chave, noRRN, arqIndice);
        }
    }
}

// Trecho principal para a funcionalidade de criar um arquivo de índice para
// o arquivo de dados do trabalho introdutório. 
void CREATE_INDEX() {
    // Leitura do arquivo de dados e criação/leitura do arquivo de índice.
    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb");
    FILE* arqIndice = fopen(arqIndiceNome, "wb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do Arquivo.\n");
        return;
    }

    CABECALHO cabecalhoDados;
    lerCabecalhoBin(arqDados, &cabecalhoDados);

    // Inicializa o arquivo de índice zerado e com status inconsistente ('0').
    CABECALHO_ARVOREB cabecalhoTree;
    cabecalhoTree.status = '0';
    cabecalhoTree.noRaiz = -1;
    cabecalhoTree.topo = -1;
    cabecalhoTree.proxRRN = 0;
    cabecalhoTree.nroNos = 0;
    
    atualizaCabecalhoIndice(&cabecalhoTree, arqIndice);

    REGISTRO reg;
    int rrnDados = 0;

    // Percorre sequencialmente o arquivo de dados.
    while (rrnDados < cabecalhoDados.proxRRN) {
        LerRegistroBin(arqDados, &reg, rrnDados);

        // Apenas registros ativos são indexados.
        if (reg.removido == '0')
        {
            
        }

        liberaStringsRegistro(&reg); // Importante para não dar memory leak.
        rrnDados++;
    }

    // Atualiza os dados no cabeçalho do arquivo de índice.
    char statusOk = '1';
    atualizaCabecalhoIndice(&cabecalhoTree, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);

    BinarioNaTela(arqIndiceNome);
}