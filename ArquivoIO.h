#ifndef ARQUIVOIO
#define ARQUIVOIO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estruturas.h"

void imprimeCampoInt(int campo);

void imprimirRegistro(REGISTRO* reg);

void lerCabecalhoBin(FILE* arqBIN, CABECALHO* cabecalho);

void atualizarCabecalho(FILE* arqBIN, CABECALHO* regCabecalho);

void LerRegistroBin(FILE* arqBIN, REGISTRO* reg, int PosicaoRRN);

void EscreverRegistroBin(FILE *arqBIN, REGISTRO *reg);

void removerRegistro(FILE* arqBIN, int RRN, int proximo);

void liberaStringsRegistro(REGISTRO* reg);

#endif