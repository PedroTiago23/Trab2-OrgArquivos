#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include "Estruturas.h"
#include "ArquivoIO.h"
#include "fornecidas.h"

void CREATE_INDEX();

// void criarIndiceVazio(FILE* arqIndice);

void atualizaCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice);

NO_ARVOREB criarNoVazio(int tipoNo);

void escreverNoArvoreB(FILE* arqIndice, NO_ARVOREB* no, int RRN);

// retorna 1 se conseguiu inserir e 0 se não consegui inserir(a chave já existia)
int inserirArvoreB(FILE* arqArvore, int valor);

int RecursaoInserir(int rrnAtual, int chave, int ponteiroRef, int* chavePromovida, int* PonteiroRefPromovido, int* rrnFilhoDireito, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

int BtreeVazia(FILE* arquArvore, NO_ARVOREB no, CABECALHO_ARVOREB cabecalhoArvore);
#endif