/*
Funcionalidade 10 - Remoção de uma chave usando arquivo de índice.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/


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

    // agrupa todas as chaves com referências para depois distribuir.
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


// Redistribuição direita -> esquerda -> Concatenação esquerda -> direita.
// Retorna 1 se o pai também ficou com 0 chaves após a concatenação
int tratarUnderflow(int filhoIndice, int paiRRN, NO_ARVOREB* pai, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho) {
    int filhoRRN = pai->P[filhoIndice];
    NO_ARVOREB filhoPagina;
    lerRegistroIndice(&filhoPagina, filhoRRN, arqIndice);

    // tenta pegar da direita
    if (filhoIndice < pai->nroChaves) {
        int irmaoDirRRN = pai->P[filhoIndice + 1];
        NO_ARVOREB irmaoDirPagina;
        lerRegistroIndice(&irmaoDirPagina, irmaoDirRRN, arqIndice);

        if (irmaoDirPagina.nroChaves > 1) { // Tem para emprestar
            redistribuirUniformemente(pai, filhoIndice, &filhoPagina, &irmaoDirPagina);
            escreverNoArvoreB(arqIndice, &filhoPagina, filhoRRN);
            escreverNoArvoreB(arqIndice, &irmaoDirPagina, irmaoDirRRN);
            escreverNoArvoreB(arqIndice, pai, paiRRN);
            return 0; 
        }
    }

    // Tenta pegar da esquerda
    if (filhoIndice > 0) {
        int irmaoEsqRRN = pai->P[filhoIndice - 1];
        NO_ARVOREB irmaoEsqPagina;
        lerRegistroIndice(&irmaoEsqPagina, irmaoEsqRRN, arqIndice);

        if (irmaoEsqPagina.nroChaves > 1) { // Tem para emprestar
            redistribuirUniformemente(pai, filhoIndice - 1, &irmaoEsqPagina, &filhoPagina);
            escreverNoArvoreB(arqIndice, &irmaoEsqPagina, irmaoEsqRRN);
            escreverNoArvoreB(arqIndice, &filhoPagina, filhoRRN);
            escreverNoArvoreB(arqIndice, pai, paiRRN);
            return 0; 
        }
    }

    //  Concatenação, caso ninguém consiga emprestar
    if (filhoIndice < pai->nroChaves) {
        // Junta com o da direita
        int irmaoDirRRN = pai->P[filhoIndice + 1];
        NO_ARVOREB irmaoDirPagina;
        lerRegistroIndice(&irmaoDirPagina, irmaoDirRRN, arqIndice);

        concatenacaoArvoreB(pai, filhoIndice, &filhoPagina, &irmaoDirPagina, irmaoDirRRN, arqIndice, cabecalho);
        escreverNoArvoreB(arqIndice, &filhoPagina, filhoRRN);
        escreverNoArvoreB(arqIndice, pai, paiRRN);
    } 
    else {
        // Junta com o da esquerda
        int irmaoEsqRRN = pai->P[filhoIndice - 1];
        NO_ARVOREB irmaoEsqPagina;
        lerRegistroIndice(&irmaoEsqPagina, irmaoEsqRRN, arqIndice);

        concatenacaoArvoreB(pai, filhoIndice - 1, &irmaoEsqPagina, &filhoPagina, filhoRRN, arqIndice, cabecalho);
        escreverNoArvoreB(arqIndice, &irmaoEsqPagina, irmaoEsqRRN);
        escreverNoArvoreB(arqIndice, pai, paiRRN);
    }

    // Se o pai ficou com 0 chaves por causa da concatenação, o underflow vai para cima
    return (pai->nroChaves < 1) ? 1 : 0;
}

// Recursão para descermos na árvore em busca da chave que precisa ser removida, seja em nó folha ou intermediário.
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
            
            if (teveUnderflow) {
                return tratarUnderflow(i + 1, noRRN, &paginaAtual, arqIndice, cabecalhoIndice);
            }
            return 0;
        }
    }

    // Se a chave não está aqui, desce pelo ponteiro certo.
    int filhoRRN = paginaAtual.P[i];
    int teveUnderflow = removerRecursivo(chave, filhoRRN, arqIndice, cabecalhoIndice);

    if (teveUnderflow) {
        return tratarUnderflow(i, noRRN, &paginaAtual, arqIndice, cabecalhoIndice);
    }
    return 0;
}

// Função para iniciar todo o processo de remoção de uma chave da árvore-B.
void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice) {
    if (cabecalhoIndice->noRaiz == -1) {
      return;
    }
    

    int status = removerRecursivo(chave, cabecalhoIndice->noRaiz, arqIndice, cabecalhoIndice);

    // Se a raiz ficou vazia após a remoção e ela não é uma folha, a árvore diminui de altura
    if (status == 1) {
        NO_ARVOREB raizAtual;
        int raizAntigaRRN = cabecalhoIndice->noRaiz; // Guarda o RRN da raiz que será removida
        lerRegistroIndice(&raizAtual, raizAntigaRRN, arqIndice);
        
        if (raizAtual.nroChaves == 0 && raizAtual.tipoNo != -1) {
            // O novo RRN da raiz agora é o único filho que sobrou
            cabecalhoIndice->noRaiz = raizAtual.P[0]; 
            
            // Coloca a antiga raiz na pilha do topo, no disco
            raizAtual.removido = '1';
            raizAtual.proximo = cabecalhoIndice->topo;
            cabecalhoIndice->topo = raizAntigaRRN;
            escreverNoArvoreB(arqIndice, &raizAtual, raizAntigaRRN);
            
        } else if (raizAtual.nroChaves == 0 && raizAtual.tipoNo == -1) {
            // A árvore está toda vazia
            cabecalhoIndice->noRaiz = -1;
            
            // reutiliza a última folha que sobrou e esvazia
            raizAtual.removido = '1';
            raizAtual.proximo = cabecalhoIndice->topo;
            cabecalhoIndice->topo = raizAntigaRRN;
            escreverNoArvoreB(arqIndice, &raizAtual, raizAntigaRRN);
        }
    }
}

void concatenacaoArvoreB(NO_ARVOREB* pai, int posPai, NO_ARVOREB* esq, NO_ARVOREB* dir, int dirRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho) {
    
    // A chave do pai vai para a esquerda
    esq->C[esq->nroChaves] = pai->C[posPai];
    esq->PR[esq->nroChaves] = pai->PR[posPai];
    esq->nroChaves++;

    // Tudo do nó direito passa para o esquerdo
    int i;
    for (i = 0; i < dir->nroChaves; i++) {
        esq->C[esq->nroChaves] = dir->C[i];
        esq->PR[esq->nroChaves] = dir->PR[i];
        esq->P[esq->nroChaves] = dir->P[i];
        esq->nroChaves++;
    }
    esq->P[esq->nroChaves] = dir->P[i]; // Último ponteiro

    //  remoção do nó direito (Adiciona na pilha de removidos do Índice)
    dir->removido = '1';
    dir->proximo = cabecalho->topo;
    cabecalho->topo = dirRRN;
    escreverNoArvoreB(arqIndice, dir, dirRRN); // Salva a remoção no disco

    // O pai perde a chave que desceu por meio do shift 
    reorganizarPagina(pai, posPai);
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
    lerCabecalhoIndice(arqIndice, &cabecalhoIndice); 

    cabecalho.status = '0';
    atualizarCabecalho(&cabecalho, arqDados);

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
    atualizarCabecalho(&cabecalho, arqDados);
    
    cabecalhoIndice.status = '1';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);
    
    BinarioNaTela(arqDadosNome);
    BinarioNaTela(arqIndiceNome);
}
