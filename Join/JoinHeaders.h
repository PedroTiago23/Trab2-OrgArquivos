#ifndef JOIN_HEADERS
#define JOIN_HEADERS

#include "../ArquivoIO.h"
#include "../Estruturas.h"
#include "../LogicaBusca.h"
#include "../Index/BTree.h"

void NESTED_JOIN();
void INDEXED_JOIN();
void ORDER_BY();
void MERGE_JOIN();

int compara_codEstacao(const void *a, const void *b);

int compara_codProxEstacao(const void *a, const void *b);

int popularVetorRegistros(REGISTRO** vetor, FILE* arq);

#endif