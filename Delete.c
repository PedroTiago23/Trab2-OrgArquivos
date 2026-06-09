/*
Implementação da funcionalidade 4 de Delete
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Trecho principal da funcionalidade 4 de Delete, onde registros que 
// tenham campos de valor especifiacdo são removidos com a abordagem
// dinâmica de reaproveitamento de espaço

void DELETE(){
    
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    FILE* arqBIN = fopen(arqBIN_nome, "rb+");
    
    if (arqBIN == NULL) {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    int qtdDeletes;  
    scanf("%d", &qtdDeletes);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);

    // Marca o arquivo como incosistento durante o processo
    cabecalho.status = '0';
    fseek(arqBIN, 0, SEEK_SET);
    fwrite(&cabecalho.status, sizeof(char), 1, arqBIN);
    fseek(arqBIN, 0, SEEK_SET);

    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada
    REGISTRO regBusca;
    
    int contador = 0;
    while (contador < qtdDeletes)
    {
        int RRN = 0;

        int qtdCampos = 0;
        scanf("%d", &qtdCampos);

        // Settando o regBusca devidamente, colocando todos os 
        // valores inválidos e então lendo os campos do terminal
        initRegBusca(&regBusca, qtdCampos);

        while(RRN < cabecalho.proxRRN)
        {
            BuscaRegistro(arqBIN, &cabecalho, &regBusca, &regLido, &RRN);   
            
            // Verificação necessária pois é possível que BuscaRegistro já tenha passado de proxRRN
            if(RRN < cabecalho.proxRRN)
            {
                removerRegistro(arqBIN, RRN-1, cabecalho.topo);
                cabecalho.topo = RRN-1;
            }
            
            if(regLido.nomeEstacao) free(regLido.nomeEstacao);
            if(regLido.nomeLinha) free(regLido.nomeLinha);
        }

        if(regBusca.nomeEstacao) free(regBusca.nomeEstacao);
        if(regBusca.nomeLinha) free(regBusca.nomeLinha);

        contador++;
    }

    cabecalho.status = '1';
    recalcularContadores(arqBIN, &cabecalho);
    atualizarCabecalho(arqBIN, &cabecalho);

    fclose(arqBIN);
    
    BinarioNaTela(arqBIN_nome);
}