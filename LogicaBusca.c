/*
Lógica de Busca e Filtros
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Estruturas.c"


// Função usada para ler o nome de um campo na funcionalidade Select Where e definir
// qual campo que a próxima Str será armazenada a partir de um tipo enum CRITERIO
// com os possíveis nomes de cada campo.
// Usado dentro da função buscaRegistro()
CRITERIO campoParaEnum(char* nomeCampo) {
    #define RETORNA_SE_IGUAL(str, enumVal) \
        if(!strcmp(nomeCampo, str)) return enumVal

    RETORNA_SE_IGUAL("codEstacao", codEstacao);
    RETORNA_SE_IGUAL("nomeEstacao", nomeEstacao);
    RETORNA_SE_IGUAL("codLinha", codLinha);
    RETORNA_SE_IGUAL("nomeLinha", nomeLinha);
    RETORNA_SE_IGUAL("codProxEstacao", codProxEstacao);
    RETORNA_SE_IGUAL("distProxEstacao", distProxEstacao);
    RETORNA_SE_IGUAL("codLinhaIntegra", codLinhaIntegra);
    
    #undef RETORNA_SE_IGUAL
    
    return codEstIntegra; 
}


// Mini-função para lermos o inteiro após o nome do campo e armazená-lo em valorCampo
// Trata valores NULO, também usada para ler valores na funcionalidade Insert
void leIntCampoBusca(int* valorCampo, char* strInput) {
    // Agora verifica explicitamente a string "NULO"
    if(strcmp(strInput, "NULO") == 0 || strInput[0] == '\0')    
        *valorCampo = -1;
    else
        *valorCampo = atoi(strInput);
    return;
}

// Mini-função com mesmo conceito da anterior mas para strings
// Trata valores NULO, também usada para ler valores na funcionalidade Insert
void leStringCampoBusca(int* tamCampo, char** stringCampo, char* strInput) {

    if(strInput[0] == '\0') { // É NULO
        *tamCampo = 0;
        *stringCampo = malloc(5); // Tamanho da palavra "NULO" + \0
        strcpy(*stringCampo, "NULO");
    } else {
        *tamCampo = strlen(strInput);
        *stringCampo = malloc(*tamCampo + 1);
        strcpy(*stringCampo, strInput);
    }
    return;
}


// Compara dois registros e retorna true caso sejam iguais, e false caso contrário
// Se estiver sendo usado em SelectWhere, coloque o regBusca no primeiro parâmetro
bool ComparaRegistros(REGISTRO* filtro, REGISTRO* comparado) {

    // Se o filtro não for nulo (-1) e for diferente do comparado, retorna falso.
    #define VERIFICA_INT(campo) \
        if (filtro->campo != -2 && filtro->campo != comparado->campo) return false

    // Se o filtro não for nulo e for diferente do comparado, retorna falso.
    #define VERIFICA_STR(campo) \
        if (filtro->campo != NULL && strcmp(filtro->campo, comparado->campo) != 0) return false

    VERIFICA_INT(codEstacao);
    VERIFICA_INT(codLinha);
    VERIFICA_INT(codProxEstacao);
    VERIFICA_INT(distProxEstacao);
    VERIFICA_INT(codLinhaIntegra);
    VERIFICA_INT(codEstIntegra);

    VERIFICA_STR(nomeEstacao);
    VERIFICA_STR(nomeLinha);

    return true;

    #undef VERIFICA_INT
    #undef VERIFICA_STR
}


// Função para inicializar um registro de busca usado em BuscaRegistro()
// e então ler os valores no terminal, armazenando-os devidamente
void initRegBusca(REGISTRO* regBusca, int qtdCampos) {
    // Inicialização padrão
    regBusca->codEstacao = -2;
    regBusca->codLinha = -2;
    regBusca->codProxEstacao = -2;
    regBusca->distProxEstacao = -2;
    regBusca->codLinhaIntegra = -2;
    regBusca->codEstIntegra = -2;
    regBusca->nomeEstacao = NULL;
    regBusca->nomeLinha = NULL;

    for(int i = 0; i < qtdCampos; i++) {
        char idCampo[32]; 
        scanf("%s", idCampo);
        
        CRITERIO nomeCampo = campoParaEnum(idCampo);
        char stringBuscada[50] = "";  
        
        // Feito pois estava dando um erro ao ler inteiros com ScanQuoteString()
        if (nomeCampo == nomeEstacao || nomeCampo == nomeLinha) {
            ScanQuoteString(stringBuscada);
        } else {
            scanf("%s", stringBuscada);
        }

        #define CASO_INT(campoEnum) \
            case campoEnum: \
                leIntCampoBusca(&regBusca->campoEnum, stringBuscada); \
                break

        #define CASO_STR(campoEnum, tamCampo, stringCampo) \
            case campoEnum: \
                leStringCampoBusca(&regBusca->tamCampo, &regBusca->stringCampo, stringBuscada); \
                break

        switch(nomeCampo) {
            CASO_STR(nomeEstacao, tamNomeEstacao, nomeEstacao);
            CASO_STR(nomeLinha, tamNomeLinha, nomeLinha);
            CASO_INT(codEstacao);
            CASO_INT(codLinha);
            CASO_INT(codProxEstacao);
            CASO_INT(distProxEstacao);
            CASO_INT(codEstIntegra);
            CASO_INT(codLinhaIntegra);

            default:
                printf("Falha de processamento do arquivo\n");
                return;
        }

        #undef CASO_INT
        #undef CASO_STR
    }  
    return;
}

// Função usada após todas as alterações no arquivo binário para recalcular quantas 
// estações e pares de estações únicos existem no arquivo, já que podem ter sido alteradas.
void recalcularContadores(FILE* arqBIN, CABECALHO* cabecalho) 
{
    
    // Aloca as listas dinamicamente
    char** listaNomesUnicos = malloc(cabecalho->proxRRN * sizeof(char*));
    ParEstacoes* listaParesUnicos = malloc(cabecalho->proxRRN * sizeof(ParEstacoes));
    
    int qtdEstacoesUnicas = 0;
    int qtdParesUnicos = 0;

    for (int i = 0; i < cabecalho->proxRRN; i++) 
    {
        REGISTRO reg;
        LerRegistroBin(arqBIN, &reg, i);
        
        // Apenas conta registros que não estão marcados como removidos
        if (reg.removido == '0') 
        {
            
            // Para cada registro reg lido, verifica a lista de nomes de estações para ver se já existe
            bool estacaoJaExiste = false;
            for (int j = 0; j < qtdEstacoesUnicas; j++) 
            {
                if (strcmp(listaNomesUnicos[j], reg.nomeEstacao) == 0) 
                {
                    estacaoJaExiste = true;
                    break; // Achou, não precisa continuar procurando
                }
            }
            
            // Se varreu toda a lista e não achou, insere no final
            if (!estacaoJaExiste) {
                listaNomesUnicos[qtdEstacoesUnicas] = (char*)malloc((strlen(reg.nomeEstacao)+1) * sizeof(char));
                strcpy(listaNomesUnicos[qtdEstacoesUnicas], reg.nomeEstacao);
                qtdEstacoesUnicas++;
            }

            // Mesma lógica mas para os pares de estações
            if (reg.codProxEstacao != -1) {
                bool parJaExiste = false;
                for (int j = 0; j < qtdParesUnicos; j++) {
                    if (listaParesUnicos[j].origem == reg.codEstacao && listaParesUnicos[j].destino == reg.codProxEstacao) {
                        parJaExiste = true;
                        break; // Achou, para a busca
                    }
                }
                
                // Adicionamos na lista de pares únicos
                if (!parJaExiste) {
                    listaParesUnicos[qtdParesUnicos].origem = reg.codEstacao;
                    listaParesUnicos[qtdParesUnicos].destino = reg.codProxEstacao;
                    qtdParesUnicos++;
                }
            }
        }
        
        if (reg.nomeEstacao) free(reg.nomeEstacao);
        if (reg.nomeLinha) free(reg.nomeLinha);
    }

    // Atualiza o cabeçalho original com os contadores absolutos
    cabecalho->nroEstacoes = qtdEstacoesUnicas;
    cabecalho->nroParesEstacoes = qtdParesUnicos;
    
    // Libera as listas auxiliares
    for(int i = 0; i < qtdEstacoesUnicas; i++)
        free(listaNomesUnicos[i]);
    free(listaNomesUnicos);
    free(listaParesUnicos);
}

// Percorre arqBIN até encontrar o primeiro regLido que seja igual à regBusca
void BuscaRegistro(FILE* arqBIN, CABECALHO* cabecalho, REGISTRO* regBusca, REGISTRO* regLido, int* RRN) 
{
    while(*RRN < cabecalho->proxRRN)
    {
        LerRegistroBin(arqBIN, regLido, *RRN);
        (*RRN)++;

        // Se encontramos o registro procurado
        if(ComparaRegistros(regBusca, regLido) && regLido->removido == '0') 
            return;
        

        if(regLido->nomeEstacao)
        {
            free(regLido->nomeEstacao);
            regLido->nomeEstacao = NULL;
        }
        if(regLido->nomeLinha) 
        { 
            free(regLido->nomeLinha);
            regLido->nomeLinha = NULL;
        }
    }
    return;
}