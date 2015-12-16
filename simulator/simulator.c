#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int gpr[16];    //Registradores de propósito geral (GPR)
int flags_reg[7];   //Registrador de flags 1 - overflow
int mem[200] = {0x1345AF2D};    //Memória de dados
int pc = 0x00000000;    //Ponteiro para a próxima instrução que será executada
int ir;     //Registrador de propósito específico que armazena a instrução que está sendo executada neste ciclo
int hi;     //Registrador de propósito específico que armazena os bytes mais significativos da operação de multiplicação
int lo;     //Registrador de propósito específico que armazena os bytes menos significativos da operação de multiplicação
int mem_lenth = 0;   //Tamanho da memória utilisada pelo programa

bool check_and_set_carry_overflow(int v0, int v1, bool unsigned_operation, bool subs_operation)
{
    if (unsigned_operation)     //Operações sem sinal
    {
        unsigned int value_0 = (unsigned int) v0;
        unsigned int value_1 = (unsigned int) v1;
        unsigned int max = 0xFFFFFFFF;      //Valor inteiro sem sinal máximo
        if (!subs_operation && (value_1 > (max - value_0))) //Se o valor1 for maior do que o número máximo decrescido do valor0, acontecerá um carry
        {
            flags_reg[0x00000001] = false;
            flags_reg[0x00000006] = true;   //CARRY
            return true;
        }
        else
        {
            flags_reg[0x00000001] = false;
            flags_reg[0x00000006] = false;
            return false;
        }
    }
    else        //Operações com sinal
    {
        int value_0 = v0;
        int value_1 = v1;
        int max = 0x7FFFFFFF;       //Valor máximo para inteiros com sinal
        int min = 0x80000000;       //Valor mínimo para inteiros com sinal
        if ((value_0 > 0) && (value_1 > 0))     //Se ambos os valores forem positivos
        {
            if (!subs_operation && (value_1 > (max - value_0)))     //Se for uma operação de soma e ocorrer overflow
            {
                flags_reg[0x00000001] = true;   //OVERFLOW
                flags_reg[0x00000006] = false;
                return true;
            }
            else        //Se for uma operação de subtração ou não ocorrer overflow
            {
                flags_reg[0x00000001] = false;
                flags_reg[0x00000006] = false;
                return false;
            }
        }
        else if ((value_0 < 0) && (value_1 < 0))    //Se ambos os valores forem negativos
        {
            if (!subs_operation && (value_1 < (min - value_0)))     //Se for uma operação de soma e ocorrer overflow
            {
                flags_reg[0x00000001] = true;   //OVERFLOW
                flags_reg[0x00000006] = false;
                return true;
            }
            else        //Se for uma operação de subtração ou não ocorrer overflow
            {
                flags_reg[0x00000001] = false;
                flags_reg[0x00000006] = false;
                return false;
            }
        }
        else        //Se um dos números for positivo e o outro negativo
        {
            flags_reg[0x00000001] = false;
            flags_reg[0x00000006] = false;
            return false;
        }
    }
}

void check_and_set_neg_zero_true(int value)
{
    if (value == 0x00000000)    //Se o valor for zero
    {
        flags_reg[0x00000002] = true;   //ZERO
        flags_reg[0x00000003] = false;   //NEG
        flags_reg[0x00000004] = true;   //NEGZERO
        flags_reg[0x00000005] = false;   //TRUE
    }
    if (value < 0x00000000)     //Se o valor for menor que zero
    {
        flags_reg[0x00000002] = false;   //ZERO
        flags_reg[0x00000003] = true;   //NEG
        flags_reg[0x00000004] = true;   //NEGZERO
        flags_reg[0x00000005] = false;   //TRUE
    }
    if (value > 0x00000000)     //Se o valor for maior que zero
    {
        flags_reg[0x00000002] = false;   //ZERO
        flags_reg[0x00000003] = false;   //NEG
        flags_reg[0x00000004] = false;   //NEGZERO
        flags_reg[0x00000005] = true;   //TRUE
    }
}

void reset_neg_zero_true()
{
    flags_reg[0x00000002] = false;   //ZERO
    flags_reg[0x00000003] = false;   //NEG
    flags_reg[0x00000004] = false;   //NEGZERO
    flags_reg[0x00000005] = false;   //TRUE
}

bool get_flag(int flag)
{
    return flags_reg[flag];
}

