#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define  QTD_INDIVIDUOS  10
#define  GERACOES  20

void imprimir_elementos(int* vetor_valor_itens, int* vetor_peso_itens, int qtd_itens);
void imprimir_vetor(int* vetor, int tamanho);
void imprimir_matriz(int** matriz, int colunas, int linhas);
void imprimir_elite_master(int** matriz, int* lista_valor, int qtd_itens);
void imprimir_media_geracao(int** matriz, int* lista_valor, int qtd_itens);

int* aloca_vetor_zerado(int qtd_itens);
int** aloca_matriz_zerada(int qtd_itens, int linhas);
void mata_vetor(int* ponteiro);
void mata_matriz(int** ponteiro);

void copia_matriz(int** matriz, int colunas, int linhas, int** matriz_duplicada);
void copia_vetor(int* vet_destino, int*vet_fonte, int tamanho);

void primeira_geracao(int* vetor_valor_itens, int* vetor_peso_itens, int** matriz_geracao, int qtd_itens, int capacidade_maxima_mochila);
void elitiza(int** elite, int** matriz, int* vetor_valor, int qtd_itens);
void geracao(int* pai, int* mae, int* vetor_peso_itens, int** geracao, int** copia_geracao, int** elite, int qtd_itens, int capacidade_mochila);
int selecionar_parente(int* parente, int** geracao_anterior, int** elite, int qtd_itens, int aux);

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

    vetor_valor_itens = aloca_vetor_zerado(qtd_itens);// cria um vetor de valor dos itens
    vetor_peso_itens = aloca_vetor_zerado(qtd_itens);// cria um vetor de peso dos itens
    while(!feof(mochila)){
        fscanf(mochila, "%d %d", &vetor_valor_itens[i], &vetor_peso_itens[i]);
        i++;
    }
    //-------------Fim Leitura ----------------------

    imprimir_elementos(vetor_valor_itens, vetor_peso_itens, qtd_itens);

    matriz_geracao = aloca_matriz_zerada(qtd_itens, QTD_INDIVIDUOS);// cria uma matriz da prox geracao
    matriz_duplicada = aloca_matriz_zerada(qtd_itens, QTD_INDIVIDUOS);
    elite = aloca_matriz_zerada(qtd_itens, QTD_INDIVIDUOS/3); //porcetagem elitismo
    pai = aloca_vetor_zerado(qtd_itens);
    mae = aloca_vetor_zerado(qtd_itens);
    

   

    primeira_geracao(vetor_valor_itens, vetor_peso_itens, matriz_geracao, qtd_itens, capacidade_maxima_mochila); // geracao aleatoria
    copia_matriz(matriz_geracao, QTD_INDIVIDUOS, qtd_itens, matriz_duplicada);
    
    printf("Primeira Geracao:");
    imprimir_media_geracao(matriz_geracao, vetor_valor_itens, qtd_itens);
    
    for(i = 0; i < GERACOES; i++){
        elitiza(elite, matriz_duplicada, vetor_valor_itens, qtd_itens);
        geracao(pai, mae, vetor_peso_itens,matriz_geracao, matriz_duplicada, elite, qtd_itens, capacidade_maxima_mochila);
        copia_matriz(matriz_geracao, QTD_INDIVIDUOS, qtd_itens, matriz_duplicada);
        printf("Media de valor da %d Geracao:", i+2);
        imprimir_media_geracao(matriz_geracao, vetor_valor_itens, qtd_itens);
    }
    
    imprimir_elite_master(elite, vetor_valor_itens, qtd_itens);
    

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
void imprimir_vetor(int* vetor, int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n----------------\n");
}
void imprimir_matriz(int** matriz, int colunas, int linhas){
    for(int j=0; j<colunas; j++){ //imprimir todos os cromossomos 
        for(int i=0; i<linhas; i++){
            printf("%d", matriz[i][j]);
        }
        printf("\n");
    }
    printf("----------------\n");
}
void imprimir_elite_master(int** matriz, int* lista_valor, int qtd_itens){
    int i, j, valores_elite = 0;

    printf("Valores da populcao elite na ultima geracao!\n");
    for(i = 0; i < QTD_INDIVIDUOS/3; i++){
        valores_elite = 0;
        for(j=0; j < qtd_itens; j++){
            if(matriz[j][i]==1){
                valores_elite = valores_elite + lista_valor[j]; 
            }  
        }
        
        for ( j = 0; j < qtd_itens; j++)
        {
            printf("%d",matriz[j][i]);
        }
        printf(" Valor: %d\n", valores_elite);
    }
    printf("----------------\n");
}


int* aloca_vetor_zerado(int tamanho){
    int *vetor;
    vetor = (int *)malloc(tamanho * sizeof(int));
    for(int i = 0; i < tamanho; i++){
        vetor[i] = 1;
    }
    return vetor;
}
int** aloca_matriz_zerada(int colunas, int linhas){
    int **vetor_de_vetor, *aux;
    vetor_de_vetor = (int**)malloc(colunas * sizeof(int*));
    for(int i = 0; i < colunas; i++){
        vetor_de_vetor[i] = aloca_vetor_zerado(linhas);
    }
    return vetor_de_vetor;
}
void mata_vetor(int* ponteiro){
    free(ponteiro);
}
void mata_matriz(int** ponteiro){
    free(ponteiro);
}

