#include "DeleteBtree.h"


// Usada para avaliar se uma página está abaixo do mínimo de chaves 
int contarChaves(NO_ARVOREB* pagina) 
{
    int quantidade = 0;

    // Percorre o vetor de chaves até o limite 
    for (int i = 0; i < ORDEM_ARVORE - 1; i++) 
    {
        // Se a chave for diferente de -1 (vazio ou removido), soma o contador
        if (pagina->C[i] != -1) 
        {
            quantidade++;
        }
    }

    return quantidade;
}

// Remove os espaços vazios entre as chaves.
void reorganizarPagina(NO_ARVOREB* pagina, int posicaoRemovida) {
    for (int j = posicaoRemovida; j < pagina->nroChaves - 1; j++) {
        pagina->C[j] = pagina->C[j+1];
        pagina->PR[j] = pagina->PR[j+1];
        pagina->P[j+1] = pagina->P[j+2]; 
    }
    
    // Tira a última posição que ficou duplicada
    int ultimaPos = pagina->nroChaves - 1;
    pagina->C[ultimaPos] = -1;
    pagina->PR[ultimaPos] = -1;
    if (pagina->tipoNo != -1) {
        pagina->P[ultimaPos + 1] = -1;
    }
    
    pagina->nroChaves--;
}

// Recursão para descermos na árvore em busca da chave a ser removida, seja em nó folha ou intermediário.
// Retorna 1 apenas se a página atual estiver com underflow, senão retorna 0. 
int removerRecursivo(int chave, int noRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice) {
    if (noRRN == -1) return 0; // Chave não encontrada na árvore

    NO_ARVOREB paginaAtual;
    lerRegistroIndice(&paginaAtual, noRRN, arqIndice);

    int i = 0;
    while (i < paginaAtual.nroChaves && chave > paginaAtual.C[i]) {
        i++;
    }
    
    // Achou a chave na página atual
    if (i < paginaAtual.nroChaves && paginaAtual.C[i] == chave) {
        
        if (paginaAtual.tipoNo == -1) {
            // caso seja uma folha
            reorganizarPagina(&paginaAtual, i);
            escreverNoArvoreB(arqIndice, &paginaAtual, noRRN);
            
            // Retorna 1 se a página ficou com 0 chaves (Underflow)
            return (paginaAtual.nroChaves < 1) ? 1 : 0; 
        } 
        else {
            // Caso seja um nó interno (Busca o sucessor)
            NO_ARVOREB paginaSucessor;
            int folhaSucessorRRN = paginaAtual.P[i+1];
            
            lerRegistroIndice(&paginaSucessor, folhaSucessorRRN, arqIndice);
            while (paginaSucessor.P[0] != -1) {
                folhaSucessorRRN = paginaSucessor.P[0];
                lerRegistroIndice(&paginaSucessor, folhaSucessorRRN, arqIndice);
            }
            
            int chaveSucessor = paginaSucessor.C[0];
            int referenciaSucessor = paginaSucessor.PR[0];

            // Substitui a chave atual pela sucessora e escreve no disco
            paginaAtual.C[i] = chaveSucessor;
            paginaAtual.PR[i] = referenciaSucessor;
            escreverNoArvoreB(arqIndice, &paginaAtual, noRRN);
            
            // Desce recursivamente para apagar o sucessor lá na folha onde ele estava
            int teveUnderflow = removerRecursivo(chaveSucessor, paginaAtual.P[i+1], arqIndice, cabecalhoIndice);
            
            
            return 0; 
        }
    }

    // Se a chave não está aqui, desce pelo ponteiro certo.
    int filhoRRN = paginaAtual.P[i];
    int teveUnderflow = removerRecursivo(chave, filhoRRN, arqIndice, cabecalhoIndice);

    
    return 0; 
}

// Função para iniciar todo o processo de remoção de uma chave da árvore-B.
void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice) {
    if (cabecalhoIndice->noRaiz == -1) return;

    int status = removerRecursivo(chave, cabecalhoIndice->noRaiz, arqIndice, cabecalhoIndice);

    // Se a raiz ficou vazia após a remoção e ela não é uma folha, a árvore diminui de altura
    if (status == 1) {
        NO_ARVOREB raizAtual;
        lerRegistroIndice(&raizAtual, cabecalhoIndice->noRaiz, arqIndice);
        
        if (raizAtual.nroChaves == 0 && raizAtual.tipoNo != -1) {
            // O novo RRN da raiz agpra é o único filho que sobrou
            cabecalhoIndice->noRaiz = raizAtual.P[0]; 
        } else if (raizAtual.nroChaves == 0 && raizAtual.tipoNo == -1) {
            // A árvore está completamente vazia
            cabecalhoIndice->noRaiz = -1;
        }
    }
}

void DELETE_INDEX() {
    char arqDadosNome[32]; 
    char arqIndiceNome[32];
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
    lerCabecalhoIndice(&cabecalhoIndice, arqIndice); 

    cabecalho.status = '0';
    atualizaCabecalhoBin(&cabecalho, arqDados);

    cabecalhoIndice.status = '0';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    REGISTRO regLido;
    REGISTRO regBusca;
    
    for (int contador = 0; contador < qtdDeletes; contador++) {
        int qtdCampos = 0;
        scanf("%d", &qtdCampos);
        initRegBusca(&regBusca, qtdCampos);

        if (regBusca.codEstacao != -2) {
            int rrnEncontrado = buscaChave(regBusca.codEstacao, cabecalhoIndice.noRaiz, arqIndice);

            if (rrnEncontrado != -1) {
                LerRegistroBin(arqDados, &regLido, rrnEncontrado);

                if (ComparaRegistros(&regBusca, &regLido) && regLido.removido == '0') {
                    
                    removerRegistro(arqDados, rrnEncontrado, cabecalho.topo);
                    cabecalho.topo = rrnEncontrado;

                    removerChaveArvore(regLido.codEstacao, arqIndice, &cabecalhoIndice);
                }
                if(regLido.nomeEstacao) free(regLido.nomeEstacao);
                if(regLido.nomeLinha) free(regLido.nomeLinha);
            }
        }
        else {
            int RRN = 0;
            while(RRN < cabecalho.proxRRN) {
                BuscaRegistro(arqDados, &cabecalho, &regBusca, &regLido, &RRN);   
                
                if(RRN < cabecalho.proxRRN) {
                    removerRegistro(arqDados, RRN, cabecalho.topo);
                    cabecalho.topo = RRN;
                    removerChaveArvore(regLido.codEstacao, arqIndice, &cabecalhoIndice);
                }
                
                if(regLido.nomeEstacao) free(regLido.nomeEstacao);
                if(regLido.nomeLinha) free(regLido.nomeLinha);
            }
        }

        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);
    }

    cabecalho.status = '1';
    recalcularContadores(arqDados, &cabecalho);
    atualizaCabecalhoBin(&cabecalho, arqDados);
    
    cabecalhoIndice.status = '1';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);
    
    BinarioNaTela(arqIndiceNome);
}