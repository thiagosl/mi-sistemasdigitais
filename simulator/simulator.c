#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int gpr[16];
int flags_reg[7];
int mem[200] = {0x1345AF2D};
int pc = 0x00000000;
int ir;
int hi;
int lo;

bool check_and_set_carry_overflow(int v0, int v1, bool unsigned_operation, bool subs_operation)
{
    if (unsigned_operation)
    {
        unsigned int value_0 = (unsigned int) v0;
        unsigned int value_1 = (unsigned int) v1;
        unsigned int max = 0xFFFFFFFF;
        if (!subs_operation && (value_1 > (max - value_0)))
        {
            flags_reg[0x00000001] = true;   //OVERFLOW
            return true;
        }
        else
        {
            flags_reg[0x00000001] = false;
            return false;
        }
    }
    else
    {
        int value_0 = v0;
        int value_1 = v1;
        int max = 0x7FFFFFFF;
        int min = 0x80000000;
        if ((value_0 > 0) && (value_1 > 0))
        {
            if (!subs_operation && (value_1 > (max - value_0)))
            {
                flags_reg[0x00000006] = true;   //CARRY
                return true;
            }
            else
            {
                flags_reg[0x00000006] = false;
                return false;
            }
        }
        else if ((value_0 < 0) && (value_1 < 0))
        {
            if (!subs_operation && (value_1 < (min - value_0)))
            {
                flags_reg[0x00000006] = true;   //CARRY
                return true;
            }
            else
            {
                flags_reg[0x00000006] = false;
                return false;
            }
        }
        else
        {
            flags_reg[0x00000006] = false;
            return false;
        }
    }
}

void check_and_set_neg_zero_true(int value)
{
    if (value == 0x00000000)
    {
        flags_reg[0x00000002] = true;   //ZERO
        flags_reg[0x00000004] = true;   //NEGZERO
        flags_reg[0x00000005] = false;   //TRUE
    }
    if (value < 0x00000000)
    {
        flags_reg[0x00000003] = true;   //NEG
        flags_reg[0x00000004] = true;   //NEGZERO
        flags_reg[0x00000005] = false;   //TRUE
    }
    if (value > 0x00000000)
    {
        flags_reg[0x00000002] = false;   //ZERO
        flags_reg[0x00000003] = false;   //NEG
        flags_reg[0x00000004] = false;   //NEGZERO
        flags_reg[0x00000005] = true;   //TRUE
    }
}

bool get_flag(int flag)
{
    return flags_reg[flag];
}

void ula_add(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, false);
    if (!carry)
    {
        check_and_set_neg_zero_true(gpr[reg1]);
    }
}

void ula_addu(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    bool overflow = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], true, false);
    if (!overflow)
    {
        check_and_set_neg_zero_true(gpr[reg1]);
    }
}

void ula_addinc(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, false);
    if (!carry)
    {
        gpr[reg1] = gpr[reg1] + 0x00000001;
        carry = check_and_set_carry_overflow(gpr[reg1], 0x00000001, false, false);
        if (!carry)
        {
            check_and_set_neg_zero_true(gpr[reg1]);
        }
    }
}

void ula_inca(int reg1, int reg2)
{
    gpr[reg1] = ++gpr[reg2];
    bool carry = check_and_set_carry_overflow(gpr[reg2], 0x00000001, false, false);
    if (!carry)
    {
        check_and_set_neg_zero_true(gpr[reg1]);
    }
}

void ula_sub(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] - gpr[reg3];
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, true);
    if (!carry)
    {
        check_and_set_neg_zero_true(gpr[reg1]);
    }
}

void ula_subdec(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] - gpr[reg3];
    bool carry = check_and_set_carry_overflow(gpr[reg2], gpr[reg3], false, true);
    if (!carry)
    {
        gpr[reg1] = gpr[reg1] - 0x00000001;
        carry = check_and_set_carry_overflow(gpr[reg1], 0x00000001, false, true);
        if (!carry)
        {
            check_and_set_neg_zero_true(gpr[reg1]);
        }
    }
}

void ula_deca(int reg1, int reg2)
{
    gpr[reg1] = --gpr[reg2];
    bool carry = check_and_set_carry_overflow(gpr[reg2], 0x00000001, false, true);
    if (!carry)
    {
        check_and_set_neg_zero_true(gpr[reg1]);
    }
}

