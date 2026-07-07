/*
Implementação da funcionalidade de Junção Ordenação-Intercalação.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "JoinHeaders.h"


void MERGE_JOIN() {
    char nomeArq1[32];
    char nomeCampo1[32];
    char nomeArq2[32];
    char nomeCampo2[32];

    // Lê as entradas do usuário no terminal
    scanf("%s %s %s %s", nomeArq1, nomeCampo1, nomeArq2, nomeCampo2);

    // Verifica os campos que foram digitados
    if (strncmp(nomeCampo1, "codProxEst", 10) != 0 || strcmp(nomeCampo2, "codEstacao") != 0) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // define e abre os arquivos no modo leitura binária 
    FILE* arq1 = fopen(nomeArq1, "rb");
    FILE* arq2 = fopen(nomeArq2, "rb");

    // Verifica se os arquivos tiveram problemas para serem abertos 
    if (arq1 == NULL || arq2 == NULL) {
        printf("Falha no processamento do arquivo.\n");
        
        if (arq1 != NULL) {
            fclose(arq1);
        }
        
        if (arq2 != NULL) {
            fclose(arq2);
        }
        
        return;
    }

    // Define e le os cabeçalhos dos dois arquivos 
    CABECALHO cab1;
    CABECALHO cab2;
    lerCabecalhoBin(arq1, &cab1);
    lerCabecalhoBin(arq2, &cab2);

    // Verifica se os arquivos não estão marcados como inconsistentes antes de mexer
    if (cab1.status == '0' || cab2.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq1);
        fclose(arq2);
        return;
    }

    // define o vetor1 para o arquivo1 e carrega na memória o vetor 
    // de registros lidos que não são logicamente removidos ou nulos
    REGISTRO *vetor1 = malloc(cab1.proxRRN * sizeof(REGISTRO));
    // quantidade de registros validos que foram lidos
    int qtd1 = 0;
    
    for (int i = 0; i < cab1.proxRRN; i++) {
        REGISTRO reg;
        LerRegistroBin(arq1, &reg, i);
        
        //  só será guardado registros não removidos e com o campo válido
        if (reg.removido == '0' && reg.codProxEstacao != -1) {
            vetor1[qtd1] = reg;
            qtd1++;
        } else {
            // se o registro for nulo ou removido, apenas libera a memoria dele 
            // e não entra no vetor
            liberaStringsRegistro(&reg);
        }
    }
    fclose(arq1);

    // Mesma lógica do vetor1, mas agora para o arquivo2
    REGISTRO *vetor2 = malloc(cab2.proxRRN * sizeof(REGISTRO));
    int qtd2 = 0;
    
    for (int i = 0; i < cab2.proxRRN; i++) {
        REGISTRO reg;
        LerRegistroBin(arq2, &reg, i);
        
        if (reg.removido == '0' && reg.codEstacao != -1) { 
            vetor2[qtd2] = reg;
            qtd2++;
        } else {
            liberaStringsRegistro(&reg);
        }
    }
    fclose(arq2);

    // Ordena ambos os vetores, o arquivo1 é ordenado pelo codProxEstacao
    // o arquivo2 é ordenado pelo codEstacao
    qsort(vetor1, qtd1, sizeof(REGISTRO), compara_codProxEstacao);
    qsort(vetor2, qtd2, sizeof(REGISTRO), compara_codEstacao);

    // Definindo e inicializando as variaveis que representam os ponteiros 
    // dos arquivos 1 e 2, além da flag existe_um, que verifica se 
    // o registro foi encontrado ou não existe
    int p1 = 0;
    int p2 = 0;
    bool existe_um = false;

    // Loop que busca nos dois vetores uma única vez. 
    // Se algum dos dois vetores acabar, acaba o loop
    while (p1 < qtd1 && p2 < qtd2) {
        
        if (vetor1[p1].codProxEstacao == vetor2[p2].codEstacao) {
            // Encontrou o par que corresponde 
            // imprime o resultado
            printf("%d %s %s %d %s\n", vetor1[p1].codEstacao, vetor1[p1].nomeEstacao, vetor1[p1].nomeLinha, vetor2[p2].codEstacao, vetor2[p2].nomeEstacao);
            existe_um = true;
            
            // Avança o ponteiro do vetor 1
            // Já que o codEstacao do vetor 2 é único, o vetor 1 não vai achar mais ninguém igual a ele
            p1++; 
            

          // Como ambos os arquivos estã ordenados em ordem crescente
          // o arquivo1 em ordem crescente de codProxEstacao e o arquivo2
          // em ordem crescente de codEstacao, pode acontecer de um ponteiro 
          // de um arquivo estar na frente do outro arquivo

          // Caso o ponteiro do arquivo1 está atrás do arquivo2
        } else if (vetor1[p1].codProxEstacao < vetor2[p2].codEstacao) {
            // Avança o ponteiro do arquivo1 
            p1++;
           
          // Caso o ponteiro do arquivo2 esteja atrás do arquivo1 
        } else {
            // Avança o ponteiro do arquivo2
            p2++;
        }
    }

    // Caso não seja encontrado o par, a flag permanecerá como falsa.
    if (existe_um == false) {
        printf("Registro inexistente.\n");
    }

    
    // Libera a memória de todos os campos necessários
    for (int i = 0; i < qtd1; i++) {
        liberaStringsRegistro(&vetor1[i]);
    }
    free(vetor1);
    
    for (int i = 0; i < qtd2; i++) {
        liberaStringsRegistro(&vetor2[i]);
    }
    free(vetor2);

    return;
}