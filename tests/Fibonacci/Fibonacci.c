#include <stdio.h>
int main(void)
{
    int num1 = 1;
    int num2 = 1;
    int n = 45;
    int aux, i;
    for (i = 2; i < n; i++)
    {
        aux = num1 + num2;
        num1 = num2;
        num2 = aux;
    }
    printf("%d", num2);
}
