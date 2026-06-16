/*
Declarações de Create Table
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/


#ifndef CREATETABLE
#define CREATETABLE 

#include <stdio.h>
#include "Estruturas.h"

void lerIntCSV(int* campoRegistro, char** separationPtr);

void LerRegistroCSV(char *linha, REGISTRO *reg);

void CREATE_TABLE();



#endif