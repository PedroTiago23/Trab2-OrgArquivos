/*
Declarações de funções usadas na remoção de chaves em uma árvore-B.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef DELETE_TREE
#define DELETE_TREE

#include "BTree.h"

void DELETE_INDEX();

int contarChaves(NO_ARVOREB* pagina);

void redistribuirUniformemente(NO_ARVOREB* noPai, int indicePai, NO_ARVOREB* filhoEsq, NO_ARVOREB* filhoDir);

void reorganizarPagina(NO_ARVOREB* pagina, int posicaoRemovida);

int tratarUnderflow(int filhoIndice, int paiRRN, NO_ARVOREB* noPai, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice);

int removerRecursivo(int chave, int noRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice);

void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice);

void concatenacaoArvoreB(NO_ARVOREB* pai, int posPai, NO_ARVOREB* esq, NO_ARVOREB* dir, int dirRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

#endif