void ula_add(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, false);      //Verifica a ocorrencia da flag overflow
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
    unsigned int value_0 = gpr[reg2];
    unsigned int value_1 = gpr[reg3];
    gpr[reg1] =  value_0 + value_1;
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], true, false);    //Verifica a ocorrencia da flag carry
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
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, false);      //Verifica a ocorrencia da flag overflow
    if (!overflow)
    {
        gpr[reg1] = gpr[reg1] + 0x00000001;
        overflow = check_and_set_carry_overflow(gpr[reg1], 0x00000001, false, false);      //Verifica a ocorrencia da flag overflow
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
    gpr[reg1] = ++gpr[reg2];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], 0x00000001, false, false);     //Verifica a ocorrencia da flag overflow
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
    gpr[reg1] = gpr[reg2] - gpr[reg3];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, true);       //Verifica a ocorrencia da flag overflow
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
    gpr[reg1] = gpr[reg2] - gpr[reg3];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, true);       //Verifica a ocorrencia da flag overflow
    if (!overflow)
    {
        gpr[reg1] = gpr[reg1] - 0x00000001;
        overflow = check_and_set_carry_overflow(gpr[reg1], 0x00000001, false, true);       //Verifica a ocorrencia da flag overflow
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
    gpr[reg1] = --gpr[reg2];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], 0x00000001, false, true);      //Verifica a ocorrencia da flag overflow
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);     //Se não ocorreu carry, verifica as flags zero, neg, negzero e true
    }
    else
    {
        reset_neg_zero_true();
    }
}

void ula_mult(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
    if ((hi == 0x00000000) && (lo == 0x00000000))
    {
        check_and_set_neg_zero_true(0x00000000);    //Se o número for zero, ativa flag zero e negzero
    }
    else
    {
        check_and_set_neg_zero_true(hi);    //Se o número for diferente de zero verifica as flags zero, neg, negzero e true
    }
}

void ula_multu(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
    if ((hi == 0x00000000) && (lo == 0x00000000))
    {
        check_and_set_neg_zero_true(0x00000000);        //Se o número for zero, ativa flag zero e negzero
    }
    else
    {
        check_and_set_neg_zero_true(hi);        //Se o número for diferente de zero verifica as flags zero, neg, negzero e true
    }
}

void ula_mfh(int reg1)
{
    gpr[reg1] = hi;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(hi);
}

void ula_mfl(int reg1)
{
    gpr[reg1] = lo;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(lo);
}

