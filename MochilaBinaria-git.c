#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define  QTD_INDIVIDUOS 100
#define  GERACOES 40
#define  PARAMETRO 0.6
#define  ELITE QTD_INDIVIDUOS*15/100
#define  RALE QTD_INDIVIDUOS*5/100


void imprimir_elementos(int* lista_valor, int* lista_peso, int qtd_itens);
void imprimir_media_geracao(float** matriz, int* lista_valor, int qtd_itens);

float* aloca_lista_float_zerada(int tamanho);
int* aloca_lista_int_zerada(int tamanho);
float** aloca_matriz_zerada(int colunas, int linhas);
void mata_vetor_int(int* ponteiro);
void mata_vetor_float(float* ponteiro);
void mata_matriz_float(float** ponteiro);

void copia_matriz(float** matriz, float** matriz_duplicada, int colunas, int linhas);

void gerar_aleatorio(float** matriz_geracao, int* lista_peso, int qtd_itens, int capacidade_maxima_mochila, int inicio);
void ordenar(float** matriz, int* vetor_valor, int qtd_itens);
void geracao(float** geracao, float** geracao_anterior, float* pai, float* mae, int lista_peso[], int qtd_itens, int capacidade_mochila);
void filho(float** geracao_anterior, float* pai, float* mae, float* filho, int qtd_itens);
void decodificador(float** matriz, int posicao, int qtd_itens, int lista_peso[], int lista_valor[]);

