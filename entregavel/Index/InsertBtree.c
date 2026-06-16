/*
Funções fundamentais para a inserção correta em Árvore-B.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "InsertBtree.h"

int inserirRecursivo(int rrnAtual, int chave, int PonteiroRef, int* chavePromovida, int* PonteiroRefPromovido, int* rrnFilhoDireito, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho) {
    
    if (rrnAtual == -1) {
        *chavePromovida = chave;              
        *PonteiroRefPromovido = PonteiroRef; 
        *rrnFilhoDireito = -1;
        return 1; 
    }

    
    NO_ARVOREB paginaAtual;
    lerRegistroIndice(&paginaAtual, rrnAtual, arqIndice); 

    int pos = 0;
    while (pos < paginaAtual.nroChaves && chave > paginaAtual.C[pos]) {
        pos++;
    }

    // Para Ignorar chaves que são iguais
    if (pos < paginaAtual.nroChaves && chave == paginaAtual.C[pos]) {
        return 0; 
    }


    int teveSplit = inserirRecursivo(paginaAtual.P[pos], chave, PonteiroRef, chavePromovida, PonteiroRefPromovido, rrnFilhoDireito, arqIndice, cabecalho);
    
    if (teveSplit == 0) {
        return 0; // Retorna 0, apos o split ter dado certo(retornado 0), não precisa subir a árvore
    }

    if (paginaAtual.nroChaves < 3) {
        // Encontra a posição da chave promovida na página atual
        int PosInserir = 0;
        while (PosInserir < paginaAtual.nroChaves && *chavePromovida > paginaAtual.C[PosInserir]) {
            PosInserir++;
        }

        // Shift de trás pra frente
        for (int i = paginaAtual.nroChaves; i > PosInserir; i--) {
            paginaAtual.C[i] = paginaAtual.C[i-1];
            paginaAtual.PR[i] = paginaAtual.PR[i-1];
            paginaAtual.P[i+1] = paginaAtual.P[i];
        }

        // Colcoando as coisas no lugar
        paginaAtual.C[PosInserir] = *chavePromovida;
        paginaAtual.PR[PosInserir] = *PonteiroRefPromovido;
        paginaAtual.P[PosInserir+1] = *rrnFilhoDireito;
        paginaAtual.nroChaves++;

        escreverNoArvoreB(arqIndice, &paginaAtual, rrnAtual);
        return 0; 
    }

    // Split 
    int ChaveTemp[4], PonteiroRefTemp[4], P_temp[5];
    
    int PosInserir = 0;
    while (PosInserir < paginaAtual.nroChaves && *chavePromovida > paginaAtual.C[PosInserir]) {
        PosInserir++;
    }

    // Copia as chaves e Ponteiros de Referência pra memória temporária 
    int i_C = 0;
    for (int i = 0; i < 4; i++) {
        if (i == PosInserir) {
            ChaveTemp[i] = *chavePromovida;
            PonteiroRefTemp[i] = *PonteiroRefPromovido;
        } else {
            ChaveTemp[i] = paginaAtual.C[i_C];
            PonteiroRefTemp[i] = paginaAtual.PR[i_C];
            i_C++;
        }
    }

    // Copia pra memória temporária 
    // O novo filho entra em PosInserir + 1
    int i_P = 0;
    for (int i = 0; i < 5; i++) {
        if (i == PosInserir + 1) {
            P_temp[i] = *rrnFilhoDireito;
        } else {
            P_temp[i] = paginaAtual.P[i_P];
            i_P++;
        }
    }

    // Atualiza a página atual (Nó Esquerdo)
    NO_ARVOREB esq = criarNoVazio(paginaAtual.tipoNo);
    esq.C[0] = ChaveTemp[0]; 
    esq.PR[0] = PonteiroRefTemp[0];
    esq.C[1] = ChaveTemp[1]; 
    esq.PR[1] = PonteiroRefTemp[1];
    esq.P[0] = P_temp[0]; esq.P[1] = P_temp[1]; 
    esq.P[2] = P_temp[2];
    esq.nroChaves = 2;

  
    *chavePromovida = ChaveTemp[2];
    *PonteiroRefPromovido = PonteiroRefTemp[2];

    // Cria a Nova Página (Nó Direito)
    NO_ARVOREB dir = criarNoVazio(paginaAtual.tipoNo);
    dir.C[0] = ChaveTemp[3]; 
    dir.PR[0] = PonteiroRefTemp[3];
    dir.P[0] = P_temp[3]; 
    dir.P[1] = P_temp[4];
    dir.nroChaves = 1;
    *rrnFilhoDireito = cabecalho->proxRRN; // Pega o RRN vazio disponível

    // Escreve as alterações
    escreverNoArvoreB(arqIndice, &esq, rrnAtual);
    escreverNoArvoreB(arqIndice, &dir, *rrnFilhoDireito);
    
    cabecalho->proxRRN++;
    cabecalho->nroNos++;

    return 1; // o nó pai foi cortado com sucesso
}

// Primeira função a ser chamada para inserir uma dada chave na árvore-B.
// Gerencia as chaves, referências e chamadas da função recursiva.
void inserirNaArvoreB(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho, int chave, int PonteiroRef) {
    
    // Caso a árvore esteja vazia
    if (cabecalho->nroNos == 0) {
        NO_ARVOREB novaRaiz = criarNoVazio(-1); // -1 = Nó folha
        novaRaiz.C[0] = chave;
        novaRaiz.PR[0] = PonteiroRef;
        novaRaiz.nroChaves = 1;
        
        int rrnNovaRaiz = cabecalho->proxRRN;
        escreverNoArvoreB(arqIndice, &novaRaiz, rrnNovaRaiz);
        
        cabecalho->noRaiz = rrnNovaRaiz;
        cabecalho->proxRRN++;
        cabecalho->nroNos++;
        return;
    }

    //  No caso da Árvore existir, é só descer ela 
    int chavePromovida = -1;
    int prPromovido = -1;
    int rrnFilhoDireito = -1;

    int teveSplit = inserirRecursivo(cabecalho->noRaiz, chave, PonteiroRef, &chavePromovida, &prPromovido, &rrnFilhoDireito, arqIndice, cabecalho);

    // Verifica o Split 
    if (teveSplit) {
        NO_ARVOREB novaRaiz = criarNoVazio(0); // 0 = Nó raiz com filhos
        
        novaRaiz.C[0] = chavePromovida;
        novaRaiz.PR[0] = prPromovido;
        novaRaiz.P[0] = cabecalho->noRaiz;
        novaRaiz.P[1] = rrnFilhoDireito;
        novaRaiz.nroChaves = 1;
        
        int rrnNovaRaiz = cabecalho->proxRRN;
        escreverNoArvoreB(arqIndice, &novaRaiz, rrnNovaRaiz);
        
        cabecalho->noRaiz = rrnNovaRaiz;
        cabecalho->proxRRN++;
        cabecalho->nroNos++;
    }
}