void ula_div(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] / gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_divu(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] / gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_asl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
    flags_reg[0x00000001] = (gpr[reg2] >> 31) ^ ((gpr[reg2] >> 30) & 0x00000001);
    flags_reg[0x00000006] = (gpr[reg2] >> 31); //carry
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_asr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_zeros(int reg1)
{
    gpr[reg1] = 0x00000000;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_ones(int reg1)
{
    gpr[reg1] = 0x00000001;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_passa(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_passnota(int reg1, int reg2)
{
    gpr[reg1] = ~gpr[reg2];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_and(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] & gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_andnota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) & gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_nand(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] & gpr[reg3]);
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_or(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] | gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_ornota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) | gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_nor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] | gpr[reg3]);
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] ^ gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xornota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) ^ gpr[reg3];
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_xnor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] ^ gpr[reg3]);
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}
/*************************
*/
void ula_lsl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
    flags_reg[0x00000001] = 0x00000000;
    flags_reg[0x00000006] = (gpr[reg2] >> 31); //carry
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_lsr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_slt(int reg1, int reg2, int reg3)
{
    if (gpr[reg2] < gpr[reg3])
    {
        gpr[reg1] = 0x00000001;
    }
    else
    {
        gpr[reg1] = 0x00000000;
    }
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void ula_sltu(int reg1, int reg2, int reg3)
{
    if (gpr[reg2] < gpr[reg3])
    {
        gpr[reg1] = 0x00000001;
    }
    else
    {
        gpr[reg1] = 0x00000000;
    }
    check_and_set_carry_overflow(0x00000000, 0x00000000, false, false);
    check_and_set_neg_zero_true(gpr[reg1]);     //Verifica as flags zero, neg, negzero e true
}

void const_loadlit(int reg1, int cst)
{
    gpr[reg1] = cst;
}

void const_lcl(int reg1, int cst)
{
    gpr[reg1] = cst | (gpr[reg1] & 0xFFFF0000);     //Pega os 16 bits mais significativos do valor no registrador e faz um OR com a constante
}

void const_lch(int reg1, int cst)
{
    gpr[reg1] = (cst << 8) | (gpr[reg1] & 0x0000FFFF);      //Pega os 16 bits menos significativos do valor no registrador e faz um OR com a constante deslocada 8 bits a esquerda
}

void mem_load(int reg1, int addr)
{
    gpr[reg1] = mem[gpr[addr]];     //Pega endereço no registrador, lê o valor neste endereço de memória e grava no registrador
}

void mem_store(int reg1, int addr)
{
    mem[gpr[addr]] = gpr[reg1];     //Grava no endereço de memória o valor no registrador
}

void jump(int addr)
{
    pc = addr;
}

void jump_true(int flag, int addr)
{
    bool is_true = get_flag(flag);
    if (is_true)
    {
        pc = addr;
    }
}

void jump_false(int flag, int addr)
{
    bool is_true = get_flag(flag);
    if (!is_true)
    {
        pc = addr;
    }
}

void jump_and_link(int addr)
{
    gpr[0x00000007] = pc;
    pc = addr;
}

void jump_register(int reg1)
{
    pc = gpr[reg1];
}

void decode_and_run_instruction(int ir)
{
    int layout = ir >> 0x0000001E; //Desloca 30 bits
    if (layout == 0x00000000)   //Layout de operações lógico-aritméticas é definido como 00
    {
        int opcode = (ir >> 0x0000000C) & 0x0000003F; //Desloca 12 bits e pega os 6 menos significativos
        int reg3 = ir & 0x0000000F;     //Pega os 4 bits menos significativos
        int reg2 = (ir >> 0x00000004) & 0x0000000F;     //Desloca 4 bits e pega os 4 menos significativos
        int reg1 = (ir >> 0x00000008) & 0x0000000F;     //Desloca 8 bits e pega os 4 menos significativos
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
            ula_mult(reg1, reg2);
        }
        else if (opcode == 0x00000008)
        {
            ula_multu(reg1, reg2);
        }
        else if (opcode == 0x00000009)
        {
            ula_mfh(reg1);
        }
        else if (opcode == 0x0000000A)
        {
            ula_mfl(reg1);
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
    else if (layout == 0x00000001)  //Layout de operações com constantes é definido como 01
    {
        int opcode = (ir >> 0x00000014) & 0x00000003; //Desloca 20 bits e pega os 2 menos significativos
        int cst = ir & 0x0000FFFF;      //Pega os 16 bits menos significativos
        int reg1 = (ir >> 0x0000000F) & 0x0000000F;     //Desloca 16 bits e pega os 4 menos significativos
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
    else if (layout == 0x00000002)  //Layout de operaões de acesso a memória é definida como 10
    {
        int opcode = (ir >> 0x00000008) & 0x00000003; //Desloca 8 bits e pega os 2 menos significativos
        int reg2 = ir & 0x0000000F;     //Pega os 4 bits menos significativos
        int reg1 = (ir >> 0x00000004) & 0x0000000F;     //Desloca 4 bits e pega os 4 menos significativos
        if (opcode == 0x00000000)
        {
            mem_load(reg1, reg2);
        }
        else if (opcode == 0x00000001)
        {
            mem_store(reg1, reg2);
        }
    }
    else if (layout == 0x00000003)  //Layout de operações de desvios e saltos é definido como 11
    {
        int opcode = (ir >> 0x00000014) & 0x0000000F; //Desloca 20 bits e pega 4 menos significativos
        int addr = ir & 0x00000FFF;     //Pega os 12 bits menos significativos
        int reg1 = (ir >> 0x0000000C) & 0x0000000F;     //Desloca 12 bits e pega os 4 menos significativos
        int cond = (ir >> 0x0000000F) & 0x0000000F;     //Desloca 16 bits e pega os 4 menos significativos
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
            jump_and_link(addr);
        }
        else if (opcode == 0x00000004)
        {
            jump_register(reg1);
        }
    }
}

void writeResult() {
	FILE *file_w = fopen("resultado simulador.txt", "w");    // Abre o arquivo de escrita

    int i;
    for (i = 0; i < 16; i++)    //Percorre os 16 registradores
        fprintf(file_w, "gpr%d: %d", i, gpr[i]);    //Imprime cada um no arquivo de saida
    for (i = 0; i < mem_lenth; i++) {   //Percorre a memoria utilezada
        fprintf(file_w, "mem(%d): %d", i, mem[i]);  //Imprime cada valor no arquivo de saida
    }
    fclose(file_w);
}

int main(int argc, char *argv[]) {
    char *file_name;
    if (argc > 1)               // Se o usuario tiver passado algum parametro
        file_name = argv[1];     // Define o nome do arquivo de entrada como o parametro recebido
    else
        file_name = "resultado montador.bin"; // Nome default do arquivo de entrada

	FILE *file_r = fopen(file_name, "r");      // Abre o arquivo de leitura

    if (file_r == NULL) {
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	    return -1;
	} else {
        char c = fgetc(file_r);      //Le um caractere do arquivo
        int instruction = 0;
        int i;
	    while(!feof(file_r)) {       //Verifica se chegou no fim do arquivo
            for(i = 0; i < 32; i++) {   //Percorre os 32 bits da palavra
                if (c == '1')       //Se o bit for '1'
                    instruction++;
                if (i != 31)
                    instruction = instruction << 1;     //Desloca um bit para a esquerda
                char c = fgetc(file_r);     //Le um caractere do arquivo
            }
            mem[mem_lenth] = instruction;   //Salva a palavra na memoria
            mem_lenth++;
	    }
	    while (true) {
            ir = mem[pc];                       //Transfere para o registrador IR a instrução que será executada
            pc++;                               //Incrementa o ponteiro de instruções
            decode_and_run_instruction(ir);     //Decodifica e executa a instrução
        }
    }
    fclose(file_r);      //Fecha o arquivo de entrada

    return 1;
}
