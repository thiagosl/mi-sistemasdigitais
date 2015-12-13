void main(void)
{
    int vet[] = {1, 3, 5, 7, 9, 11, 12, 13, 14, 16};
    int chave = 7;
    int tam = 10;
    int inf = 0; //Limite inferior      (o primeiro elemento do vetor em C é zero          )
    int sup = tam-1; //Limite superior    (termina em um número a menos 0 à 9 são 10 numeros )
    int meio;
    int result = -1;
    while (inf <= sup)
    {
        meio = (inf + sup)/2;
        if (chave == vet[meio])
            result = meio;
        if (chave < vet[meio])
            sup = meio-1;
        else
            inf = meio+1;
    }
    printf("%d", result);
}
