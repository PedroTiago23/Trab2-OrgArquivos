/*
Arquivo com todas as definições usadas para indexação de arquivos e suas funcionalidades.
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


// Funções gerais para manuseio de árvores-B.

void atualizaCabecalhoIndice(CABECALHO_ARVOREB* cabecalho, FILE* arqIndice);

void lerCabecalhoIndice(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

void lerRegistroIndice(NO_ARVOREB* registroNo, int RRN, FILE* arqIndice);

int buscaChave(int chave, int noRRN, FILE* arqIndice);

void escreverNoArvoreB(FILE* arqIndice, NO_ARVOREB* pagina, int noRRN);



void CREATE_INDEX();



void SELECT_INDEX();



// Funções para inserção.

void INSERT_INDEX();

int inserirRecursivo(int rrnAtual, int chave, int PonteiroRef, int* chavePromovida, int* PonteiroRefPromovido, int* rrnFilhoDireito, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);

void inserirNaArvoreB(FILE* arqIndice, CABECALHO_ARVOREB* cabecalho, int chave, int PonteiroRef);



// Funções para remoção.

void DELETE_INDEX();

int contarChaves(NO_ARVOREB* pagina);

void redistribuirUniformemente(NO_ARVOREB* noPai, int indicePai, NO_ARVOREB* filhoEsq, NO_ARVOREB* filhoDir);

void reorganizarPagina(NO_ARVOREB* pagina, int posicaoRemovida);

int tratarUnderflow(int filhoIndice, int paiRRN, NO_ARVOREB* noPai, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice);

int removerRecursivo(int chave, int noRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalhoIndice);

void removerChaveArvore(int chave, FILE* arqIndice, CABECALHO_ARVOREB* CabecalhoIndice);

void concatenacaoArvoreB(NO_ARVOREB* pai, int posPai, NO_ARVOREB* esq, NO_ARVOREB* dir, int dirRRN, FILE* arqIndice, CABECALHO_ARVOREB* cabecalho);


#endif