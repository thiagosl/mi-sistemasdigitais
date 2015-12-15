#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

int gpr[16];
int flags_reg[6];
int mem[200] = {0x1345AF2D};
int pc = 0x00;
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
    gpr[reg1] = gpr[reg2] + gpr[reg3] + 0x01;
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
    gpr[reg1] = gpr[reg2] - gpr[reg3] - 0x01;
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
    gpr[reg1] = gpr[reg2] << 0x01;
}

void ula_asr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 0x01;
}

void ula_zeros(int reg1)
{
    gpr[reg1] = 0x00;
}

void ula_ones(int reg1)
{
    gpr[reg1] = 0x01;
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
    gpr[reg1] = gpr[reg2] << 1;
}

void ula_lsr(int reg1, int reg2)
{
    gpr[reg1] = gpr[reg2] >> 1;
}

void ula_slt(int reg1, int reg2, int reg3)
{
    if (gpr[reg2] < gpr[reg3])
    {
        gpr[reg1] = 0x0001;
    }
    else
    {
        gpr[reg1] = 0x0000;
    }
}

void ula_sltu(int reg1, int reg2, int reg3)
{
    if (gpr[reg2] < gpr[reg3])
    {
        gpr[reg1] = 0x0001;
    }
    else
    {
        gpr[reg1] = 0x0000;
    }
}

void const_loadlit(int reg1, int cst)
{
    gpr[reg1] = cst;
}

void const_lcl(int reg1, int cst)
{
    gpr[reg1] = cst | (gpr[reg1] & 0xffff0000);
}

void const_lch(int reg1, int cst)
{
    gpr[reg1] = (cst << 8) | (gpr[reg1] & 0x0000ffff); //tem que ver
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
    gpr[0x0110] = pc;
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

void main(void)
{
    int a = 0x0001;
    printf ("%d", a);
    int i;
    while (true)
    {
        ir = mem[pc];
        pc++;
        /*
        if (layout 1)
        {
            if (inst 1)
            {
                ula_add(reg1, reg2, reg3);
            }
            else if (inst 2)
            {
                ula_sub(reg1, reg2, reg3);
            }
        }
        else if (layout 2)
        {
            if (inst 1)
            {
                jump(dest);
            }
            else if (inst 2)
            {

            }
        }
        else if (layout 3)
        {

        }*/
    }
}
