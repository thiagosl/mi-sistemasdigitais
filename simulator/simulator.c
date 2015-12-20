#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int gpr[16];    //Registradores de propósito geral (GPR)
int flags_reg[7];   //Registrador de flags 1 - overflow
int mem[200];    //Memória de dados
unsigned int pc = 0x00000000;    //Ponteiro para a próxima instrução que será executada
unsigned int ir;     //Registrador de propósito específico que armazena a instrução que está sendo executada neste ciclo
unsigned int mem_lenth = 0;   //Tamanho da memória utilisada pelo programa
char *file_name;    //Nome do arquivo de leitura
bool halt = false;

bool check_and_set_carry_overflow(int value_0, int value_1, bool unsigned_operation)
{
    flags_reg[1] = false;
    flags_reg[6] = false;
    //if (unsigned_operation)     //Operações sem sinal
    //{
    unsigned int u_v0 = value_0;
    unsigned int u_v1 = value_1;
    unsigned int max = 0xFFFFFFFF;      //Valor inteiro sem sinal máximo
    if (u_v1 > (max - u_v0)) //Se o valor1 for maior do que o número máximo decrescido do valor0, acontecerá um carry
    {
        flags_reg[6] = true;   //CARRY
        if (unsigned_operation)
            return true;
    }
    //}
    if (!unsigned_operation)        //Operações com sinal
    {
        int max = 0x7FFFFFFF;       //Valor máximo para inteiros com sinal
        int min = 0x80000000;       //Valor mínimo para inteiros com sinal
        if ((value_0 > 0) && (value_1 > 0))     //Se ambos os valores forem positivos
        {
            if (value_1 > (max - value_0))     //Se for uma operação de soma e ocorrer overflow
            {
                flags_reg[1] = true;   //OVERFLOW
                return true;
            }
        }
        else if ((value_0 < 0) && (value_1 < 0))    //Se ambos os valores forem negativos
        {
            if (value_1 < (min - value_0))     //Se for uma operação de soma e ocorrer overflow
            {
                flags_reg[1] = true;   //OVERFLOW
                return true;
            }
        }
        return false;
    }
}

void check_and_set_neg_zero_true(int value)
{
    if (value == 0)    //Se o valor for zero
    {
        flags_reg[2] = true;   //ZERO
        flags_reg[3] = false;   //NEG
        flags_reg[4] = true;   //NEGZERO
        flags_reg[5] = false;   //TRUE
    }
    if (value < 0)     //Se o valor for menor que zero
    {
        flags_reg[2] = false;   //ZERO
        flags_reg[3] = true;   //NEG
        flags_reg[4] = true;   //NEGZERO
        flags_reg[5] = true;   //TRUE
    }
    if (value > 0)     //Se o valor for maior que zero
    {
        flags_reg[2] = false;   //ZERO
        flags_reg[3] = false;   //NEG
        flags_reg[4] = false;   //NEGZERO
        flags_reg[5] = true;   //TRUE
    }
}

void reset_neg_zero_true()
{
    flags_reg[2] = false;   //ZERO
    flags_reg[3] = false;   //NEG
    flags_reg[4] = false;   //NEGZERO
    flags_reg[5] = false;   //TRUE
}

bool get_flag(int flag)
{
    return flags_reg[flag];
}

