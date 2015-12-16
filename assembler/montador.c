#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "uthash.h"

typedef int bool;
#define true 1
#define false 0

struct hash {                   // Hashtable
    char key[100];              // Chave
    int value;                  // Valor
    UT_hash_handle hh;          // Faz dessa struct uma hashtable
};

struct hash *registers = NULL;  // Hashtable de registradores
struct hash *labels = NULL;     // Hashtable de labels
int numInstructions = 0;        // Num total de instrucoes
char *fileName;                 // Nome do arquivo

/* Adiciona registrador (nome, codigo) na hashmap */
void addRegist(char key[], int value) {
    struct hash *s;
    HASH_FIND_STR(registers, key, s);
    if (s==NULL) {                                      // Se nao existir na hash nenhum registrador com esse nome
      s = (struct hash*)malloc(sizeof(struct hash));
      strncpy(s->key, key, 100);
      s->value = value;
      HASH_ADD_STR(registers, key, s);                  // Adiciona registrador
    }
}

/* Adiciona label (nome, endereco) na hashmap */
void addLabel(char key[], int value) {
    struct hash *s;
    HASH_FIND_STR(labels, key, s);
    if (s==NULL) {                                      // Se nao existir na hash nenhum label com esse nome
      s = (struct hash*)malloc(sizeof(struct hash));
      strncpy(s->key, key, 100);
      s->value = value;
      HASH_ADD_STR(labels, key, s);                     // Adiciona label
    }
}

/* Procura registrador pelo nome na hashmap e retorna seu codigo */
int findRegist(char key[]) {
    struct hash *s;
    HASH_FIND_STR(registers, key, s);
    return s->value;
}

/* Procura label pelo nome na hashmap e retorna seu endereco */
int findLabel(char key[]) {
    struct hash *s;
    HASH_FIND_STR(labels, key, s);
    return s->value;
}

/* Tira todos os espacos e tabs a esquerda de string */
char* leftClean(char *s) {
    int i = 0;
    while(s[i] != '\0' && (s[i] == ' ' || s[i] == '\t')) {  // Pula todos os espacos e tabs a esquerda
        i++;
    }
    if (s[i] == '\0' || s[i] == ';')    // Se não sobrou nada ou somente um comentário
        return NULL;                    // Retorna null

    return strchr(s, s[i]);             // Se não, retorna a string sem os espacos iniciais
}

/* Armazena na hashtable os registradores (nome, codigo) r0 a r15 */
void createRegisters() {
    char key[100];
    int i;
    for (i = 0; i < 16; i++) {
        sprintf(key, "r%d", i);     // Chave (nome)
        addRegist(key, i);          // Adiciona registrador
    }
}

/* Ignora o comentario da linha, se tiver */
char* ignoreComents (char *s) {
    if (s[0] == ';')            // Se a linha ja comecar com comentario
        return NULL;

    s = strrev(s);              // Inverte a string
    char *aux = strchr(s, ';'); // Pega a posicao do "ponto e virgula" na string, se tiver
    while (aux != NULL) {       // Verifica se a linha contem comentario (ponto e virgula)
        s = ++aux;              // Tira tudo que tem antes do ponto e virgula (lembrando que a string esta invertida)
        aux = strchr(s, ';');
    }
    s = leftClean(s);           // Tira os espacos a esquerda da string, se tiver
    if (s != NULL)
        s = strrev(s);          // Desinverte a string

    return s;
}

/* Ignora o label e o comentario da linha, se tiver */
char* ignoreLabelsAndComents(char *instruction) {
    instruction = ignoreComents(instruction);   // Ignora o comentario da linha, se tiver
    if(instruction == NULL)
        return NULL;

    char *aux = strchr(instruction, ':');       // Pega a posicao do "dois pontos" na string, se tiver
    if (aux != NULL) {
        if (aux[1] == '\0')
            return NULL;
        instruction = ++aux;    // Ignora label (tudo antes do "dois pontos")
    }
    instruction = leftClean(instruction);       // Tira todos os espacos e tabs a esquerda da string, se tiver
    if(instruction == NULL)
        return NULL;

    return instruction;
}

