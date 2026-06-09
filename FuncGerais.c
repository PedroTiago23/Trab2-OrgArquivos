/*
Arquivo com funções usadas em vários outros processos
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#ifndef FUNC_AUX
#define FUNC_AUX

#include "Estruturas.c"
#include <stdio.h>
#include <stdbool.h>

void imprimeCampoInt(int campo)
{
    if(campo != -1)
        printf("%d ", campo);
    else 
        printf("%s ", CAMPO_NULO);
    return;
}

// Imprime um registro na ordem certa a partir de uma struct 
// de registro lido na memória principal
void imprimirRegistro(REGISTRO* reg)
{
    printf("%d %s ", reg->codEstacao, reg->nomeEstacao);
    
    imprimeCampoInt(reg->codLinha);

    if(reg->tamNomeLinha)
        printf("%s ", reg->nomeLinha);
    else printf("%s ", CAMPO_NULO);

    imprimeCampoInt(reg->codProxEstacao);
    imprimeCampoInt(reg->distProxEstacao);
    imprimeCampoInt(reg->codLinhaIntegra);

    // Não usando a função de impressão para não imprimir um espaço a mais no final
    if(reg->codEstIntegra != -1)
        printf("%d", reg->codEstIntegra);
    else printf("%s", CAMPO_NULO);
    printf("\r\n");

    return;
}

// Lê o cabeçalho do arquivo em binário e guarda os dados coletados
// em uma struct com os respectivos campos de um registro de cabeçalho
void lerCabecalhoBin(FILE* arqBIN, CABECALHO* cabecalho)
{
    fseek(arqBIN, 0, SEEK_SET);

    fread(&cabecalho->status, sizeof(char), 1, arqBIN);
    fread(&cabecalho->topo, sizeof(int), 1, arqBIN);
    fread(&cabecalho->proxRRN, sizeof(int), 1, arqBIN);
    fread(&cabecalho->nroEstacoes, sizeof(int), 1, arqBIN);
    fread(&cabecalho->nroParesEstacoes, sizeof(int), 1, arqBIN);
    
    return;
}

// Função para sobrescrever o registro de cabeçalho do arquivo binário com novos dados
void atualizarCabecalho(FILE* arqBIN, CABECALHO* regCabecalho)
{
    fseek(arqBIN, 0, SEEK_SET);
    fwrite(&regCabecalho->status, sizeof(char), 1, arqBIN);
    fwrite(&regCabecalho->topo, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->proxRRN, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->nroEstacoes, sizeof(int), 1, arqBIN);
    fwrite(&regCabecalho->nroParesEstacoes, sizeof(int), 1, arqBIN);
    return;
}

// Lê um registro completo de dado RRN no arquivo em binário e armazena em uma struct registro especificada 
void LerRegistroBin(FILE* arqBIN, REGISTRO* reg, int PosicaoRRN){
    fseek(arqBIN, 17 + (PosicaoRRN * 80),SEEK_SET); // Coloca o ponteiro em qual RRN estiver em PosicaoRRN
        
    fread(&reg->removido,sizeof(char) ,1 ,arqBIN);
    fread(&reg->proximo, sizeof(int), 1, arqBIN);
    fread(&reg->codEstacao, sizeof(int), 1, arqBIN);
    fread(&reg->codLinha, sizeof(int), 1, arqBIN);
    fread(&reg->codProxEstacao, sizeof(int), 1, arqBIN);
    fread(&reg->distProxEstacao, sizeof(int), 1, arqBIN);   
    fread(&reg->codLinhaIntegra, sizeof(int), 1, arqBIN);
    fread(&reg->codEstIntegra, sizeof(int), 1, arqBIN);
    
    // Para os campos variáveis
    // No caso de nome estação, não nos preocupamos em checar por nulo já que nunca serão nulos
    fread(&reg->tamNomeEstacao, sizeof(int),1, arqBIN);
    reg->nomeEstacao = (char*) malloc(reg->tamNomeEstacao + 1);
    fread(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
    reg->nomeEstacao[reg->tamNomeEstacao] = '\0';

    // Nome da linha
    fread(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    // Verificação de se é um campo nulo
    if(reg->tamNomeLinha){
        reg->nomeLinha = (char*) malloc(reg->tamNomeLinha + 1);
        fread(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
        reg->nomeLinha[reg->tamNomeLinha] = '\0';
    }
    else
    {
        reg->nomeLinha = malloc(LEN_CAMPO_NULO);
        strcpy(reg->nomeLinha, CAMPO_NULO);
    }
    
    return;
}

// Função para escrever um registro no arquivo binário a partir de uma estrutura registro
void EscreverRegistroBin(FILE *arqBIN, REGISTRO *reg)
{
    // Contador usado para determinar quantos bytes de lixo escrever no final do registro
    int BytesEscritos = 0;

    // Sequência de writes na ordem certa
    fwrite(&reg->removido, sizeof(char), 1, arqBIN);
    fwrite(&reg->proximo, sizeof(int), 1, arqBIN);
    BytesEscritos += 5; 

    fwrite(&reg->codEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->codLinha, sizeof(int), 1, arqBIN);
    fwrite(&reg->codProxEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->distProxEstacao, sizeof(int), 1, arqBIN);
    fwrite(&reg->codLinhaIntegra, sizeof(int), 1, arqBIN);
    fwrite(&reg->codEstIntegra, sizeof(int), 1, arqBIN);
    BytesEscritos += 24; 

    fwrite(&reg->tamNomeEstacao, sizeof(int), 1, arqBIN);
    fwrite(reg->nomeEstacao, sizeof(char), reg->tamNomeEstacao, arqBIN);
    fwrite(&reg->tamNomeLinha, sizeof(int), 1, arqBIN);
    
    if (reg->tamNomeLinha > 0 && reg->nomeLinha != NULL) {
        fwrite(reg->nomeLinha, sizeof(char), reg->tamNomeLinha, arqBIN);
    }
    
    BytesEscritos += 8 + reg->tamNomeEstacao + reg->tamNomeLinha;

    for(int i = 0; i < TAM_REGISTRO - BytesEscritos; i++)
        fputc('$', arqBIN);
    
    return;
}

// Função usada para ler o nome de um campo na funcionalidade Select Where e definir
// qual campo que a próxima Str será armazenada a partir de um tipo enum CRITERIO
// com os possíveis nomes de cada campo.
// Usado dentro da função buscaRegistro()
CRITERIO campoParaEnum(char* nomeCampo)
{
    if(!strcmp(nomeCampo, "codEstacao")) return codEstacao;
    else if(!strcmp(nomeCampo, "nomeEstacao")) return nomeEstacao;
    else if(!strcmp(nomeCampo, "codLinha")) return codLinha;
    else if(!strcmp(nomeCampo, "nomeLinha")) return nomeLinha;
    else if(!strcmp(nomeCampo, "codProxEstacao")) return codProxEstacao;
    else if(!strcmp(nomeCampo, "distProxEstacao")) return distProxEstacao;
    else if(!strcmp(nomeCampo, "codLinhaIntegra")) return codLinhaIntegra;
    else return codEstIntegra;
}

// Mini-função para lermos o inteiro após o nome do campo e armazená-lo em valorCampo
// Trata valores NULO, também usada para ler valores na funcionalidade Insert
void leIntCampoBusca(int* valorCampo, char* strInput)
{
    // Agora verifica explicitamente a string "NULO"
    if(strcmp(strInput, "NULO") == 0 || strInput[0] == '\0')    
        *valorCampo = -1;
    else
        *valorCampo = atoi(strInput);
    return;
}

// Mini-função com mesmo conceito da anterior mas para strings
// Trata valores NULO, também usada para ler valores na funcionalidade Insert
void leStringCampoBusca(int* tamCampo, char** stringCampo, char* strInput)
{
    if(strInput[0] == '\0') // É NULO
    {
        *tamCampo = 0;
        *stringCampo = malloc(5); // Tamanho da palavra "NULO" + \0
        strcpy(*stringCampo, "NULO");
    }
    else
    {
        *tamCampo = strlen(strInput);
        *stringCampo = malloc(*tamCampo + 1);
        strcpy(*stringCampo, strInput);
    }
    return;
}

// Compara dois registros e retorna true caso sejam iguais, e false caso contrário
// Se estiver sendo usado em SelectWhere, coloque o regBusca no primeiro parâmetro
bool ComparaRegistros(REGISTRO* filtro, REGISTRO* comparado) {
    
    // Macro dentro do escopo da função para comparar strings
    // Se o filtro não for nulo (-1) e for diferente do comparado, retorna falso.
    #define VERIFICA_INT(campo) \
        if (filtro->campo != -2 && filtro->campo != comparado->campo) return false

    // Macro dentro do escopo da função para comparar strings
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
void initRegBusca(REGISTRO* regBusca, int qtdCampos)
{
    // Inicializa com valores não encontrados no arquivo normalmente para
    // sinalizar na comparação que estes campos não deverão ser comparados
    regBusca->codEstacao = -2;
    regBusca->codLinha = -2;
    regBusca->codProxEstacao = -2;
    regBusca->distProxEstacao = -2;
    regBusca->codLinhaIntegra = -2;
    regBusca->codEstIntegra = -2;
    regBusca->nomeEstacao = NULL;
    regBusca->nomeLinha = NULL;

    // Loop para ler os campos do terminal e guardar no registro de busca
    for(int i = 0; i < qtdCampos; i++)  
        {
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

            // Define qual foi o nome do campo lido para armazenar nele
            switch(nomeCampo)
            {
                case nomeEstacao:
                    leStringCampoBusca(&regBusca->tamNomeEstacao, &regBusca->nomeEstacao, stringBuscada);
                    break;
                case nomeLinha:
                    leStringCampoBusca(&regBusca->tamNomeLinha, &regBusca->nomeLinha, stringBuscada);
                    break;
                case codEstacao:
                    leIntCampoBusca(&regBusca->codEstacao, stringBuscada);
                    break;
                case codLinha:
                    leIntCampoBusca(&regBusca->codLinha, stringBuscada);
                    break;
                case codProxEstacao:
                    leIntCampoBusca(&regBusca->codProxEstacao, stringBuscada);
                    break;
                case distProxEstacao:
                    leIntCampoBusca(&regBusca->distProxEstacao, stringBuscada);
                    break;
                case codEstIntegra:
                    leIntCampoBusca(&regBusca->codEstIntegra, stringBuscada);
                    break;
                case codLinhaIntegra:
                    leIntCampoBusca(&regBusca->codLinhaIntegra, stringBuscada);
                    break;
                default:
                    printf("Falha de processamento do arquivo\n");
                    return;
            }
        }  
    
    return;
}

// Função que pula para um registro de RRN especificado e 
// marca seus campos de removido e proximo devidamente
void removerRegistro(FILE* arqBIN, int RRN, int proximo)
{
    long OffsetRegAtual = 17 + (RRN * 80);
    fseek(arqBIN, OffsetRegAtual, SEEK_SET);
    char removido = '1'; 
    fwrite(&removido, sizeof(char), 1, arqBIN);
    fwrite(&proximo, sizeof(int), 1, arqBIN);
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

#endif