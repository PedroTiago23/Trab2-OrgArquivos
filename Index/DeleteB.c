#include "DeleteBtree.h"

// Remove os espaços vazios entre as chaves.
void reorganizarPagina(NO_ARVOREB* pagina)
{

}

int removerRecursivo(int chave, int noRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice)
{

    NO_ARVOREB paginaAtual;
    lerRegistroIndice(&paginaAtual, noRRN, arqIndice);

    int i = 0;
    while (i < ORDEM_ARVORE-1 && paginaAtual.C[i] != -1 && chave > paginaAtual.C[i]) 
        i++;
    
    if (i < ORDEM_ARVORE-1 && paginaAtual.C[i] == chave)
    {
        // Caso a chave for de um nó folha, podemos remover normal.
        if(paginaAtual.tipoNo == -1)
        {
            paginaAtual.C[i] = -1;
            paginaAtual.PR[i] = -1;
        }
        else    // Caso contrário, devemos trocar a chave com seu sucessor imediato.
        {
            // Busca do sucessor.
            NO_ARVOREB paginaSucessor;
            int sucessorRRN = paginaAtual.P[i+1];
            
            while(sucessorRRN != -1)
            {
                lerRegistroIndice(&paginaSucessor, sucessorRRN, arqIndice);
                if(paginaSucessor.P[0] == -1)   // Alcançamos a página folha com o sucessor (elmeento mais à esquerda).
                break;
                else 
                sucessorRRN = paginaSucessor.P[0];
            }
            // Atualizando chaves e referências.
            paginaAtual.C[i] = paginaSucessor.C[0];
            paginaAtual.PR[i] = paginaSucessor.PR[0];

            paginaSucessor.C[0] = -1;
            paginaSucessor.PR[0] = -1;
        }
        reorganizarPagina(&paginaAtual);

        // A partir daqui, verificar se a taxa de ocupação está consistente.
    }

    // Caso procuramos na página, não encontramos e não tem onde prosseguir.
    if(paginaAtual.P[i] == -1)
        return -1;  // Deletes não retornam se foi encontrado ou não mas veremos o que fazer com isto.

    // Se tivermos páginas com chaves maiores que a procurada para continuarmos.
    noRRN = paginaAtual.P[i]; 
    removerRecursivo(chave, noRRN, arqIndice, &cabecalhoIndice);
}

// Função para iniciar todo o processo de remoção de uma chave da árvore-B.
void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice)
{

    return;
}

void DELETE_INDEX()
{
    // Leitura do arquivo de dados e do arquivo de índice.
    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb+");
    FILE* arqIndice = fopen(arqIndiceNome, "rb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int qtdDeletes;  
    scanf("%d", &qtdDeletes);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqDados, &cabecalho);
    CABECALHO_ARVOREB cabecalhoIndice;
    lerCabecalhoIndice(arqIndice, &cabecalhoIndice);

    // Marca os arquivos como incosistentes durante o processo.
    cabecalho.status = '0';
    fseek(arqDados, 0, SEEK_SET);
    fwrite(&cabecalho.status, sizeof(char), 1, arqDados);
    fseek(arqDados, 0, SEEK_SET);

    cabecalhoIndice.status = '0';
    fseek(arqIndice, 0, SEEK_SET);
    fwrite(&cabecalhoIndice.status, sizeof(char), 1, arqIndice);
    fseek(arqIndice, 0, SEEK_SET);

    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada.
    REGISTRO regBusca;
    
    int contador = 0;
    while (contador < qtdDeletes)
    {
        int RRN = 0;

        int qtdCampos = 0;
        scanf("%d", &qtdCampos);

        // Settando o regBusca devidamente, colocando todos os 
        // valores inválidos e então lendo os campos do terminal
        initRegBusca(&regBusca, qtdCampos);

        // Caso o codEstacao é um dos campos buscados, usamos a busca pelo arquivo de índice.
        if(regBusca.codEstacao != -2)
        {
            // Pulamos diretamente pro registro com o codEstacao buscado através da árvore que retorna seu offset.
            // Se o Offset = 17 + (RRN * 80), então o RRN = (Offset - 17)/80
            int offsetReg = buscaChave(regBusca.codEstacao, cabecalhoIndice.noRaiz, arqIndice);

            if(offsetReg != -1)
            {
                RRN = (offsetReg - 17)/80;
                LerRegistroBin(arqDados, &regLido, RRN);

                if(ComparaRegistros(&regBusca, &regLido) && regLido.removido == '0')
                {
                    removerRegistro(arqDados, RRN-1, cabecalho.topo);
                    cabecalho.topo = RRN-1;

                    // Remoção da chave no arquivo de índice.
                    removerChaveArvore(regLido.codEstacao, arqIndice, &cabecalhoIndice);
                }
                if(regLido.nomeEstacao) free(regLido.nomeEstacao);
                if(regLido.nomeLinha) free(regLido.nomeLinha);
            }
        }

        else while(RRN < cabecalho.proxRRN)
        {
            BuscaRegistro(arqDados, &cabecalho, &regBusca, &regLido, &RRN);   
            
            // Verificação necessária pois é possível que BuscaRegistro já tenha passado de proxRRN
            if(RRN < cabecalho.proxRRN)
            {
                removerRegistro(arqDados, RRN-1, cabecalho.topo);
                cabecalho.topo = RRN-1;
            }
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
        }

        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);

        contador++;
    }

    cabecalho.status = '1';
    recalcularContadores(arqDados, &cabecalho);
    atualizarCabecalho(arqDados, &cabecalho);
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);
    
    BinarioNaTela(arqDadosNome);
    BinarioNaTela(arqIndiceNome);
}