/* o caractere '\n' do final da string */
char* removeLast(char aux[100]) {
    aux = strrev(aux);
    if (aux[0] == '\n')
        aux++;
    return strrev(aux);
}

/* Percorre o arquivo de entrada procurando labels e salvando-os na hashmap */
void createLabels() {
	char *line;
	FILE *file;
	int actualPosition = 0;
	int actualPositionData = 0;
	char aux[300];

	file = fopen(fileName, "r");  // Abre o arquivo
	if(file != NULL) {
        bool pseg = false;
        bool dseg = false;
        bool module = false;
	    while(fgets(aux, sizeof(aux), file) != NULL) {// Le as linhas do arquivo
	        line = removeLast(aux);                   // Tira o caractere '\n' da string
	        line = leftClean(line);                   // Tira os espacos e tabs a esquerda de string
	        if (line != NULL)
                line = ignoreComents(line);           // Ignora o comentario da linha, se tiver

            if (line != NULL && line[0] == '.') {     // Se a linha nao tiver vazia (era somente espacos e comentario) e for uma diretiva
                char directive[100];
                int i = 0;
                do {
                    directive[i] = line[i];            // Salva a primeira palavra da string na variavel "deretiva"
                    i++;
                } while(line[i] != ' ' && line[i] != '\0' && line[i] != '\t');
                directive[i] = '\0';
                if (!strcmp(directive, ".module")) {        // Verifica qual foi a diretiva lida
                    module = true;                      // Ativa o module (programa comecou)
                } else if (!strcmp(directive, ".end")) {
                    module = false;                     // Desativa o module (programa terminou)
                } else if (!strcmp(directive, ".pseg")) {
                    pseg = true;                        // Ativa o pseg (trecho de instrucoes comecou)
                    dseg = false;                       // Desativa o deseg (trecho de dados terminou)
                } else if (!strcmp(directive, ".dseg")) {
                    pseg = false;                       // Desativa o pseg (trecho de instrucoes terminou)
                    dseg = true;                        // Ativa o deseg (trecho de dados comecou)
                } else if (!strcmp(directive, ".word")) {
                    if (dseg && module) {       // Se tiver em um trecho de dados dentro do module
                        actualPositionData += 1;    // Incrementa a posicao atual da memoria de dados
                    }
                }
            } else if (line != NULL && line[0] != '.' && module) {      // Se a linha nao for uma diretiva
                char *aux = strchr(line, ':');      // Pega a posicao do "dois pontos" na string, se tiver
                if (aux != NULL) {
                    aux += 1;
                    if (ignoreLabelsAndComents(aux) != NULL) {   // Se a linha tiver uma instrucao além do label
                        if (ignoreLabelsAndComents(aux)[0] != '.' &&  pseg) {   // Se essa instrução não for uma diretiva
                            actualPosition++;                   // Incrementa a posicao atual da memoria
                        } else if (ignoreLabelsAndComents(aux)[0] == '.' && ignoreLabelsAndComents(aux)[0] == 'w' && dseg) { // Se essa instrução for ".word"
                            actualPositionData++;               // Incrementa a posicao atual da memoria de dados
                        }
                    }
                    line = strrev(line);                    // Inverte a string
                    char *label = strchr(line, ':') + 1;    // Pega tudo que esta depois do "dois pontos"
                    label = strrev(label);                  // Desinverte a string
                    if (dseg)       // Se o label estiver em uma área de dados
                        addLabel(label, (numInstructions + actualPositionData));    // Adiciona label na hashmap
                    else if (pseg)  // Se o label estiver em uma área de instruções
                        addLabel(label, actualPosition);        // Adiciona label na hashmap
                } else {
                    if (pseg) {
                        actualPosition++;   // Incrementa a posicao atual da memoria
                    }
                }
            }
	    }
    }
	fclose(file);    // Fecha o arquivo
}

/* Identifica e retorna o mnemonico da instrucao */
char* getMnemonic(char instruction[]) {
    char mnemonic[100];
    int i = 0;
    do {
        mnemonic[i] = instruction[i];   // Pega a primeira palavra (o mnemonico) da instrucao
        i++;
    } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t' && instruction[i] != '.');
    mnemonic[i] = '\0';

    return mnemonic;
}

