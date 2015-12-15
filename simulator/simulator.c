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

void ula_add(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
}

void ula_addu(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3];
}

void ula_addinc(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] + gpr[reg3] + 0x00000001;
}

void ula_inca(int reg1, int reg2)
{
    gpr[reg1] = ++gpr[reg2];
}

void ula_sub(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] - gpr[reg3];
}

void ula_subdec(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] - gpr[reg3] - 0x00000001;
}

void ula_deca(int reg1, int reg2)
{
    gpr[reg1] = --gpr[reg2];
}

void ula_mult(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
}

void ula_multu(int reg1, int reg2)
{
    hi = (gpr[reg1] * gpr[reg2]);
    lo = (gpr[reg1] * gpr[reg2]);
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
}

void ula_divu(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] / gpr[reg3];
}

void ula_asl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
}

void ula_asr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
}

void ula_zeros(int reg1)
{
    gpr[reg1] = 0x00000000;
}

void ula_ones(int reg1)
{
    gpr[reg1] = 0x00000001;
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
}

void ula_andnota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) & gpr[reg3];
}

void ula_nand(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] & gpr[reg3]);
}

void ula_or(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] | gpr[reg3];
}

void ula_ornotb(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] | (~gpr[reg3]);
}

void ula_nor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] | gpr[reg3]);
}

void ula_xor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = gpr[reg2] ^ gpr[reg3];
}

void ula_xornota(int reg1, int reg2, int reg3)
{
    gpr[reg1] = (~gpr[reg2]) ^ gpr[reg3];
}

void ula_xnor(int reg1, int reg2, int reg3)
{
    gpr[reg1] = ~(gpr[reg2] ^ gpr[reg3]);
}

void ula_lsl(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] << 0x00000001;
}

void ula_lsr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x00000001;
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
    gpr[reg1] = mem[addr];
}

void mem_store(int reg1, int addr)
{
    mem[addr] = gpr[reg1];
}

void jump(int addr)
{
    pc = addr;
}

void jump_true(int flag, int addr)
{
    bool is_true = check_flag(flag);
    if (is_true)
    {
        pc = addr;
    }
}

void jump_false(int flag, int addr)
{
    bool is_true = check_flag(flag);
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

bool check_flag(int flag)
{
    return flags_reg[flag];
}

void set_flags(int flag1, int flag2, int flag3, int flag4, int flag5, int flag6)
{
    flags_reg[0x00000001] = flag1;   //OVERFLOW
    flags_reg[0x00000002] = flag2;   //ZERO
    flags_reg[0x00000003] = flag3;   //NEG
    flags_reg[0x00000004] = flag4;   //NEGZERO
    flags_reg[0x00000005] = flag5;   //TRUE
    flags_reg[0x00000006] = flag6;   //CARRY
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
        int reg1 = (ir >> 0x0000FFFF) & 0x0000000F;
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
        int opcode = (ir >> 0x0000000C) & 0x00000001; //Desloca 12 bits e pega o bit menos significativo
        int cst = ir & 0x0000FFFF;
        int reg1 = (ir >> 0x0000FFFF) & 0x0000000F;
        if (opcode == 0x00000000)
        if (opcode == 0x00000000)
        {
            mem_load();
        }
        else if (opcode == 0x00000001)
        {
            mem_store();
        }
    }
    else if (layout == 0x00000003)  //Layout de operações de desvios e saltos
    {
        int opcode = (ir >> 0x00000014) & 0x0000000F; //Desloca 20 bits e pega 4 menos significativos
        if (opcode == 0x00000000)
        {
            jump();
        }
        else if (opcode == 0x00000001)
        {
            jump_true();
        }
        else if (opcode == 0x00000002)
        {
            jump_false();
        }
        else if (opcode == 0x00000003)
        {
            jump_and_link();
        }
        else if (opcode == 0x00000004)
        {
            jump_register();
        }
    }
}
void main(void)
{
    int a = 0x00000014;
    int b = 0x0000000c;
    printf ("20=%d, 12=%d", a, b);
    int i;
    while (true)
    {
        ir = mem[pc];
        pc++;
        decode_and_run_instruction(ir);
    }
}
