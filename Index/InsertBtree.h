/*
Declaração das funções de inserção.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef INSERT_BTREE
#define INSERT_BTREE

#include <stdio.h>
#include "BTree.h"
#include "Estruturas.h"

int inserirRecursivo(int rrnAtual, int chave, int PonteiroRef, int* chavePromovida, int* PonteiroRefPromovido, int* rrnFilhoDireito, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

void inserirNaArvoreB(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho, int chave, int PonteiroRef);

#endif 