/*
Declarações das funções relacionada a lógica e busca de registros.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef LOGICABUSCA
#define LOGICABUSCA


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Estruturas.h"



CRITERIO campoParaEnum(char* nomeCampo);

void leIntCampoBusca(int* valorCampo, char* strInput);

void leStringCampoBusca(int* tamCampo, char** stringCampo, char* strInput);

bool ComparaRegistros(REGISTRO* filtro, REGISTRO* comparado);

void initRegBusca(REGISTRO* regBusca, int qtdCampos);

void recalcularContadores(FILE* arqBIN, CABECALHO* cabecalho);

void BuscaRegistro(FILE* arqBIN, CABECALHO* cabecalho, REGISTRO* regBusca, REGISTRO* regLido, int* RRN);

#endif