/* Identifica e retorna o primeiro registrador da instrucao */
int getR1(char instruction[]) {
    instruction = strchr(instruction, ' ');     // Ignora a primeira palavra da instrucao
    instruction = leftClean(instruction);       // Limpaos os espacos a esquerda da string
    char reg[100];
    reg[0] = instruction[0];        // Pega os dois primeiros caracteres da string e, se precisar, o terceiro
    reg[1] = instruction[1];
    if (instruction[2] >= '0' && instruction[2] <= '9') {
        reg[2] = instruction[2];
        reg[3] = '\0';
    } else {
        reg[2] = '\0';
    }
    return (findRegist(reg) << 8);      // Procura o registrador na hashmap e retorna seu codigo deslocado 8 bits a esquerda
}

/* Identifica e retorna o segundo registrador da instrucao */
int getR2(char instruction[]) {
    instruction = strchr(instruction, ',') + 1; // Ignora tudo antes da virgula
    instruction = leftClean(instruction);       // Limpaos os espacos a esquerda da string
    char reg[100];
    reg[0] = instruction[0];        // Pega os dois primeiros caracteres da string e, se precisar, o terceiro
    reg[1] = instruction[1];
    if (instruction[2] >= '0' && instruction[2] <= '9') {
        reg[2] = instruction[2];
        reg[3] = '\0';
    } else {
        reg[2] = '\0';
    }
    return (findRegist(reg) << 4);      // Procura o registrador na hashmap e retorna seu codigo deslocado 4 bits a esquerda
}

/* Identifica e retorna o terceiro registrador na instrucao */
int getR3(char instruction[]) {
    instruction = strchr(instruction, ',') + 1;
    instruction = strchr(instruction, ',') + 1; // Ignora tudo antes da segunda virgula
    instruction = leftClean(instruction);       // Limpaos os espacos a esquerda da string
    char reg[100];
    reg[0] = instruction[0];        // Pega os dois primeiros caracteres da string e, se precisar, o terceiro
    reg[1] = instruction[1];
    if (instruction[2] >= '0' && instruction[2] <= '9') {
        reg[2] = instruction[2];
        reg[3] = '\0';
    } else {
        reg[2] = '\0';
    }
    return findRegist(reg);     // Procura o registrador na hashmap e retorna seu codigo
}

/* Identifica e retorna a constante na instrucao */
int getConst(char instruction[]) {
    instruction = strchr(instruction, ',') + 1; // Ignora tudo antes da virgula
    instruction = leftClean(instruction);       // Limpaos os espacos a esquerda da string

    char constante[100];
    int i = 0;
    do {
        constante[i] = instruction[i];      // Pega a primeira palavra da string
        i++;
    } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
    constante[i] = '\0';
    if (constante[0] <= '9' && constante[0] >= '0') {       // Se for um numero
        return atoi(constante);                             // Transforma em inteiro e retorna
    } else if (!strcmp(constante, "LOWBYTE")) {             // Se for "LOWBYTE
        return (getDestination(instruction) & 0x0000ffff);  // Pega o byte menos significativo do codigo do label a seguir
    } else if (!strcmp(constante, "HIGHBYTE")) {            // Se for "HIGHBYTE"
        return (getDestination(instruction) >> 16);         // Pega o byte mais significativo do codigo do label a seguir
    } else {                                                // Se for somente um label
        return (findLabel(constante) & 0x0000ffff);         // Pega o byte menos significativo do codigo do label
    }
}

/* Identifica e retorna condicao na instrucao */
int getCondicao(char instruction[]) {
    instruction = strchr(instruction, '.') + 1; // Ignora tudo antes do ponto
    char condicao[100];
    int i = 0;
    while(instruction[i] != ' ') {
        condicao[i] = instruction[i];       // Pega a primeira palavra da string
        i++;
    }
    condicao[i] = '\0';

    if (!strcmp(condicao, "overflow"))      // Verifica qual flag esta na condicao e retorna seu respectivo codigo deslocado 16 bits a esquerda
        return (1 << 16);
    else if (!strcmp(condicao, "zero"))
        return (2 << 16);
    else if (!strcmp(condicao, "neg"))
        return (3 << 16);
    else if (!strcmp(condicao, "negzero"))
        return (4 << 16);
    else if (!strcmp(condicao, "true"))
        return (5 << 16);
    else if (!strcmp(condicao, "carry"))
        return (6 << 16);
    else
        return 0;
}

