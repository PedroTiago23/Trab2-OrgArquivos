#ifndef DELETE_TREE
#define DELETE_TREE

#include "BTree.h"

void DELETE_INDEX();

void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice);

int removerRecursivo(int chave);

void reorganizarPagina(NO_ARVOREB* pagina);

void concatenacaoArvoreB(NO_ARVOREB* pai, int posPai, NO_ARVOREB* esq, NO_ARVOREB* dir, int dirRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

#endif