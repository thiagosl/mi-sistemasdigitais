#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "uthash.h"

typedef int bool;
#define true 1
#define false 0

char nomeArq[] = "teste.asm";
int posAtual = 0;

struct hash {
    char *key;                /* key */
    int valor;
    UT_hash_handle hh;        /* faz dessa struct uma hashtable */
};

struct hash *registradores = NULL;
struct hash *labels = NULL;
struct hash *flags = NULL;

void addRegist(char key[], int valor) {
    struct hash *s;
    s = malloc(sizeof(struct hash));
    s->key = key;
    strcpy(s->valor, valor);
    HASH_ADD_STR(registradores, key, s);
}

void addLabel(char key[], int valor) {
    struct hash *s;
    HASH_FIND_STR(labels, &key, s);
    if (s==NULL) {
      s = (struct hash*)malloc(sizeof(struct hash));
      s->key = key;
      HASH_ADD_STR(labels, key, s);
    }
    strcpy(s->valor, valor);
}

int findRegist(int key) {
    struct hash *s;

    HASH_FIND_STR(registradores, &key, s);
    return s->valor;
}

int findLabel(int key) {
    struct hash *s;

    HASH_FIND_STR(labels, &key, s);
    return s->valor;
}

char* tiraEspacos(char s[]) {
    char novo[100];
    char c;
    bool b = false;
    int i = 0;
    do {
        c = s[i];
        if (c != ' ' && c != '\t') {
            b = true;
        }
        i++;
    } while(c != '\0' && b == false);
    if (s[--i] == '\0') {
        return NULL;
    } else {
        int j = 0;
        do {
            c = s[i];
            novo[j] = c;
            i++;
            j++;
        } while(c != '\0' && b == false);
    }

    return novo;
}

void criaRegistradores() {
    char key[100];
    int i;
    for (i = 0; i < 16; i++) {
        sprintf(key, "r%d", i);
        addRegist(key, i);
    }
}

char* ignoraLabelsEComents(char instrucao[100]) {
    char *aux = strchr(instrucao, ':');
    if (aux != NULL)
        instrucao = ++aux;

    instrucao = tiraEspacos(instrucao);
    if(instrucao == NULL || instrucao[0] == ';')
        return NULL;

    return instrucao;
}