/* Identifica e retorna o destino na instrucao */
int getDestination(char instruction[]) {
    instruction = strchr(instruction, ' ') + 1; // Ignora tudo antes do primeiro espaco
    instruction = leftClean(instruction);       // Limpaos os espacos a esquerda da string

    char label[100];
    int i = 0;
    do {
        label[i] = instruction[i];      // Pega a primeira palavra da string
        i++;
    } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
    label[i] = '\0';

    return findLabel(label);        // Procura o label na hashmap e retorna seu respectivo codigo
}

/* Traduz a instrucao e retorna o codigo binario resultante */
int traduzir(char instruction[]) {
    int result = 0;
    char *mnemonic = getMnemonic(instruction);     // Pega o mnemonico da instrucao
    if (!strcmp(mnemonic, "add")) {     // Identifica o mnemonico e acopla o codigo binario de cada uma das partes da instrucao
        result = (0x00000 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "addu")) {
        result = (0x00001 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "addinc")) {
        result = (0x00002 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "inca")) {
        result = (0x00003 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "sub")) {
        result = (0x00004 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "subdec")) {
        result = (0x00005 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "deca")) {
        result = (0x00006 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "mult")) {
        result = (0x00007 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "multu")) {
        result = (0x00008 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "mfh")) {
        result = (0x00009 << 12) + getR1(instruction);
    } else if (!strcmp(mnemonic, "mfl")) {
        result = (0x0000A << 12) + getR1(instruction);
    } else if (!strcmp(mnemonic, "div")) {
        result = (0x0000B << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "divu")) {
        result = (0x0000C << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "asl")) {
        result = (0x0000D << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "asr")) {
        result = (0x0000E << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "zeros")) {
        result = (0x0000F << 12) + getR1(instruction);
    } else if (!strcmp(mnemonic, "ones")) {
        result = (0x00010 << 12) + getR1(instruction);
    } else if (!strcmp(mnemonic, "passa")) {
        result = (0x00011 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "passnota")) {
        result = (0x00012 << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "and")) {
        result = (0x00013 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "andnota")) {
        result = (0x00014 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "nand")) {
        result = (0x00015 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "or")) {
        result = (0x00016 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "ornota")) {
        result = (0x00017 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "nor")) {
        result = (0x00018 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "xor")) {
        result = (0x00019 << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "xornota")) {
        result = (0x0001A << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "xnor")) {
        result = (0x0001B << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "lsl")) {
        result = (0x0001C << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "lsr")) {
        result = (0x0001D << 12) + getR1(instruction) + getR2(instruction);
    } else if (!strcmp(mnemonic, "slt")) {
        result = (0x0001E << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "sltu")) {
        result = (0x0001F << 12) + getR1(instruction) + getR2(instruction) + getR3(instruction);
    } else if (!strcmp(mnemonic, "loadlit")) {
        result = (0x400 << 20) + (getR1(instruction) << 8) + getConst(instruction);
    } else if (!strcmp(mnemonic, "lcl")) {
        result = (0x401 << 20) + (getR1(instruction) << 8) + getConst(instruction);
    } else if (!strcmp(mnemonic, "lch")) {
        result = (0x402 << 20) + (getR1(instruction) << 8) + getConst(instruction);
    } else if (!strcmp(mnemonic, "load")) {
        result = (0x800000 << 8) + (getR1(instruction) >> 4) + (getR2(instruction) >> 4);
    } else if (!strcmp(mnemonic, "store")) {
        result = (0x800001 << 8) + (getR1(instruction) >> 4) + (getR2(instruction) >> 4);
    } else if (!strcmp(mnemonic, "j")) {
        result = (0xC00 << 20) + getDestination(instruction);
    } else if (!strcmp(mnemonic, "jt")) {
        result = (0xC01 << 20) + getCondicao(instruction) + getDestination(instruction);
    } else if (!strcmp(mnemonic, "jf")) {
        result = (0xC02 << 20) + getCondicao(instruction) + getDestination(instruction);
    } else if (!strcmp(mnemonic, "jal")) {
        result = (0xC03 << 20) + (getR1(instruction) << 4);
    } else if (!strcmp(mnemonic, "jr")) {
        result = (0xC04 << 20) + (getR1(instruction) << 4);
    } else if (!strcmp(mnemonic, "nop")) {
        result = 0x00011000;      //passa r0, r0
    } else {
        printf("ERRO: instrucao inexistente - %s\n", instruction);
    }
    return result;
}

