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

// Arquivos de cada uma das 6 funcionalidades do Trabalho introdutório
#include "Dados/CreateTable.h"
#include "Dados/SelectFrom.h"
#include "Dados/SelectWhere.h"
#include "Dados/Delete.h"
#include "Dados/Insert.h"
#include "Dados/Update.h"

// Arquivos de cada uma das 4 funcionalidades do Trabalho 1
#include "Index/CreateIndex.h"
#include "Index/SelectWhereBtree.h"
#include "Index/IntoInsert.h"
#include "Index/DeleteBtree.h"

// Arquivo de definição das funções e funcionalidades do Trabalho 2
#include "Join/JoinHeaders.h"

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
            SELECT_INDEX();
            break;
        case 9:
            INSERT_INDEX();
            break;
        case 10:
            DELETE_INDEX();
            break;
        case 11:
            NESTED_JOIN();
            break;
        case 12:
            INDEXED_JOIN();
            break;
        case 13:
            ORDER_BY();
            break;
        case 14:
            MERGE_JOIN();
            break;
        default:
            printf("Entrada inválida!\n");
            break;
    }
}

/*
11 arquivoEntrada1.bin nomeCampo1 arquivoEntrada2.bin nomeCampo2
12 arquivoEntrada1.bin nomeCampo1 arquivoEntrada2.bin nomeCampo2 indiceArquivoEntrada2.bin
13 arquivoEntrada.bin campoOrdenacao arquivoOrdenado.bin
14 arquivoEntrada1.bin nomeCampo1 arquivoEntrada2.bin nomeCampo2
*/