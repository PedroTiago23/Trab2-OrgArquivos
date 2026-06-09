/*
Implementação da funcionalidade 3 de Select Where 
Bruno Dias de Campos Filho - 16832658
Pedro Tiago Biffi - 16827777
*/

// Trecho principal da funcionalidade Select Where, onde apenas registros
// com campos de valores específicados são impressos
void SELECT_WHERE(){
    
    char arqBIN_nome[32];
    scanf("%s", arqBIN_nome); 
    
    FILE* arqBIN = fopen(arqBIN_nome, "rb");
    
    if (arqBIN == NULL) {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    int qtdBuscas;  
    scanf("%d", &qtdBuscas);

    CABECALHO cabecalho;
    lerCabecalhoBin(arqBIN, &cabecalho);

    // Registro para cada registro lido do arquivo
    REGISTRO regLido;
    // Registro usado para comparar cada registro lido com os campos da entrada
    REGISTRO regBusca;
    
    int contador = 0;
    while (contador < qtdBuscas)
    {
        int RRN = 0;
        bool existe_um = false;

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

    fclose(arqBIN);
    
}