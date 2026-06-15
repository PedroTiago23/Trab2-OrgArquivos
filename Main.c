/*
Trabalho 1 de SCC0215 - Organização de Arquivos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include "fornecidas.h"

// Arquivos contendo todo conteúdo auxiliar para as funcionalidades
#include "Estruturas.h" 
#include "ArquivoIO.h"
#include "LogicaBusca.h"

// Arquivos de cada uma das 6 funcionalidades
#include "Dados/CreateTable.h"
#include "Dados/SelectFrom.h"
#include "Dados/SelectWhere.h"
#include "Dados/Delete.h"
#include "Dados/Insert.h"
#include "Dados/Update.h"

// Arquivos da funcionalidades da árvore B
#include "Index/CreateIndex.h"
#include "Index/SelectWhereBT.h"
#include "Index/InsertBtree.h"


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
        case 7:
            CREATE_INDEX();
            break;
        case 8:
            SELECT_TREE();
            break;
        default:
            printf("Entrada inválida!");
            break;
    }
}