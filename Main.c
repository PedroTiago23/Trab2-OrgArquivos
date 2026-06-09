/*
Trabalho 1 de SCC0215 - Organização de Arquivos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include "fornecidas.c"

// Arquivos contendo todo conteúdo auxiliar para as funcionalidades
#include "Estruturas.c" 
#include "ArquivoIO.c"
#include "LogicaBusca.c"

// Arquivos de cada uma das 6 funcionalidades
#include "CreateTable.c"
#include "SelectFrom.c"
#include "SelectWhere.c"
#include "Delete.c"
#include "Insert.c"
#include "Update.c"

int main(){
    // Lendo entrada de qual será a funcionalidade executada
    int comando;
    scanf("%d", &comando);
    
    // Executando...
    switch (comando){
        case 1:
            CREATE_TABLE();
            break;
        case 2:
            SELECT();
            break;
        case 3:
            SELECT_WHERE();
            break;
        case 4:
            DELETE();
            break;
        case 5:
            INSERT();
            break;
        case 6:
            UPDATE();
            break;
        default:
            printf("Entrada inválida!");
            break;
    }
}