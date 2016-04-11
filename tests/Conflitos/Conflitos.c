#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int i = 0;
	int num = 2;
	int aux = 0;
	int n = 18;

	for(i=0; i<n; i++){
		aux = num;
		num = num + num;
		num = num + aux;
	}
	printf("%d\n", num);
	return 0;
}