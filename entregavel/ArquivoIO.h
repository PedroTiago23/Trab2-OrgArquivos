/*
Declarações do Módulo de I/O de Arquivos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef ARQUIVOIO
#define ARQUIVOIO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.h"
#include "LogicaBusca.h"
#include "fornecidas.h"

void imprimeCampoInt(int campo);

void imprimirRegistro(REGISTRO* reg);

void lerCabecalhoBin(FILE* arqBIN, CABECALHO* cabecalho);

void atualizarCabecalho(CABECALHO* regCabecalho, FILE* arqBIN);

void LerRegistroBin(FILE* arqBIN, REGISTRO* reg, int PosicaoRRN);

void EscreverRegistroBin(FILE *arqBIN, REGISTRO *reg);

void lerRegistroTerminal(REGISTRO* novoRegistro);

void removerRegistro(FILE* arqBIN, int RRN, int proximo);

void liberaStringsRegistro(REGISTRO* reg);

#endif