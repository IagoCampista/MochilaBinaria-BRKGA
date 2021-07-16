#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define  QTD_INDIVIDUOS  10
#define  GERACOES  10

void imprimir_elementos(int* vetor_valor_itens, int* vetor_peso_itens, int qtd_itens);
void primeira_geracao(int* vetor_valor_itens, int* vetor_peso_itens, int** matriz_geracao, int qtd_itens, int capacidade_maxima_mochila);
int* gera_vetor(int qtd_itens);
int** gera_matriz(int qtd_itens);
void mata_vetor(int* ponteiro);
void mata_matriz(int** ponteiro);

int main (){

    int capacidade_maxima_mochila, i=0, qtd_itens;
    FILE *mochila;
    int *vetor_valor_itens, *vetor_peso_itens, **matriz_geracao;
    // Ler o arquivo mochila.txt e armazena num vetor de struct de itens
    mochila = fopen ("mochila.txt", "r");
    if(mochila == NULL){
        printf("Erro na abertura do arquivo\n\n");
        return 1;
    }

    fscanf(mochila, "%d %d", &capacidade_maxima_mochila, &qtd_itens);
    printf("Capacidade maxima da mochila: %d \t quantidade de itens: %d\n", capacidade_maxima_mochila, qtd_itens);

    vetor_valor_itens = gera_vetor(qtd_itens);// cria um vetor de valor dos itens
    vetor_peso_itens = gera_vetor(qtd_itens);// cria um vetor de peso dos itens
    matriz_geracao = gera_matriz(qtd_itens);// cria uma matriz da prox geracao
    
    while(!feof(mochila)){
        fscanf(mochila, "%d %d", &vetor_valor_itens[i], &vetor_peso_itens[i]);
        i++;
    }
    //-------------Fim Leitura ----------------------

    imprimir_elementos(vetor_valor_itens, vetor_peso_itens, qtd_itens);
    primeira_geracao(vetor_valor_itens, vetor_peso_itens, matriz_geracao, qtd_itens, capacidade_maxima_mochila); // geracao aleatoria
    
    mata_vetor(vetor_peso_itens);
    mata_vetor(vetor_valor_itens);
    mata_matriz(matriz_geracao);
    return 0;
}

void imprimir_elementos(int* vetor_valor_itens, int* vetor_peso_itens, int qtd_itens){
    int i;
    for  (i=0; i<qtd_itens; i++){
       printf("Elemento [%d]: valor: %.2d \t peso: %.2d\n", i, vetor_valor_itens[i], vetor_peso_itens[i]);
    }
}

void primeira_geracao(int* vetor_valor_itens,int* vetor_peso_itens, int** matriz_geracao ,int qtd_itens, int capacidade_maxima_mochila){
    int i, j, k, peso_total;
    for(j = 0; j < QTD_INDIVIDUOS; j++){ //numero de cromossomos
        do{
            for(i = 0; i < qtd_itens; i++){//criar um cromossomo 
                matriz_geracao[i][j] = rand()%2; //cada casa da matriz irá receber 0 ou 1 aleatoriamente
            }
            peso_total=0;
            for(k = 0; k < qtd_itens; k++){ //apos criar um cromossomo

                if(matriz_geracao[k][j] == 1) //verificar se aquele cromossomo tem determinado item 
                {
                    peso_total = peso_total + vetor_peso_itens[k];
                }  
            }
        }while(peso_total > capacidade_maxima_mochila); //caso o cromossomo criado ultrapasse o peso limite deve-se criar outro cromossomo
    }

    for(j=0; j<QTD_INDIVIDUOS; j++){ //imprimir todos os cromossomos 
        for(i=0; i<qtd_itens; i++){
            printf("%d", matriz_geracao[i][j]);
        }
        printf("\n");
    }

}

int* gera_vetor(int tamanho){
    int *vetor;
    vetor = (int *)malloc(tamanho * sizeof(int));
    return vetor;
}
int** gera_matriz(int colunas){
    int **vetor_de_vetor;
    vetor_de_vetor = (int **)malloc(colunas * sizeof(int));
    for(int i = 0; i < colunas; i++){
        vetor_de_vetor[i] = gera_vetor(QTD_INDIVIDUOS);
    }
    return vetor_de_vetor;
}
void mata_vetor(int* ponteiro){
    free(ponteiro);
}
void mata_matriz(int** ponteiro){
    free(ponteiro);
}
