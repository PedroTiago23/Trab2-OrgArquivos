#include "intoInsert.h"

void INSERT_TREE() {

    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb+");
    FILE* arqIndice = fopen(arqIndiceNome, "rb+");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    CABECALHO cabecalhoDados;
    lerCabecalhoBin(arqDados, &cabecalhoDados);
    
    CABECALHO_ARVOREB cabecalhoIndice;
    lerCabecalhoIndice(&cabecalhoIndice, arqIndice);

    // Defini como incosistente o arquivo antes de começar a mexer
    cabecalhoDados.status = '0';
    
    atualizaCabecalhoBin(&cabecalhoDados, arqDados);

    cabecalhoIndice.status = '0';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    // loop para inserir 
    int qtdInsercoes;
    scanf("%d", &qtdInsercoes);

    for (int i = 0; i < qtdInsercoes; i++) {
        REGISTRO novoReg;

        // função do trabalho 1 de ler o registro do terminal
        lerRegistroTerminal(&novoReg); 

        int rrnInserido = -1; 

        // escrevendo no disco
        if (cabecalhoDados.topo == -1) {
            // Se não tem registro removido, insere no final
            rrnInserido = cabecalhoDados.proxRRN;
            
            EscreveRegistroBin(arqDados, &novoReg, rrnInserido);
            
            cabecalhoDados.proxRRN++;
        } else {
            
            rrnInserido = cabecalhoDados.topo;
            
            // Calcula o byte  onde o registro removido começa no arquivo de dados
            long offsetRemovido = 17 + ((long)rrnInserido * 80);
            fseek(arqDados, offsetRemovido, SEEK_SET);

            // Ler o  RRN removido que estava salvo lá.
            char statusRemovido;
            fread(&statusRemovido, sizeof(char), 1, arqDados); 
            
            int proximoRRN;
            fread(&proximoRRN, sizeof(int), 1, arqDados);

            //  Atualiza o cabecalhoDados.topo com esse valor.
            // A pilha desce, o próximo removido agora é o novo topo.
            cabecalhoDados.topo = proximoRRN;

            // Sobrescreve o registro no rrnInserido com o novoReg.
            // Volta o ponteiro do arquivo para o início offset removido para escrever por cima.
            fseek(arqDados, offsetRemovido, SEEK_SET);

            EscreveRegistroBin(arqDados, &novoReg, rrnInserido);
        }

        // Atualiza o índice
        inserirNaArvoreB(arqIndice, &cabecalhoIndice, novoReg.codEstacao, rrnInserido);

        // limpa a memória
        liberaStringsRegistro(&novoReg);
    }

    // Fecha tudo e deixa como status consistente 
    cabecalhoDados.status = '1';
    
    atualizaCabecalhoBin(&cabecalhoDados, arqDados);

    cabecalhoIndice.status = '1';
    atualizaCabecalhoIndice(&cabecalhoIndice, arqIndice);

    fclose(arqDados);
    fclose(arqIndice);

    BinarioNaTela(arqIndiceNome);
}