/*
Declarações para as funções de BTree
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include "../Estruturas.h"
#include "../ArquivoIO.h"
#include "../LogicaBusca.h"
#include "../fornecidas.h"

NO_ARVOREB criarNoVazio(int tipoNo);

void atualizaCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice);

void lerCabecalhoIndice(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

void lerRegistroIndice(NO_ARVOREB* registroNo, int RRN, FILE* arqIndice);

int buscaChave(int chave, int noRRN, FILE* arqIndice);

void escreverNoArvoreB(FILE* arqIndice, NO_ARVOREB* pagina, int noRRN);

#endif