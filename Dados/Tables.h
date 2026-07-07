/*
Conjunto das definições relacionadas ao manuseio de tabelas de dados e suas funcionalidades.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/


#ifndef DATA_METHODS
#define DATA_METHODS

#include <stdio.h>
#include <stdbool.h>
#include "../Estruturas.h"
#include "../ArquivoIO.h"
#include "../LogicaBusca.h"
#include "../fornecidas.h"


// Funções para a criação da tabela binária a partir de um arquivo .csv

void CREATE_TABLE();

void lerIntCSV(int* campoRegistro, char** separationPtr);

void LerRegistroCSV(char *linha, REGISTRO *reg);


void SELECT();

void SELECT_WHERE();

void INSERT();

void DELETE();

void UPDATE();

#endif