int main (){
    int capacidade_maxima_mochila, i=0, qtd_itens;
    FILE *mochila;
    int *lista_valor, *lista_peso;
    float **matriz_geracao, **matriz_duplicada, *pai, *mae;
    srand((unsigned) time(NULL));

    // Ler o arquivo mochila.txt e armazena num vetor de struct de itens
    mochila = fopen ("mochila.txt", "r");
    if(mochila == NULL){
        printf("Erro na abertura do arquivo\n\n");
        return 1;
    }
    fscanf(mochila, "%d %d", &capacidade_maxima_mochila, &qtd_itens);
    printf("Capacidade maxima da mochila: %d \t quantidade de itens: %d\n", capacidade_maxima_mochila, qtd_itens);
    lista_valor = aloca_lista_int_zerada(qtd_itens);// cria um vetor de valor dos itens
    lista_peso = aloca_lista_int_zerada(qtd_itens);// cria um vetor de peso dos itens
    while(!feof(mochila)){
        fscanf(mochila, "%d %d", &lista_valor[i], &lista_peso[i]);
        i++;
    }
    imprimir_elementos(lista_valor, lista_peso, qtd_itens);
    //-------------Fim Leitura ----------------------

    matriz_geracao = aloca_matriz_zerada(QTD_INDIVIDUOS, qtd_itens);// cria uma matriz da proxima geracao
    matriz_duplicada = aloca_matriz_zerada(QTD_INDIVIDUOS, qtd_itens);
    pai = aloca_lista_float_zerada(qtd_itens);
    mae = aloca_lista_float_zerada(qtd_itens);

    gerar_aleatorio(matriz_geracao, lista_peso, qtd_itens, capacidade_maxima_mochila, 0); // geracao aleatoria
    ordenar(matriz_geracao, lista_valor, qtd_itens);
    copia_matriz(matriz_geracao, matriz_duplicada, QTD_INDIVIDUOS, qtd_itens);
    
    printf("Primeira Geracao:");
    for(i = 0; i < QTD_INDIVIDUOS; i++){
        decodificador(matriz_geracao, i, qtd_itens, lista_peso, lista_valor);
    }
    printf("Media de valor da 1 Geracao:");
    imprimir_media_geracao(matriz_geracao, lista_valor, qtd_itens);

    for(i = 1; i < GERACOES; i++){
        geracao(matriz_geracao, matriz_duplicada, pai, mae, lista_peso, qtd_itens, capacidade_maxima_mochila);
        ordenar(matriz_geracao, lista_valor, qtd_itens);
        copia_matriz(matriz_geracao, matriz_duplicada, QTD_INDIVIDUOS, qtd_itens);
        printf("\n----------------\n");
        printf("Media de valor da %d Geracao:", i+1);
        imprimir_media_geracao(matriz_geracao, lista_valor, qtd_itens);
        for(int j = 0; j < ELITE; j++){
            decodificador(matriz_geracao, j, qtd_itens, lista_peso, lista_valor);
        }
    }
    mata_vetor_int(lista_peso);
    mata_vetor_int(lista_valor);
    mata_vetor_float(pai);
    mata_vetor_float(mae);
    mata_matriz_float(matriz_geracao);
    mata_matriz_float(matriz_duplicada);
    fclose(mochila);
    return 0;
}
//ok
void imprimir_elementos(int* lista_valor, int* lista_peso, int qtd_itens){
    int i;
    for  (i=0; i<qtd_itens; i++){
       printf("Elemento [%d]: valor: %.2d \t peso: %.2d\n", i, lista_valor[i], lista_peso[i]);
    }
}
//ok
void imprimir_media_geracao(float** matriz, int* lista_valor, int qtd_itens){
    int i, j, soma=0, media = 0;

    for(j = 0; j < QTD_INDIVIDUOS; j++){
        for(i=0; i < qtd_itens; i++){
            if(matriz[j][i] >= PARAMETRO){
                soma = soma + lista_valor[i];
            }
        }
    }
    media = soma/QTD_INDIVIDUOS;
    printf("%d",media);
    
}
//ok
float* aloca_lista_float_zerada(int tamanho){
    float *vetor;
    vetor = (float *)malloc(tamanho * sizeof(float));
    for(int i = 0; i < tamanho; i++){
        vetor[i] = 1;
    }
    return vetor;
}
//ok
int* aloca_lista_int_zerada(int tamanho){
    int *vetor;
    vetor = (int *)malloc(tamanho * sizeof(int));
    for(int i = 0; i < tamanho; i++){
        vetor[i] = 1;
    }
    return vetor;
}
//ok
float** aloca_matriz_zerada(int colunas, int linhas){
    float **vetor_de_vetor;
    vetor_de_vetor = (float**)malloc(colunas * sizeof(float*));
    for(int i = 0; i < colunas; i++){
        vetor_de_vetor[i] = aloca_lista_float_zerada(linhas);
    }
    return vetor_de_vetor;
}
void mata_vetor_int(int* ponteiro){
    free(ponteiro);
}
void mata_vetor_float(float* ponteiro){
    free(ponteiro);
}
void mata_matriz_float(float** ponteiro){
    free(ponteiro);
}
//ok
void copia_matriz(float** matriz, float** matriz_duplicada, int colunas, int linhas){
    int i,j;
    for(j = 0; j < colunas; j++){
        for(i = 0; i < linhas; i++){
            matriz_duplicada[j][i] = matriz[j][i];
        }
    }
}
//ok
void gerar_aleatorio(float** matriz_geracao, int* lista_peso, int qtd_itens, int capacidade_maxima_mochila, int inicio){
    int i, j, k, peso_total;
    for(j = inicio; j < QTD_INDIVIDUOS; j++){ //numero de cromossomos
        do{
            for(i = 0; i < qtd_itens; i++){//criar um cromossomo 
                matriz_geracao[j][i] = rand()%100/100.00; //cada casa da matriz irá receber 0 ou 1 aleatoriamente
            }
            peso_total=0;
            for(k = 0; k < qtd_itens; k++){ //apos criar um cromossomo
                if(matriz_geracao[j][k] >= PARAMETRO){ //verificar se aquele cromossomo tem determinado item 
                    peso_total = peso_total + lista_peso[k];
                }
            }
        }while(peso_total > capacidade_maxima_mochila); //caso o cromossomo criado ultrapasse o peso limite deve-se criar outro cromossomo 
    }
}
//ok
void ordenar(float** matriz, int* vetor_valor, int qtd_itens){
    int valor, i, j, k, *lista_valores;
    float aux;
    lista_valores = aloca_lista_int_zerada(QTD_INDIVIDUOS);
    
    for(j = 0; j < QTD_INDIVIDUOS; j++){     // preencher vetor de valores do individuo
        valor = 0;
        for(i = 0; i < qtd_itens; i++){ 
            if(matriz[j][i] >= PARAMETRO){ 
               valor = valor + vetor_valor[i];
            }
        }
        lista_valores[j] = valor;
    }
    
    for(j = 0; j < QTD_INDIVIDUOS-1; j++){
        for(i = j+1; i < QTD_INDIVIDUOS; i++){
            if(lista_valores[i] > lista_valores[j]){
                valor = lista_valores[j];
                lista_valores[j] = lista_valores[i];
                lista_valores[i] = valor;

                for(k = 0; k < qtd_itens; k++){
                    aux = matriz[j][k];
                    matriz[j][k] = matriz[i][k];
                    matriz[i][k] = aux;
                }
            }
        }
    }
    mata_vetor_int(lista_valores);
}
//ok
void filho(float** geracao_anterior, float* pai, float* mae, float* filho, int qtd_itens){
    int aleatorio, i, j;
    aleatorio = rand();
    pai = geracao_anterior[aleatorio%(ELITE)];
    aleatorio = rand();
    mae = geracao_anterior[ELITE + aleatorio%(QTD_INDIVIDUOS-ELITE)];
    for(i = 0; i < qtd_itens; i++){
        aleatorio = rand()%100/100.00;
        if(aleatorio >= 0.5){
            filho[i] = pai[i];
        }     
        else{
            filho[i] = mae[i];
        }
    }     
}
//ok
void geracao(float** geracao, float** geracao_anterior, float* pai, float* mae, int lista_peso[], int qtd_itens, int capacidade_mochila){
    int i, j, peso_total, aux;
    for(j = 0; j < ELITE; j++){
        for(i = 0; i < qtd_itens; i++){
            geracao[j][i] = geracao_anterior[j][i];
        }
    }
    for(j = ELITE; j < QTD_INDIVIDUOS - RALE; j++){ 
        do{
            filho(geracao_anterior, pai, mae, geracao[j], qtd_itens);
            peso_total=0;
            for(i = 0; i < qtd_itens; i++){ 
                if(geracao[j][i] >= PARAMETRO){
                    peso_total = peso_total + lista_peso[i];
                }  
            }
        }while(peso_total > capacidade_mochila); 
    }
    gerar_aleatorio(geracao, lista_peso, qtd_itens, capacidade_mochila, QTD_INDIVIDUOS-RALE);
}
//ok
void decodificador(float** matriz, int posicao, int qtd_itens, int lista_peso[], int lista_valor[]){
    int i, peso = 0, valor = 0;
    printf("\n");
    for(i = 0; i < qtd_itens; i++){
        if(matriz[posicao][i] >= PARAMETRO){
            peso = peso + lista_peso[i];
            valor = valor + lista_valor[i];
            printf("1\t");
        }
        else{
            printf("0\t");
        }
    }
    printf("\n");

    for(i = 0; i < qtd_itens; i++){
        printf("%.2f\t", matriz[posicao][i]);
    }
    printf("\n Peso: %d, Valor: %d\n", peso, valor);
    
}