void ula_mult(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
    if ((hi == 0x00000000) && (lo == 0x00000000))
    {
        check_and_set_neg_zero_true(0x00000000);
    }
    else
    {
        check_and_set_neg_zero_true(hi);
    }
}

void ula_multu(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
    if ((hi == 0x00000000) && (lo == 0x00000000))
    {
        check_and_set_neg_zero_true(0x00000000);
    }
    else
    {
        check_and_set_neg_zero_true(hi);
    }
}

void ula_mfh(int reg1)
{
    gpr[reg1] = hi;
}

void ula_mfl(int reg1)
{
    gpr[reg1] = lo;
}

void ula_div(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] / gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_divu(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] / gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_asl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_asr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_zeros(int reg1)
{
    gpr[reg1] = 0x00000000;
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_ones(int reg1)
{
    gpr[reg1] = 0x00000001;
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_passa(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2];
}

void ula_passnota(int reg1, int reg2)
{
    gpr[reg1] = ~gpr[reg2];
}

void ula_and(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] & gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_andnota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) & gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_nand(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] & gpr[reg3]);
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_or(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] | gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_ornotb(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] | (~gpr[reg3]);
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_nor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] | gpr[reg3]);
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_xor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] ^ gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_xornota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) ^ gpr[reg3];
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_xnor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] ^ gpr[reg3]);
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_lsl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
    check_and_set_neg_zero_true(gpr[reg1]);
}

void ula_lsr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
    check_and_set_neg_zero_true(gpr[reg1]);
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
    check_and_set_neg_zero_true(gpr[reg1]);
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
    check_and_set_neg_zero_true(gpr[reg1]);
}

void const_loadlit(int reg1, int cst)
{
    gpr[reg1] = cst;
}

void const_lcl(int reg1, int cst)
{
    gpr[reg1] = cst | (gpr[reg1] & 0xFFFF0000);
}

void const_lch(int reg1, int cst)
{
    gpr[reg1] = (cst << 8) | (gpr[reg1] & 0x0000FFFF);
}

void mem_load(int reg1, int addr)
{
    gpr[reg1] = mem[gpr[addr]];
}

void mem_store(int reg1, int addr)
{
    mem[gpr[addr]] = gpr[reg1];
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
    if (layout == 0x00000000)   //Layout de operações lógico-aritméticas
    {
        int opcode = (ir >> 0x0000000C) & 0x0000003F; //Desloca 12 bits e pega os 6 menos significativos
        int reg3 = ir & 0x0000000F;
        int reg2 = (ir >> 0x00000004) & 0x0000000F;
        int reg1 = (ir >> 0x00000008) & 0x0000000F;
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
            ula_ornotb(reg1, reg2, reg3);
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
    else if (layout == 0x00000001)  //Layout de operações com constantes
    {
        int opcode = (ir >> 0x00000014) & 0x00000003; //Desloca 20 bits e pega 2 menos significativos
        int cst = ir & 0x0000FFFF;
        int reg1 = (ir >> 0x0000000F) & 0x0000000F;
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
    else if (layout == 0x00000002)  //Layout de operaões de acesso a memória
    {
        int opcode = (ir >> 0x00000008) & 0x00000003; //Desloca 8 bits e pega os 2 bits menos significativos
        int reg2 = ir & 0x0000000F;
        int reg1 = (ir >> 0x00000004) & 0x0000000F;
        if (opcode == 0x00000000)
        {
            mem_load(reg1, reg2);
        }
        else if (opcode == 0x00000001)
        {
            mem_store(reg1, reg2);
        }
    }
    else if (layout == 0x00000003)  //Layout de operações de desvios e saltos
    {
        int opcode = (ir >> 0x00000014) & 0x0000000F; //Desloca 20 bits e pega 4 menos significativos
        int addr = ir & 0x00000FFF;
        int reg1 = (ir >> 0x0000000C) & 0x0000000F;
        int cond = (ir >> 0x0000000F) & 0x0000000F;
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
void main(void)
{
    int max = 0x7FFFFFFF;
    int min = 0x80000000;
    printf ("%d %d", max, min);
    while (true)
    {
        ir = mem[pc];
        pc++;
        decode_and_run_instruction(ir);
    }
}