void criaLabels() {
	char *linha;
	FILE *arq;
	int count = 0;

	arq = fopen(nomeArq, "r");
	if(arq == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else {
        bool pseg = false;
        bool dseg = false;
        bool module = false;
	    while(fgets(linha, sizeof(linha), arq) != NULL) {
	        linha = tiraEspacos(linha);
            if (linha != NULL && linha[0] == '.' && linha[0] != ';') {
                char diretiva[100];
                int i = 0;
                do {
                    diretiva[i] = linha[i];
                    i++;
                } while(linha[i] != ' ' || linha[i] != '\n' || linha[i] != '\t' || linha[i] != ';');
                diretiva[i] = '\0';
                if (strcmp(diretiva, ".module")) {
                    module = true;
                } else if (strcmp(diretiva, ".end")) {
                    module = false;
                } else if (strcmp(diretiva, ".pseg")) {
                    pseg = true;
                    dseg = false;
                } else if (strcmp(diretiva, ".dseg")) {
                    pseg = false;
                    dseg = true;
                } else if (strcmp(diretiva, ".word")) {
                    if (dseg && module) {
                        count += 4;
                    }
                }
            } else if (linha != NULL && linha[0] != ';' && linha[0] != '.' && module && pseg) {
                char *aux = strchr(linha, ':');
                if (aux != NULL) {
                    linha = strrev(linha);
                    char *label = strchr(linha, ':') + 1;
                    label = strrev(label);
                    addLabel(label, count);

                    aux += 1;
                    if (ignoraLabelsEComents(aux) != NULL) {
                        count++;
                    }
                } else {
                    count++;
                }
            }
	    }
    }
	fclose(arq);
}

char* getMneumonico(char instrucao[]) {
    char mneumonico[100];
    int i = 0;
    do {
        mneumonico[i] = instrucao[i];
        i++;
    } while(instrucao[i] != ' ' || instrucao[i] != '\n' || instrucao[i] != '\t' || instrucao[i] != ';');
    mneumonico[i] = '\0';

    return mneumonico;
}

int getR1(char instrucao[]) {
    instrucao = strchr(instrucao, ' ');
    instrucao = tiraEspacos(instrucao);
    char reg[100];
    reg[0] = instrucao[0];
    reg[1] = instrucao[1];
    reg[2] = '\0';

    return (findRegist(reg) << 8);
}

int getR2(char instrucao[]) {
    instrucao = strchr(instrucao, ',') + 1;
    instrucao = tiraEspacos(instrucao);
    char reg[100];
    reg[0] = instrucao[0];
    reg[1] = instrucao[1];
    reg[2] = '\0';

    return (findRegist(reg) << 4);
}

int getR3(char instrucao[]) {
    instrucao = strchr(instrucao, ',') + 1;
    instrucao = strchr(instrucao, ',') + 1;
    instrucao = tiraEspacos(instrucao);
    char reg[100];
    reg[0] = instrucao[0];
    reg[1] = instrucao[1];
    reg[2] = '\0';

    return findRegist(reg);
}

int getLabel(char instrucao[]) {
    instrucao = strchr(instrucao, ' ') + 1;
    instrucao = tiraEspacos(instrucao);

    char label[100];
    int i = 0;
    do {
        label[i] = instrucao[i];
        i++;
    } while(instrucao[i] != ' ' || instrucao[i] != '\n' || instrucao[i] != '\t' || instrucao[i] != ';');
    label[i] = '\0';

    return findLabel(label);
}

int getConst(char instrucao[]) {
    instrucao = strchr(instrucao, ',') + 1;
    instrucao = tiraEspacos(instrucao);

    char constante[100];
    int i = 0;
    do {
        constante[i] = instrucao[i];
        i++;
    } while(instrucao[i] != ' ' || instrucao[i] != '\n' || instrucao[i] != '\t' || instrucao[i] != ';');
    constante[i] = '\0';

    if (constante[0] <= 9 && constante[0] >= 0)
        return atoi(constante);
    else if (strcmp(constante, "LOWBYTE")) {
        return (getLabel(instrucao) & 0x00001111);
    } else if (strcmp(constante, "HIBYTE")) {
        return (getLabel(instrucao) >> 16);
    } else {
        return (findLabel(constante) & 0x00001111);
    }
}

int getCondicao(char instrucao[]) {
    instrucao = strchr(instrucao, '.') + 1;
    char condicao[100];
    int i = 0;
    do {
        condicao[i] = instrucao[i];
        i++;
    } while(instrucao[i] != ' ');
    condicao[i] = '\0';

    if (strcmp(condicao, "overflow"))
        return 1;
    else if (strcmp(condicao, "zero"))
        return 2;
    else if (strcmp(condicao, "neg"))
        return 3;
    else if (strcmp(condicao, "negzero"))
        return 4;
    else if (strcmp(condicao, "true"))
        return 5;
    else if (strcmp(condicao, "carry"))
        return 6;
    else
        return 0;
}

int getDestino(char instrucao[]) {
    instrucao = strchr(instrucao, ',') + 1;
    instrucao = tiraEspacos(instrucao);
    instrucao = strrev(instrucao);
    instrucao = tiraEspacos(instrucao);
    instrucao = strrev(instrucao);

    int destino = atoi(instrucao);

    return destino;
}

int traduzir(char instrucao[]) {
    int resp = 0;
    char *mneumonico = getMneumonico(instrucao);
    if (strcmp(mneumonico, "add")) {
        resp = (0x00000 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "addu")) {
        resp = (0x00001 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "addinc")) {
        resp = (0x00002 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "inca")) {
        resp = (0x00003 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "sub")) {
        resp = (0x00004 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "subdec")) {
        resp = (0x00005 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "deca")) {
        resp = (0x00006 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "mult")) {
        resp = (0x00007 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "multu")) {
        resp = (0x00008 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "mfh")) {
        resp = (0x00009 << 12) + getR1(instrucao);
    } else if (strcmp(mneumonico, "mfl")) {
        resp = (0x0000A << 12) + getR1(instrucao);
    } else if (strcmp(mneumonico, "div")) {
        resp = (0x0000B << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "divu")) {
        resp = (0x0000C << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "asl")) {
        resp = (0x0000D << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "asr")) {
        resp = (0x0000E << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "zaros")) {
        resp = (0x0000F << 12) + getR1(instrucao);
    } else if (strcmp(mneumonico, "ones")) {
        resp = (0x00010 << 12) + getR1(instrucao);
    } else if (strcmp(mneumonico, "passa")) {
        resp = (0x00011 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "passnota")) {
        resp = (0x00012 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "and")) {
        resp = (0x00013 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "andnota")) {
        resp = (0x00014 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "nand")) {
        resp = (0x00015 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "or")) {
        resp = (0x00016 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "ornotb")) {
        resp = (0x00017 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "nor")) {
        resp = (0x00018 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "xor")) {
        resp = (0x00019 << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "xornota")) {
        resp = (0x0001A << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "xnor")) {
        resp = (0x0001B << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "lsl")) {
        resp = (0x0001C << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "lsr")) {
        resp = (0x0001D << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "slt")) {
        resp = (0x0001E << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "sltu")) {
        resp = (0x0001F << 12) + getR1(instrucao) + getR2(instrucao) + getR3(instrucao);
    } else if (strcmp(mneumonico, "loadlit")) {
        resp = (0x400 << 20) + (getR1(instrucao) << 8) + getConst(instrucao);
    } else if (strcmp(mneumonico, "lcl")) {
        resp = (0x401 << 20) + (getR1(instrucao) << 8) + getConst(instrucao);
    } else if (strcmp(mneumonico, "lch")) {
        resp = (0x402 << 20) + (getR1(instrucao) << 8) + getConst(instrucao);
    } else if (strcmp(mneumonico, "load")) {
        resp = (0x80000 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "store")) {
        resp = (0x80001 << 12) + getR1(instrucao) + getR2(instrucao);
    } else if (strcmp(mneumonico, "j")) {
        resp = (0xC00 << 20) + getDestino(instrucao);
        if (getDestino(instrucao) == posAtual)
            resp = 0x40000000;
    } else if (strcmp(mneumonico, "jt")) {
        resp = (0xC01 << 20) + getCondicao(instrucao); + getDestino(instrucao);
    } else if (strcmp(mneumonico, "jf")) {
        resp = (0xC02 << 20) + getCondicao(instrucao); + getDestino(instrucao);
    } else if (strcmp(mneumonico, "jal")) {
        resp = (0xC03 << 20) + (getR1(instrucao) << 4);
    } else if (strcmp(mneumonico, "jr")) {
        resp = (0xC04 << 20) + (getR1(instrucao) << 4);
    } else if (strcmp(mneumonico, "nop")) {
        resp = 0x20000000;
    } else {
        printf("ERRO: instrucao inexistente");
    }
    posAtual++;
}

void escreveArquivo(FILE *arq, int bin) {
    int i;
    for (i = 0; i < 32; i++) {
        if (bin >= 2147483648)
            fprintf(arq, "%d", 1);
        else
            fprintf(arq, "%d", 0);
        bin = (bin << 1);
    }
}

int main() {
    criaRegistradores();
    criaLabels();

    char *instrucao;
	FILE *arqR;
	FILE *arqW;
	arqR = fopen(nomeArq, "r");
	arqW = fopen("resultado.bin", "w");

    if(arqR == NULL)
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	else {
        bool pseg = false;
        bool dseg = false;
        bool module = false;
	    while(fgets(instrucao, sizeof(instrucao), arqR) != NULL) {
	        instrucao = ignoraLabelsEComents(instrucao);
            if (instrucao != NULL && instrucao[0] == '.') {
                char diretiva[100];
                int i = 0;
                do {
                    diretiva[i] = instrucao[i];
                    i++;
                } while(instrucao[i] != ' ' || instrucao[i] != '\n' || instrucao[i] != '\t' || instrucao[i] != ';');
                diretiva[i] = '\0';
                if (strcmp(diretiva, ".module")) {
                    module = true;
                } else if (strcmp(diretiva, ".end")) {
                    module = false;
                } else if (strcmp(diretiva, ".pseg")) {
                    pseg = true;
                    dseg = false;
                } else if (strcmp(diretiva, ".dseg")) {
                    pseg = false;
                    dseg = true;
                } else if (strcmp(diretiva, ".word")) {
                    if (dseg && module) {
                        instrucao += 5;
                        instrucao = tiraEspacos(instrucao);
                        char word[100];
                        int i = 0;
                        do {
                            word[i] = instrucao[i];
                            i++;
                        } while(instrucao[i] != ' ' || instrucao[i] != '\n' || instrucao[i] != '\t' || instrucao[i] != ';');
                        word[i] = '\0';

                        escreveArquivo(arqW, atoi(word));
                        posAtual++;
                    }
                }
            } else if (instrucao != NULL && module && pseg) {
                escreveArquivo(arqW, traduzir(instrucao));
            }
	    }
    }
    fclose(arqR);
    fclose(arqW);

    return 0;
}