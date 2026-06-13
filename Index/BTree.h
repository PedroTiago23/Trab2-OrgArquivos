#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdbool.h>
#include "../Estruturas.h"
#include "../ArquivoIO.h"
#include "../LogicaBusca.h"
#include "../fornecidas.h"

void atualizaCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice);

void lerCabecalhoIndice(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

void lerRegistroIndice(NO_ARVOREB* registroNo, int RRN, FILE* arqIndice);

int buscaChave(int chave, int noRRN, FILE* arqIndice);

#endif