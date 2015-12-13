#include <stdio.h>
int main(void)
{
    int vetor[10] = {2, 3, 9, 8, 10, 4, 1, 6, 7, 5};
    int tamanho = 10;
    int i, j, aux;
    for (i=0; i<tamanho; i++){
        for (j=i; j>0; j--){
            if (vetor[j] < vetor[j-1]){
                aux = vetor[j];
                vetor[j] = vetor[j-1];
                vetor[j-1] = aux;
            }
        }
    }
    for(i=0;i<tamanho;i++){
        printf("%d\n", vetor[i]);
    }
}