/* Identifica o numero de instrucoes do arquivo de entrada */
void setNumInstructions() {
    char *instruction;
    char line[300];
    bool pseg = false;
    bool dseg = false;
    bool module = false;
    FILE *fileR = fopen(fileName, "r");     // Abre o arquivo de leitura

    if (fileR != NULL) {
        while(fgets(line, sizeof(line), fileR) != NULL) {       // Le cada linha do arquivo
            instruction = removeLast(line);                     // Remove o '\n' da string
            instruction = ignoreLabelsAndComents(instruction);    // Retira o label e o comentario da string, se tiver

            if (instruction != NULL && instruction[0] == '.') {     // Se a linha nao tiver vazia (era somente espacos e comentario) e for uma diretiva
                char directive[100];
                int i = 0;
                do {
                    directive[i] = instruction[i];      // Salva a primeira palavra da string na variavel "deretiva"
                    i++;
                } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
                directive[i] = '\0';
                if (!strcmp(directive, ".module")) {    // Verifica qual foi a diretiva lida
                    module = true;                      // Ativa o module (programa comecou)
                } else if (!strcmp(directive, ".end")) {
                    module = false;                     // Desativa o module (programa terminou)
                } else if (!strcmp(directive, ".pseg")) {
                    pseg = true;                        // Ativa o pseg (trecho de instrucoes comecou)
                    dseg = false;                       // Desativa o deseg (trecho de dados terminou)
                } else if (!strcmp(directive, ".dseg")) {
                    pseg = false;                       // Desativa o pseg (trecho de instrucoes terminou)
                    dseg = true;                        // Ativa o deseg (trecho de dados comecou)
                }
            } else if (instruction != NULL && instruction[0] != '.' && module && pseg) {     // Se a linha for uma instrucao
                numInstructions++;
            }
        }
    }
}

/* Identifica e escreve todos os ".word" no arquivo de saida */
void readWords(FILE *fileW) {
    char *instruction;
    char line[300];
    bool pseg = false;
    bool dseg = false;
    bool module = false;
    FILE *fileR = fopen(fileName, "r");     // Abre o arquivo de leitura

    if (fileR != NULL) {
        while(fgets(line, sizeof(line), fileR) != NULL) {       // Le cada linha do arquivo
            instruction = removeLast(line);                     // Remove o '\n' da string
            instruction = ignoreLabelsAndComents(instruction);    // Retira o label e o comentario da string, se tiver

            if (instruction != NULL && instruction[0] == '.') {     // Se a linha nao tiver vazia (era somente espacos e comentario) e for uma diretiva
                char directive[100];
                int i = 0;
                do {
                    directive[i] = instruction[i];      // Salva a primeira palavra da string na variavel "deretiva"
                    i++;
                } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
                directive[i] = '\0';
                if (!strcmp(directive, ".module")) {    // Verifica qual foi a diretiva lida
                    module = true;                      // Ativa o module (programa comecou)
                } else if (!strcmp(directive, ".end")) {
                    module = false;                     // Desativa o module (programa terminou)
                } else if (!strcmp(directive, ".pseg")) {
                    pseg = true;                        // Ativa o pseg (trecho de instrucoes comecou)
                    dseg = false;                       // Desativa o deseg (trecho de dados terminou)
                } else if (!strcmp(directive, ".dseg")) {
                    pseg = false;                       // Desativa o pseg (trecho de instrucoes terminou)
                    dseg = true;                        // Ativa o deseg (trecho de dados comecou)
                } else if (!strcmp(directive, ".word")) {
                    if (dseg && module) {       // Se tiver em um trecho de dados dentro do module
                        instruction += 5;       // Tira o ".word" da string
                        instruction = leftClean(instruction);       // Tira os espacos e tabs a esquerda da string
                        char word[100];
                        int i = 0;
                        do {
                            word[i] = instruction[i];       // Pega a primeira palavra de string
                            i++;
                        } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
                        word[i] = '\0';

                        writeOnFile(fileW, atoi(word));      // Escreve no arquivo de saida o codigo binario da palavra obtida
                    }
                }
            }
        }
    }
}

