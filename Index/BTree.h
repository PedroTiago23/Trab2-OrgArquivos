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

#endif