void ula_add(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false);      //Verifica a ocorrencia da flag overflow
    printf("ADD: %d <-- %d\n", reg1, gpr[reg2] + gpr[reg3]);
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu overflow, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_addu(int reg1, int reg2, int reg3)
{
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], true);    //Verifica a ocorrencia da flag carry
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    printf("ADDU: %d <-- %u\n", reg1, value_0 + value_1);
    gpr[reg1] =  value_0 + value_1;
    if (!carry)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_addinc(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false);      //Verifica a ocorrencia da flag overflow
    printf("ADDINC: %d <-- %d\n", reg1, gpr[reg2] + gpr[reg3] + 1);
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    if (!overflow)
    {
        gpr[reg1] = gpr[reg1] + 1;
        overflow = check_and_set_carry_overflow(gpr[reg1], 1, false);      //Verifica a ocorrencia da flag overflow
        if (!overflow)
        {
            check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
        }
        else
        {
            reset_neg_zero_true();
        }
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_inca(int reg1, int reg2)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], 1, false);     //Verifica a ocorrencia da flag overflow
    printf("INCA: %d <-- %d\n", reg1, gpr[reg2] + 1);
    gpr[reg1] = 1 + gpr[reg2];
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_sub(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], -gpr[reg3], false);       //Verifica a ocorrencia da flag overflow
    printf("SUB: %d <-- %d\n", reg1, gpr[reg2] - gpr[reg3]);
    gpr[reg1] = gpr[reg2] - gpr[reg3];
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_subdec(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], -gpr[reg3], false);       //Verifica a ocorrencia da flag overflow
    int aux = gpr[reg2];
    int aux2 = gpr[reg3];
    printf("SUBDEC: %d <-- %d\n", reg1, gpr[reg2] - gpr[reg3] - 1);
    gpr[reg1] = gpr[reg2] - gpr[reg3] - 1;
    if (!overflow)
    {
        overflow = check_and_set_carry_overflow(aux - aux2, -1, false);       //Verifica a ocorrencia da flag overflow
        if (!overflow)
        {
            check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
        }
        else
        {
            reset_neg_zero_true();
        }
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_deca(int reg1, int reg2)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], -1, false);      //Verifica a ocorrencia da flag overflow
    printf("DECA: %d <-- %d\n", reg1, gpr[reg2] - 1);
    gpr[reg1] = gpr[reg2] - 1;
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_subu(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], -gpr[reg3], true);       //Verifica a ocorrencia da flag overflow
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    printf("SUBU: %d <-- %u\n", reg1, value_0 - value_1);
    gpr[reg1] = value_0 - value_1;
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_addincu(int reg1, int reg2, int reg3)
{
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], true);      //Verifica a ocorrencia da flag overflow
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    printf("ADDINCU: %d <-- %u\n", reg1, value_0 + value_1 + 1);
    gpr[reg1] = value_0 + value_1;
    if (!overflow)
    {
        gpr[reg1] = (unsigned int) gpr[reg1] + (unsigned int) 1;
        overflow = check_and_set_carry_overflow(gpr[reg1], 1, true);      //Verifica a ocorrencia da flag overflow
        if (!overflow)
        {
            check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
        }
        else
        {
            reset_neg_zero_true();
        }
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_passb(int reg1, int reg2)
{
    printf("PASSB: %d <-- %d\n", reg1, gpr[reg2]);
    gpr[reg1] = gpr[reg2];
    //check_and_set_carry_overflow(0, 0, false);
    //check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_passnotb(int reg1, int reg2)
{
    printf("PASSNOTB: %d <-- %d\n", reg1, gpr[reg2]);
    gpr[reg1] = ~gpr[reg2];
    //check_and_set_carry_overflow(0, 0, false);
    //check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_div(int reg1, int reg2, int reg3)
{
    printf("DIV: %d <-- %d\n", reg1, gpr[reg2] / gpr[reg3]);
    gpr[reg1] = gpr[reg2] / gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_divu(int reg1, int reg2, int reg3)
{
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    printf("DIVU: %d <-- %u\n", reg1, value_0 / value_1);
    gpr[reg1] = value_0 / value_1;
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_asl(int reg1, int reg2)
{
    printf("ASL: %d <-- %d\n", reg1, gpr[reg2] << 1);
    int aux = gpr[reg2];
    gpr[reg1] = gpr[reg2] << 1;
    flags_reg[1] = ((aux >> 31) & 0x00000001) ^ ((aux >> 30) & 0x00000001);    //Overflow
    flags_reg[6] = (aux >> 31) & 0x00000001; //carry
    if (!flags_reg[1] && !flags_reg[6])
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_asr(int reg1, int reg2)
{
    printf("ASR: %d <-- %d\n", reg1, gpr[reg2] >> 1);
    gpr[reg1] = gpr[reg2] >> 1;
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_zeros(int reg1)
{
    printf("ZEROS: %d <-- %d\n", reg1, 0);
    gpr[reg1] = 0;
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_ones(int reg1)
{
    printf("ONES: %d <-- %d\n", reg1, 1);
    gpr[reg1] = 1;
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_passa(int reg1, int reg2)
{
    if (reg1 != reg2)
    {
        printf("PASSA: %d <-- %d\n", reg1, gpr[reg2]);
        gpr[reg1] = gpr[reg2];
        check_and_set_carry_overflow(0, 0, false);
        check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
    }
    else
    {
        printf("NOP\n");
    }
}

void ula_passnota(int reg1, int reg2)
{
    printf("PASSNOTA: %d <-- %d\n", reg1, ~gpr[reg2]);
    gpr[reg1] = ~gpr[reg2];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_and(int reg1, int reg2, int reg3)
{
    printf("AND: %d <-- %d\n", reg1, gpr[reg2] & gpr[reg3]);
    gpr[reg1] = gpr[reg2] & gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_andnota(int reg1, int reg2, int reg3)
{
    printf("ANDNOTA: %d <-- %d\n", reg1, (~gpr[reg2]) & gpr[reg3]);
    gpr[reg1] = (~gpr[reg2]) & gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_nand(int reg1, int reg2, int reg3)
{
    printf("NAND: %d <-- %d\n", reg1, ~(gpr[reg2] & gpr[reg3]));
    gpr[reg1] = ~(gpr[reg2] & gpr[reg3]);
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_or(int reg1, int reg2, int reg3)
{
    printf("OR: %d <-- %d\n", reg1, gpr[reg2] | gpr[reg3]);
    gpr[reg1] = gpr[reg2] | gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_ornota(int reg1, int reg2, int reg3)
{
    printf("ORNOTA: %d <-- %d\n", reg1, (~gpr[reg2]) | gpr[reg3]);
    gpr[reg1] = (~gpr[reg2]) | gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_nor(int reg1, int reg2, int reg3)
{
    printf("NOR: %d <-- %d\n", reg1, ~(gpr[reg2] | gpr[reg3]));
    gpr[reg1] = ~(gpr[reg2] | gpr[reg3]);
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xor(int reg1, int reg2, int reg3)
{
    printf("XOR: %d <-- %d\n", reg1, gpr[reg2] ^ gpr[reg3]);
    gpr[reg1] = gpr[reg2] ^ gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xornota(int reg1, int reg2, int reg3)
{
    printf("XORNOTA: %d <-- %d\n", reg1, (~gpr[reg2]) ^ gpr[reg3]);
    gpr[reg1] = (~gpr[reg2]) ^ gpr[reg3];
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xnor(int reg1, int reg2, int reg3)
{
    printf("XNOR: %d <-- %d\n", reg1, ~(gpr[reg2] ^ gpr[reg3]));
    gpr[reg1] = ~(gpr[reg2] ^ gpr[reg3]);
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_lsl(int reg1, int reg2)
{
    printf("LSL: %d <-- %d\n", reg1, gpr[reg2] << 1);
    int aux = gpr[reg2];
    gpr[reg1] = gpr[reg2] << 1;
    flags_reg[1] = 0;   //Overflow
    flags_reg[6] = (aux >> 31) & 0x00000001; //Carry
    if (!flags_reg[6])
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_lsr(int reg1, int reg2)
{
    printf("LSR: %d <-- %d\n", reg1, (gpr[reg2] >> 1) & 0x7FFFFFFF);
    gpr[reg1] = (gpr[reg2] >> 1) & 0x7FFFFFFF;
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_slt(int reg1, int reg2, int reg3)
{
    if (gpr[reg2] < gpr[reg3])
    {
        gpr[reg1] = 1;
    }
    else
    {
        gpr[reg1] = 0;
    }
    printf("SLT: %d <-- %d\n", reg1, gpr[reg1]);
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_sltu(int reg1, int reg2, int reg3)
{
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    if (value_0 < value_1)
    {
        gpr[reg1] = 1;
    }
    else
    {
        gpr[reg1] = 0;
    }
    printf("SLT: %d <-- %d\n", reg1, gpr[reg1]);
    check_and_set_carry_overflow(0, 0, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void const_loadlit(int reg1, int cst)
{
    if (cst >= 32768)
        cst = 0xFFFF0000 | cst;     //Extensor de sinal
    printf("LOADLIT: %d <-- %d\n", reg1, cst);
    gpr[reg1] = cst;
}

void const_lcl(int reg1, int cst)
{
    printf("LCL: %d <-- %d\n", reg1, cst | (gpr[reg1] & 0xFFFF0000));
    gpr[reg1] = cst | (gpr[reg1] & 0xFFFF0000);     //Pega os 16 bits mais significativos do valor no registrador e faz um OR com a constante
}

void const_lch(int reg1, int cst)
{
    printf("LCH: %d <-- %d\n", reg1, (cst << 16) | (gpr[reg1] & 0x0000FFFF));
    gpr[reg1] = (cst << 16) | (gpr[reg1] & 0x0000FFFF);      //Pega os 16 bits menos significativos do valor no registrador e faz um OR com a constante deslocada 8 bits a esquerda
}

void mem_load(int reg1, int addr)
{
    printf("LOAD: %d <-- %d\n", reg1, mem[gpr[addr]]);
    gpr[reg1] = mem[gpr[addr]];     //Pega endereço no registrador, lê o valor neste endereço de memória e grava no registrador
}

void mem_store(int addr, int reg1)
{
    printf("STORE: %d <-- %d\n", gpr[addr], gpr[reg1]);
    //printf("endereço de mem = %d, conteudo da mem = %d, vai ser gravado = %d\n", gpr[addr], mem[gpr[addr]], gpr[reg1]);
    mem[gpr[addr]] = gpr[reg1];     //Grava no endereço de memória o valor no registrador
    if (gpr[addr] > mem_lenth)
        mem_lenth = gpr[addr];
}

void jump(int addr)
{
    printf("JUMP: pc <-- %d\n", addr);
    if ((pc - 1) == addr) {
        halt = true;
    }
    pc = addr;
}

void jump_true(int flag, int addr)
{
    printf("JT: pc <-- %d ; flag: %d\n", addr, flag);
    bool is_true = get_flag(flag);
    if (is_true)
    {
        pc = addr;
    }
    if (pc == addr) {
        //system("pause");
    }
}

void jump_false(int flag, int addr)
{
    printf("JF: pc <-- %d ; flag: %d\n", addr, flag);
    bool is_true = get_flag(flag);
    if (!is_true)
    {
        pc = addr;
    }
}

void jump_and_link(int reg1)
{
    printf("JAL: pc <-- %d ; 7 <-- pc \n", gpr[reg1], reg1);
    gpr[7] = pc;
    pc = gpr[reg1];
}

void jump_register(int reg1)
{
    printf("JUMREGISTER: pc <-- %d\n", gpr[reg1]);
    pc = gpr[reg1];
}

void decode_and_run_instruction()
{
    unsigned int layout = ir >> 30; //Desloca 30 bits
    if (layout == 0)   //Layout de operações lógico-aritméticas é definido como 00
    {
        int opcode = (ir >> 12) & 0x0000003F; //Desloca 12 bits e pega os 6 menos significativos
        int reg3 = ir & 0x0000000F;     //Pega os 4 bits menos significativos
        int reg2 = (ir >> 4) & 0x0000000F;     //Desloca 4 bits e pega os 4 menos significativos
        int reg1 = (ir >> 8) & 0x0000000F;     //Desloca 8 bits e pega os 4 menos significativos
        if (opcode == 0x00000000)
        {
            ula_add(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000001)
        {
            ula_addu(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000002)
        {
            ula_addinc(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000003)
        {
            ula_inca(reg1, reg2);
        }
        else if (opcode == 0x00000004)
        {
            ula_sub(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000005)
        {
            ula_subdec(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000006)
        {
            ula_deca(reg1, reg2);
        }
        else if (opcode == 0x00000007)
        {
            ula_subu(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000008)
        {
            ula_addincu(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000009)
        {
            ula_passb(reg1, reg2);
        }
        else if (opcode == 0x0000000A)
        {
            ula_passnotb(reg1, reg2);
        }
        else if (opcode == 0x0000000B)
        {
            ula_div(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000000C)
        {
            ula_divu(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000000D)
        {
            ula_asl(reg1, reg2);
        }
        else if (opcode == 0x0000000E)
        {
            ula_asr(reg1, reg2);
        }
        else if (opcode == 0x0000000F)
        {
            ula_zeros(reg1);
        }
        else if (opcode == 0x00000010)
        {
            ula_ones(reg1);
        }
        else if (opcode == 0x00000011)
        {
            ula_passa(reg1, reg2);
        }
        else if (opcode == 0x00000012)
        {
            ula_passnota(reg1, reg2);
        }
        else if (opcode == 0x00000013)
        {
            ula_and(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000014)
        {
            ula_andnota(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000015)
        {
            ula_nand(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000016)
        {
            ula_or(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000017)
        {
            ula_ornota(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000018)
        {
            ula_nor(reg1, reg2, reg3);
        }
        else if (opcode == 0x00000019)
        {
            ula_xor(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000001A)
        {
            ula_xornota(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000001B)
        {
            ula_xnor(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000001C)
        {
            ula_lsl(reg1, reg2);
        }
        else if (opcode == 0x0000001D)
        {
            ula_lsr(reg1, reg2);
        }
        else if (opcode == 0x0000001E)
        {
            ula_slt(reg1, reg2, reg3);
        }
        else if (opcode == 0x0000001F)
        {
            ula_sltu(reg1, reg2, reg3);
        }
    }
    else if (layout == 1)  //Layout de operações com constantes é definido como 01
    {
        int opcode = (ir >> 20) & 0x00000003; //Desloca 20 bits e pega os 2 menos significativos
        int cst = ir & 0x0000FFFF;      //Pega os 16 bits menos significativos
        int reg1 = (ir >> 16) & 0x0000000F;     //Desloca 16 bits e pega os 4 menos significativos
        if (opcode == 0x00000000)
        {
            const_loadlit(reg1, cst);
        }
        else if (opcode == 0x00000001)
        {
            const_lcl(reg1, cst);
        }
        else if (opcode == 0x00000002)
        {
            const_lch(reg1, cst);
        }
    }
    else if (layout == 2)  //Layout de operaões de acesso a memória é definida como 10
    {
        int opcode = (ir >> 8) & 0x00000003; //Desloca 8 bits e pega os 2 menos significativos
        int reg2 = ir & 0x0000000F;     //Pega os 4 bits menos significativos
        int reg1 = (ir >> 4) & 0x0000000F;     //Desloca 4 bits e pega os 4 menos significativos
        if (opcode == 0x00000000)
        {
            mem_load(reg1, reg2);
        }
        else if (opcode == 0x00000001)
        {
            mem_store(reg1, reg2);
        }
    }
    else if (layout == 3)  //Layout de operações de desvios e saltos é definido como 11
    {
        int opcode = (ir >> 20) & 0x0000000F; //Desloca 20 bits e pega 4 menos significativos
        int addr = ir & 0x00000FFF;     //Pega os 12 bits menos significativos
        int reg1 = (ir >> 12) & 0x0000000F;     //Desloca 12 bits e pega os 4 menos significativos
        int cond = (ir >> 16) & 0x0000000F;     //Desloca 16 bits e pega os 4 menos significativos
        if (opcode == 0x00000000)
        {
            jump(addr);
        }
        else if (opcode == 0x00000001)
        {
            jump_true(cond, addr);
        }
        else if (opcode == 0x00000002)
        {
            jump_false(cond, addr);
        }
        else if (opcode == 0x00000003)
        {
            jump_and_link(reg1);
        }
        else if (opcode == 0x00000004)
        {
            jump_register(reg1);
        }
    }
}

void writeResult()
{
    FILE *file_w = fopen("result.txt", "w");    // Abre o arquivo de escrita

    int i;
    for (i = 0; i < 16; i++)      //Percorre os 16 registradores
    {
        fprintf(file_w, "gpr%d: %d\n", i, gpr[i]);    //Imprime cada um no arquivo de saida
    }
    for (i = 0; i < mem_lenth; i++)     //Percorre a memoria utilezada
    {
        fprintf(file_w, "mem(%d): %d\n", i, mem[i]);  //Imprime cada valor no arquivo de saida
    }
    fclose(file_w);
}

int main(int argc, char *argv[])
{
    if (argc > 1)                // Se o usuario tiver passado algum parametro
        file_name = argv[1];     // Define o nome do arquivo de entrada como o parametro recebido
    else
        file_name = "result.bin"; // Nome default do arquivo de entrada

    FILE *file_r = fopen(file_name, "r");      // Abre o arquivo de leitura
    if (file_r == NULL)
    {
        printf("Erro, nao foi possivel abrir o arquivo\n");
        return -1;
    }
    else
    {
        char c = fgetc(file_r);      //Le um caractere do arquivo
        while(!feof(file_r))         //Verifica se chegou no fim do arquivo
        {
            int instruction = 0;
            int i;
            for(i = 0; i < 32; i++)     //Percorre os 32 bits da palavra
            {
                printf("%c",c);
                if (c == '1')       //Se o bit for '1'
                {
                    instruction++;
                }
                if (i != 31)
                {
                    instruction = (instruction << 1);     //Desloca um bit para a esquerda
                }
                c = fgetc(file_r);     //Le um caractere do arquivo
            }
            mem[mem_lenth] = instruction;   //Salva a palavra na memoria
            mem_lenth++;
            printf("\n");
        }
        while (true)
        {
            ir = mem[pc];                        //Transfere para o registrador IR a instrução que será executada
            printf("IR=%d <--> PC=%d \n", ir, pc);
            pc++;                               //Incrementa o ponteiro de instruções
            decode_and_run_instruction();       //Decodifica e executa a instrução
            writeResult();
            if (halt)
                system("pause");
        }
    }

    return 1;
}
