#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define  QTD_INDIVIDUOS  10
#define  GERACOES  30

typedef struct item{
    int peso, ganho;
} item;

typedef struct individuo{
    int peso_total, ganho_total;
} individuo;

void imprimir_elementos(item *vetor, int qtd_max);
void criar_geracao_inicial (int qtd_itens, int **geracao_de_individuos);
void imprimir_matriz(int qtd_itens, int **geracao_de_individuos, individuo *dados_individuos);
void preencher_dados (int qtd_itens, int **geracao_de_individuos, individuo *dados_individuos, item *elementos);


int main (){

    int capacidade_maxima_mochila, i, aux1, aux2, qtd_itens;
    FILE *mochila;
    individuo dados_individuos[QTD_INDIVIDUOS];

    // Ler o arquivo mochila.txt e armazena num vetor de struct de itens
    mochila = fopen ("mochila.txt", "r");
    if(mochila == NULL){
        printf("Erro na abertura do arquivo\n\n");
        return 1;
    }
    fscanf(mochila, "%d %d", &capacidade_maxima_mochila, &qtd_itens);
    printf("Capacidade maxima da mochila: %d \t quantidade de itens: %d\n", capacidade_maxima_mochila, qtd_itens);
    
    item elementos[qtd_itens];// cria um vetor de elementos
    i=0;
   while(!feof(mochila)){
        fscanf(mochila, "%d %d", &aux1, &aux2);
        elementos[i].ganho=aux1;
        elementos[i].peso = aux2;
        i++;
    }
    //-------------Fim Leitura ----------------------

    // ------------- Cria dinamicamente uma matriz em que cada linha sera um individuo (solução) ----------------------
    i=0;
    int **geracao_de_individuos = (int**) malloc(qtd_itens * sizeof(int*));
    for(i=0; i<QTD_INDIVIDUOS; i++)
    {
        geracao_de_individuos[i] = (int*) malloc(qtd_itens * sizeof(int));
    }

    criar_geracao_inicial(qtd_itens, geracao_de_individuos);//atribui aleatoriamente valores 0 ou 1 em cada posicao
    //---------------------------------------------------

    preencher_dados (qtd_itens, geracao_de_individuos, dados_individuos, elementos);

    imprimir_elementos(elementos, qtd_itens);    

    imprimir_matriz(qtd_itens, geracao_de_individuos, dados_individuos);
    
    fclose(mochila);
    free(geracao_de_individuos);
    return 0;
}

void imprimir_elementos(item *vetor, int qtd_itens){
    int i;
    for  (i=0; i<qtd_itens; i++){
       printf("Elemento [%d]: ganho: %.2d \t peso: %.2d\n", i, vetor[i].ganho, vetor[i].peso);
    }
}

void criar_geracao_inicial (int qtd_itens, int **geracao_de_individuos){
    srand(time(0));
    int i, j;
    for (i=0; i<QTD_INDIVIDUOS; i++){
        for (j=0; j<qtd_itens; j++){
            geracao_de_individuos[i][j] = rand()%2;
        }
    }
}

void imprimir_matriz(int qtd_itens, int **geracao_de_individuos, individuo *dados_individuos){
    int i, j;
    for (i=0; i<QTD_INDIVIDUOS; i++){
        printf("[");
        for (j=0; j<qtd_itens; j++){
            printf(" %d", geracao_de_individuos[i][j]);
        }
        printf("]   Ganho Total %d       Peso total: %d \n", dados_individuos[i].ganho_total, dados_individuos[i].peso_total);
    }
    printf("\n\n");
}

void preencher_dados (int qtd_itens, int **geracao_de_individuos, individuo *dados_individuos, item *elementos){
    int i, j, soma_peso, soma_ganho;
    for (i=0; i<QTD_INDIVIDUOS; i++){
        soma_peso = 0;
        soma_ganho = 0;
        for (j=0; j<qtd_itens; j++){
            if(geracao_de_individuos[i][j] == 1){
                soma_peso = soma_peso + elementos[j].peso;
                soma_ganho = soma_ganho +elementos[j].ganho;
            } 
        }
        dados_individuos[i].peso_total = soma_peso;
        dados_individuos[i].ganho_total = soma_ganho;
    }
}