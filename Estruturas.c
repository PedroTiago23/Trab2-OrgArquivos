/*
Arquivo com estruturas de dados fundamentais para o projeto
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef ESTRUTURAS
#define ESTRUTURAS


#define TAM_CABECALHO 17
#define TAM_REGISTRO 80
#define TAM_MAX_LINHA_CSV 128 
#define CAMPO_NULO "NULO"
#define LEN_CAMPO_NULO 5

// Struct para o registro de cabeçalho do arquivo binário
typedef struct
{
    char status;
    int topo;
    int proxRRN;
    int nroEstacoes;
    int nroParesEstacoes;
}CABECALHO;

// Struct para cada registro de dados do arquivo binário,
// também usado na transferência de registros do .csv pro .bin
typedef struct
{
    char removido;
    int proximo;
    int codEstacao; 
    int codLinha;
    int codProxEstacao;
    int distProxEstacao;
    int codLinhaIntegra;
    int codEstIntegra;
    int tamNomeEstacao;
    char* nomeEstacao;  
    int tamNomeLinha;
    char* nomeLinha;
} REGISTRO;

// Tipo enum para ajudar a definir qual campo foi lido como critério de busca
// em SELECT_WHERE(), para então armazenar o valor de tal campo corretamente
typedef enum
{
    codEstacao,         // 0
    nomeEstacao,        // 1
    codLinha,           // 2
    nomeLinha,          // 3
    codProxEstacao,     // 4
    distProxEstacao,    // 5
    codLinhaIntegra,    // 6
    codEstIntegra       // 7
}CRITERIO;

// Struct usado na contagem de pares diferentes presentes no arquivo
typedef struct
{
    int origem;
    int destino;
}ParEstacoes;


#endif