void copia_matriz(int** matriz, int colunas, int linhas, int** matriz_duplicada){
    int i,j;
    for(j = 0; j < colunas; j++){
        for(i = 0; i < linhas; i++){
            matriz_duplicada[i][j] = matriz[i][j];
        }
    }
}
void copia_vetor(int* vet_destino, int*vet_fonte, int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        vet_destino[i] = vet_fonte[i];
    }
}
void primeira_geracao(int* vetor_valor_itens,int* vetor_peso_itens, int** matriz_geracao ,int qtd_itens, int capacidade_maxima_mochila){
    int i, j, k, peso_total;
    srand((unsigned) time(NULL));
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
    printf("Primeira Geracao\n");
    imprimir_matriz(matriz_geracao, QTD_INDIVIDUOS, qtd_itens);
}
void elitiza(int** elite, int** matriz, int* vetor_valor, int qtd_itens){
    int valor, i, j, k, l, ja_verificado, valores_elite[QTD_INDIVIDUOS/3];
    
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
        //variável auxiliar para identificar caso o número já tenha sido verificado em 1.
        ja_verificado = 0;
        for(k = 0; k < QTD_INDIVIDUOS/3; k++){
            if((valor >= valores_elite[k]) && (ja_verificado == 0)){
                ja_verificado = 1;
                for(i = QTD_INDIVIDUOS/3 -1; i > k ; i--){
                    valores_elite[i+1] = valores_elite[i];
                    for(l = qtd_itens-1; l > 0; l--){
                        elite[i][l+1] = elite[i][l];
                    }
                }
                valores_elite[k] = valor;
                for(l = 0; l < qtd_itens; l++){
                    elite[l][k] = matriz[l][j];
                }
            }
        }
    }
}

int selecionar_parente(int* parente, int** geracao_anterior, int** elite, int qtd_itens, int aux){
    srand((unsigned) time(NULL));
    int probabilidade_elite, aleatorio, i, j;
    probabilidade_elite = rand()%10;
    if(probabilidade_elite == 0 || probabilidade_elite == 1 || probabilidade_elite == 2){
        do{
           aleatorio = rand()%(QTD_INDIVIDUOS/3);
        }while(aleatorio == aux);
            
        for(i = 0; i<qtd_itens; i++){
            parente[i] = elite[i][aleatorio];
        }
    }
    else{
        do{
           aleatorio = rand()%(QTD_INDIVIDUOS);
        }while(aleatorio == aux);
        for(i = 0; i < qtd_itens; i++){
            parente[i] = geracao_anterior[i][aleatorio];
        }
    }
    return aleatorio;
}

void geracao(int* pai, int* mae, int* vetor_peso_itens, int** geracao, int** geracao_anterior, int** elite, int qtd_itens, int capacidade_mochila){
    int i, j, peso_total, chance_mutacao, aux;
    srand((unsigned) time(NULL)); //Numero Aleatorio ?
    for(i = 0; i < QTD_INDIVIDUOS/3; i++){
        for(j = 0; i < qtd_itens; i++){
            geracao[i][j] = elite[i][j];
        }
    }
    for(j = QTD_INDIVIDUOS/3; j < QTD_INDIVIDUOS; j++){ 
        do{
            aux = QTD_INDIVIDUOS;
            aux = selecionar_parente(pai, geracao_anterior, elite, qtd_itens, aux);
            aux = selecionar_parente(mae, geracao_anterior, elite, qtd_itens, aux);
            for(i = 0; i < qtd_itens/2; i++){
                geracao[i][j] = pai[i];
            }
            for(i=qtd_itens/2; i < qtd_itens; i++){
                geracao[i][j] = mae[i];
            }
            if( j > (QTD_INDIVIDUOS-(QTD_INDIVIDUOS/3) ) ){
                for(i = 0; i < qtd_itens; i++){
                    chance_mutacao = rand()%10;
                    if( chance_mutacao <= 1){
                        if(geracao[i][j] == 1){
                            geracao[i][j] = 0;
                        }
                        else{
                            geracao[i][j] = 1;
                        }
                    }  
                }
            }
            peso_total=0;
            for(i = 0; i < qtd_itens; i++){ 
                if(geracao[i][j] == 1){
                    peso_total = peso_total + vetor_peso_itens[i];
                }  
            }
        }while(peso_total > capacidade_mochila); 
    }
}

void imprimir_media_geracao(int** matriz, int* lista_valor, int qtd_itens){
    int i, j, soma=0, media = 0;

    for(i = 0; i < QTD_INDIVIDUOS; i++){
        for(j=0; j < qtd_itens; j++){
            soma = soma + matriz[j][i] * lista_valor[j];
        }
    }
    media = soma/QTD_INDIVIDUOS;
    printf("%d",media);
    printf("\n----------------\n");

    
}
