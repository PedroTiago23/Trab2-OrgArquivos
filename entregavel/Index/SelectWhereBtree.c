/*
Funcionalidade 8 - Recuperação de registros por campos usando o arquivo de índice.
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

#include "SelectWhereBtree.h"

void SELECT_INDEX()
{
    // Leitura do arquivo de dados e do arquivo de índice.
    char arqDadosNome[32], arqIndiceNome[32];
    scanf("%s %s", arqDadosNome, arqIndiceNome);

    FILE* arqDados = fopen(arqDadosNome, "rb");
    FILE* arqIndice = fopen(arqIndiceNome, "rb");

    if (arqDados == NULL || arqIndice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int qtdBuscas;
    scanf("%d", &qtdBuscas);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqDados, &cabecalho);

    CABECALHO_ARVOREB cabecalhoIndice;
    lerCabecalhoIndice(arqIndice, &cabecalhoIndice );

    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada
    REGISTRO regBusca;

    int contador = 0;
    // Loop para cada uma das buscas
    while (contador < qtdBuscas)
    {
        int RRN = 0;
        bool existe_um = false;

        int qtdCampos = 0;
        scanf("%d", &qtdCampos);

        // Settando o regBusca devidamente, colocando todos os 
        // valores inválidos e então lendo os campos do terminal
        initRegBusca(&regBusca, qtdCampos);

        // Caso o codEstacao é um dos campos buscados, usamos a busca pelo arquivo de índice.
        if(regBusca.codEstacao != -2)
        {
            // Pulamos diretamente pro registro com o codEstacao buscado através da árvore que retorna seu offset.
            // Se o Offset = 17 + (RRN * 80), então o RRN = (Offset - 17)/80
            int offset = buscaChave(regBusca.codEstacao, cabecalhoIndice.noRaiz, arqIndice);
            if(offset != -1)
            {
                int rrnEncontrado = (offset - 17)/80;
                LerRegistroBin(arqDados, &regLido, rrnEncontrado);

                if(ComparaRegistros(&regBusca, &regLido) && regLido.removido == '0')
                {
                    imprimirRegistro(&regLido);
                    existe_um = true;
                }

                // Libera PRIMEIRO, anula DEPOIS
                if(regLido.nomeEstacao) {
                    free(regLido.nomeEstacao);
                    regLido.nomeEstacao = NULL;
                }
                if(regLido.nomeLinha) {
                    free(regLido.nomeLinha);
                    regLido.nomeLinha = NULL;
                }   
            }   

        else while(RRN < cabecalho.proxRRN)
        {
            BuscaRegistro(arqDados, &cabecalho, &regBusca, &regLido, &RRN);   
            
            // Verificação necessária pois é possível que BuscaRegistro já tenha passado de proxRRN
            if(RRN < cabecalho.proxRRN)
            {
                imprimirRegistro(&regLido);
                existe_um = true;
            }
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
        }

        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);

        if(!existe_um)
            printf("Registro inexistente.\n");
        printf("\n");
        
        contador++;
    }

    fclose(arqDados);
    fclose(arqIndice);
}   
}