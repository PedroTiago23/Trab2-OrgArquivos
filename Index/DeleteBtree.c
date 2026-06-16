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
            quantidade++;
    }

    return quantidade;
}

// Recebe 3 páginas, e tenta distribuir as chaves o mais uniformemente possível entre elas,
// deixando o lado direito com um a menos quando necessário.
void redistribuirUniformemente(NO_ARVOREB* noPai, int indicePai, NO_ARVOREB* filhoEsq, NO_ARVOREB* filhoDir)
{
    int qtdEsq = contarChaves(filhoEsq);
    int qtdDir = contarChaves(filhoDir);
    int qtdTotal = qtdEsq + qtdDir + 1;

    // Agruparemos todas as chaves com referências aqui e então distribuíremos.
    int Cs[ORDEM_ARVORE * 2];
    int PRs[ORDEM_ARVORE * 2];
    int Ps[ORDEM_ARVORE * 2 + 1];

    int indiceChaves = 0;
    int indiceP = 0;

    // Pegando chaves do filho esquerdo.
    for(int e = 0; e < qtdEsq; e++) 
    {
        Cs[indiceChaves] = filhoEsq->C[e];
        PRs[indiceChaves] = filhoEsq->PR[e];
        Ps[indiceP] = filhoEsq->P[e];
        indiceChaves++;
        indiceP++;
    }
    Ps[indiceP] = filhoEsq->P[qtdEsq];    // O último ponteiro da esquerda.
    indiceP++;

    // Pegando chave do pai
    Cs[indiceChaves] = noPai->C[indicePai];
    PRs[indiceChaves] = noPai->PR[indicePai];
    indiceChaves++;

    // Pegando chaves do filho direito.
    for(int d = 0; d < qtdDir; d++) 
    {
        Cs[indiceChaves] = filhoDir->C[d];
        PRs[indiceChaves] = filhoDir->PR[d];
        Ps[indiceP] = filhoDir->P[d];
        indiceChaves++;
        indiceP++;
    }
    Ps[indiceP] = filhoDir->P[qtdDir]; // O último ponteiro da direita
    indiceP++;

    // Sobrescrevendo os filhos com -1 para evitar lixo.
    for(int i = 0; i < ORDEM_ARVORE-1; i++) 
    {
        filhoEsq->C[i] = -1; filhoEsq->PR[i] = -1; filhoEsq->P[i] = -1;
        filhoDir->C[i] = -1; filhoDir->PR[i] = -1; filhoDir->P[i] = -1;
    }
    filhoEsq->P[ORDEM_ARVORE-1] = -1;
    filhoDir->P[ORDEM_ARVORE-1] = -1;

    // Ao mesmo tempo que definimos as duas metades, garantimos 
    // que o filho esquerdo terá mais que o direito quando necessário.
    int novoQtdEsq = qtdTotal / 2;
    
    indiceChaves = 0;
    indiceP = 0;

    // Preenche o novo filho esquerdo.
    for(int i = 0; i < novoQtdEsq; i++) 
    {
        filhoEsq->C[i] = Cs[indiceChaves];
        filhoEsq->PR[i] = PRs[indiceChaves];
        filhoEsq->P[i] = Ps[indiceP];
        indiceChaves++;
        indiceP++;
    }
    filhoEsq->P[novoQtdEsq] = Ps[indiceP];
    indiceP++;

    // Preenche o novo pai;
    noPai->C[indicePai] = Cs[indiceChaves];
    noPai->PR[indicePai] = PRs[indiceChaves];
    indiceChaves++;

    // Preenche o novo filho direito.
    int novoQtdDir = qtdTotal - novoQtdEsq - 1;
    for(int i = 0; i < novoQtdDir; i++) 
    {
        filhoDir->C[i] = Cs[indiceChaves];
        filhoDir->PR[i] = PRs[indiceChaves];
        filhoDir->P[i] = Ps[indiceP];
        indiceChaves++;
        indiceP++;
    }
    filhoDir->P[novoQtdDir] = Ps[indiceP];
    indiceP++;
}

// Remove os espaços vazios entre as chaves.
void reorganizarPagina(NO_ARVOREB* pagina, int posicaoRemovida) {
    for (int j = posicaoRemovida; j < pagina->nroChaves-1; j++) {
        pagina->C[j] = pagina->C[j+1];
        pagina->PR[j] = pagina->PR[j+1];
        pagina->P[j+1] = pagina->P[j+2]; 
    }
    
    // Tira a última posição que ficou duplicada
    int ultimaPos = pagina->nroChaves-1;
    pagina->C[ultimaPos] = -1;
    pagina->PR[ultimaPos] = -1;
    if (pagina->tipoNo != -1)
        pagina->P[ultimaPos + 1] = -1;
    
    pagina->nroChaves--;
}

// Dado uma página abaixo do mínimo de chaves, tenta redistribuição ou concatenação para resolver.
// Resolver propagação de overflow. Retorna 1 caso o nó pai esteja com underflow após a concatenação.
// Redistribuição direita -> esquerda -> Concatenação esquerda -> direita.
int tratarUnderflow(int filhoIndice, int paiRRN, NO_ARVOREB* noPai, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice)
{
    int filhoUnderflowRRN = noPai->P[filhoIndice];
    NO_ARVOREB filhoPagina;
    lerRegistroIndice(&filhoPagina, filhoUnderflowRRN, arqIndice);

    // Tentando redistribuir com o irmão direito primeiro.
    if(noPai->P[filhoIndice+1] != -1 && filhoIndice < ORDEM_ARVORE-1)
    {
        int irmaoDirRRN = noPai->P[filhoIndice+1];
        NO_ARVOREB irmaoDirPagina;
        lerRegistroIndice(&irmaoDirPagina, irmaoDirRRN, arqIndice);

        if(contarChaves(&irmaoDirPagina) > MINIMO_CHAVES)
        {
            redistribuirUniformemente(noPai, filhoIndice, &filhoPagina, &irmaoDirPagina);
            // Ainda não decidi em escrever aqui ou na redistribuirUniformemente().
            return 0;
        }
    }

    // Tentando então o esquerdo.
    if(filhoIndice > 0)
    {
        int irmaoEsquerdoRRN = noPai->P[filhoIndice-1];
        NO_ARVOREB irmaoEsqPagina;
        lerRegistroIndice(&irmaoEsqPagina, irmaoEsquerdoRRN, arqIndice);

        if(contarChaves(&irmaoEsqPagina) > MINIMO_CHAVES)
        {
            redistribuirUniformemente(noPai, filhoIndice-1, &irmaoEsqPagina, &filhoPagina);
            // Idem
            return 0;
        }
    }

    // Se nenhuma redistribuição aconteceu, temos que concatenar o filho com um de seus irmãos.
    // Primeiro tentar concatenar com irmão à esquerda, e então à direita.


}

// Recursão para descermos na árvore em busca da chave a ser removida, seja em nó folha ou intermediário.
// Retorna 1 apenas se a página atual estiver com underflow, senão retorna 0. 
int removerRecursivo(int chave, int noRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice)
{
    if (noRRN == -1) return 0; // Chave não encontrada na árvore

    NO_ARVOREB paginaAtual;
    lerRegistroIndice(&paginaAtual, noRRN, arqIndice);

    int i = 0;
    while (i < paginaAtual.nroChaves && chave > paginaAtual.C[i])
        i++;
    
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