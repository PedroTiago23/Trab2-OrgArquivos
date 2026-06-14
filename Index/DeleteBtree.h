#include "BTree.h"

#ifndef DELETE_TREE
#define DELETE_TREE

void DELETE_INDEX();

void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice);

int removerRecursivo(int chave);

void reorganizarPagina(NO_ARVOREB* pagina);

#endif