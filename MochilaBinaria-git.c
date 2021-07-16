#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define  QTD_INDIVIDUOS  10
#define  GERACOES  10

void imprimir_elementos(int* vetor_valor_itens, int* vetor_peso_itens, int qtd_itens);
void primeira_geracao(int* vetor_valor_itens, int* vetor_peso_itens, int** matriz_geracao, int qtd_itens, int capacidade_maxima_mochila);
int* gera_vetor(int qtd_itens);
int** gera_matriz(int qtd_itens, int linhas);
void duplica_matriz(int** matriz, int colunas, int** matriz_duplicada);
void mata_vetor(int* ponteiro);
void mata_matriz(int** ponteiro);
void elitiza(int** elite, int** matriz, int* vetor_valor, int qtd_itens);

int main (){

    int capacidade_maxima_mochila, i=0, qtd_itens;
    FILE *mochila;
    int *vetor_valor_itens, *vetor_peso_itens, **matriz_geracao, **matriz_duplicada, *pai, *mae, **elite;
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
    matriz_geracao = gera_matriz(qtd_itens, QTD_INDIVIDUOS);// cria uma matriz da prox geracao
    
    while(!feof(mochila)){
        fscanf(mochila, "%d %d", &vetor_valor_itens[i], &vetor_peso_itens[i]);
        i++;
    }
    //-------------Fim Leitura ----------------------

    imprimir_elementos(vetor_valor_itens, vetor_peso_itens, qtd_itens);
    primeira_geracao(vetor_valor_itens, vetor_peso_itens, matriz_geracao, qtd_itens, capacidade_maxima_mochila); // geracao aleatoria
    
    matriz_duplicada = gera_matriz(qtd_itens, QTD_INDIVIDUOS);
    duplica_matriz(matriz_geracao, qtd_itens, matriz_duplicada);
    pai = gera_vetor(qtd_itens);
    mae = gera_vetor(qtd_itens);
    elite = gera_matriz(qtd_itens, QTD_INDIVIDUOS/3); //porcetagem elitismo
    elitiza(elite, matriz_duplicada, vetor_valor_itens, qtd_itens);
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

                if(matriz_geracao[k][j] == 1){ //verificar se aquele cromossomo tem determinado item 
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
int** gera_matriz(int colunas, int linhas){
    int **vetor_de_vetor, *aux;
    vetor_de_vetor = (int**)malloc(colunas * sizeof(int*));
    for(int i = 0; i < colunas; i++){
        vetor_de_vetor[i] = gera_vetor(linhas);
    }
    return vetor_de_vetor;
}
void duplica_matriz(int** matriz, int colunas, int** matriz_duplicada){
    int i,j;
    for(j = 0; j < QTD_INDIVIDUOS; j++){
        for(i = 0; i < colunas; i++){
            matriz_duplicada[i][j] = matriz[i][j];
        }
    }
}
void mata_vetor(int* ponteiro){
    free(ponteiro);
}
void mata_matriz(int** ponteiro){
    free(ponteiro);
}

void elitiza(int** elite, int** matriz, int* vetor_valor, int qtd_itens){
    int valor, i, j, k, l, aux, valores_elite[QTD_INDIVIDUOS/3];
    
    for(k = 0; k < QTD_INDIVIDUOS/3; k++){ //zerar valores elite
        valores_elite[k] = 0;
    }

    for(j = 0; j < QTD_INDIVIDUOS; j++){     
        valor = 0;
        for(i = 0; i < qtd_itens; i++){ 
            if(matriz[i][j] == 1){ 
               valor = valor + vetor_valor[i];
            }
        }
        for(k = 0; k < QTD_INDIVIDUOS/3; k++){//está com problema pq verifica todos os itens do vetor mesmo que já tenha comparado com o anterior e dado correto.
            if(valor >= valores_elite[k]){
                for(i = QTD_INDIVIDUOS/3 -1; i > k ; i--){
                    valores_elite[i+1] = valores_elite[i];
                    for(l = qtd_itens-1; l > 0; l--){
                        elite[i][l] = elite[i][l+1];
                    }
                }
                valores_elite[k] = valor;
                printf("v:%d ve:%d  ", valor, valores_elite[k]);
                
                for(l = 0; l < qtd_itens; l++){
                    elite[l][k] = matriz[l][j];
                }
            }
        }
    }
    for(i = 0; i < QTD_INDIVIDUOS/3; i++){
        for ( j = 0; j < qtd_itens; j++)
        {
            printf("%d ",elite[j][i]);
        }
        printf(" Valor: %d\n", valores_elite[i]);
    }
}
