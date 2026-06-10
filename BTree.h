#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include "Estruturas.h"

void CREATE_INDEX();

void criarIndiceVazio(FILE* arqIndice);

NO_ARVOREB criarNoVazio(int tipoNo);

void escreverNoArvoreB(FILE* arqIndice, NO_ARVOREB* no, int RRN);

#endif