/* Traduz a instrucao e retorna o codigo binario resultante */
void writeOnFile(FILE *file, int bin) {
    int i;
    for (i = 0; i < 32; i++) {      // Para cada bit da palavra
        if (bin >= 2147483648)      // Verifica se o bit e 1 ou 0 e esqueve no arquivo
            fprintf(file, "%c", '1');
        else
            fprintf(file, "%c", '0');
        bin = (bin << 1);           // Desloca bits para esquerda (pega o proximo)
    }
    //fprintf(file, "\n");
}

/* Algoritmo principal do programa */
int main(int argc, char *argv[]) {
    if (argc > 1)               // Se o usuario tiver passado algum parametro
        fileName = argv[1];     // Define o nome do arquivo de entrada como o parametro recebido
    else
        fileName = "teste.asm"; // Nome default do arquivo de entrada

    setNumInstructions();       // Escreve no arquivo de saida o codigo binario da instrucao
    createRegisters();          // Cria e salva os registradores na hashmap
    createLabels(fileName);     // Identifica os labels e seus respectivos enderecos e salva-os na hashmap

    char *instruction;
    char line[300];
	FILE *fileR = fopen(fileName, "r");           // Abre o arquivo de leitura
	FILE *fileW = fopen("resultado montador.bin", "w");    // Abre o arquivo de escrita

    if (fileR == NULL) {
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	} else {
        bool pseg = false;
        bool dseg = false;
        bool module = false;

	    while(fgets(line, sizeof(line), fileR) != NULL) {       // Le cada linha do arquivo
            instruction = removeLast(line);                     // Remove o '\n' da string
	        instruction = ignoreLabelsAndComents(instruction);    // Retira o label e o comentario da string, se tiver

            if (instruction != NULL && instruction[0] == '.') {     // Se a linha nao tiver vazia (era somente espacos e comentario) e for uma diretiva
                char directive[100];
                int i = 0;
                do {
                    directive[i] = instruction[i];      // Salva a primeira palavra da string na variavel "deretiva"
                    i++;
                } while(instruction[i] != ' ' && instruction[i] != '\0' && instruction[i] != '\t');
                directive[i] = '\0';
                if (!strcmp(directive, ".module")) {    // Verifica qual foi a diretiva lida
                    module = true;                      // Ativa o module (programa comecou)
                } else if (!strcmp(directive, ".end")) {
                    module = false;                     // Desativa o module (programa terminou)
                } else if (!strcmp(directive, ".pseg")) {
                    pseg = true;                        // Ativa o pseg (trecho de instrucoes comecou)
                    dseg = false;                       // Desativa o deseg (trecho de dados terminou)
                } else if (!strcmp(directive, ".dseg")) {
                    pseg = false;                       // Desativa o pseg (trecho de instrucoes terminou)
                    dseg = true;                        // Ativa o deseg (trecho de dados comecou)
                }
            } else if (instruction != NULL && module && pseg) {     // Se a linha for uma instrucao
                writeOnFile(fileW, traduzir(instruction));       // Escreve no arquivo de saida o codigo binario da instrucao
            }
	    }
	    readWords(fileW);
    }
    fclose(fileR);      // Fecha o arquivo de entrada
    fclose(fileW);      // Fecha o arquivo de saida

    return 0;
}
