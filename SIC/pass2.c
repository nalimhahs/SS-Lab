#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// OPTAB
struct optab
{
    char mnemonic[10];
    int opcode;
} optab[100];

int optab_len = -1;

void load_optab()
{
    FILE *optab_file = fopen("optab.txt", "r");
    int i = 0;
    while (!(feof(optab_file)))
    {
        fscanf(optab_file, "%s %d", optab[i].mnemonic, &optab[i].opcode);
        i++;
    }
    optab_len = i;
    fclose(optab_file);
}

int get_opcode(char mn[10])
{
    for (int i = 0; i < optab_len; i++)
    {
        if (strcmp(optab[i].mnemonic, mn) == 0)
        {
            return optab[i].opcode;
        }
    }
    return -1;
}

// SYMTAB
struct symtab
{
    char symbol[10];
    int addr;
} symtab[100];

int symtab_len = 0;

int search_symtab(char sym[10])
{
    for (int i = 0; i < symtab_len; i++)
    {
        if (strcmp(symtab[i].symbol, sym) == 0)
        {
            return symtab[i].addr;
        }
    }
    return -1;
}

int add_symbol(char sym[10], int loc)
{
    if (search_symtab(sym) == -1)
    {
        strcpy(symtab[symtab_len].symbol, sym);
        symtab[symtab_len].addr = loc;
        symtab_len += 1;
        return 1;
    }
    return 0;
}

void load_symtab()
{
    FILE *symtab_file = fopen("symtab.txt", "r");
    int i = 0;
    while (!(feof(symtab_file)))
    {
        fscanf(symtab_file, "%s %d", symtab[i].symbol, &symtab[i].addr);
        if (strcmp(symtab[i].symbol, ""))
            continue;
        i++;
    }
    symtab_len = i;
    fclose(symtab_file);
}

void dump_symtab()
{
    FILE *symtab_file = fopen("symtab.txt", "w");
    int i = 0;
    while (i < symtab_len)
    {
        fprintf(symtab_file, "%s %d\n", symtab[i].symbol, symtab[i].addr);
        i++;
    }
    fclose(symtab_file);
}

/*
TEST PROG:
add_symbol("a", 100);
add_symbol("b", 200);
add_symbol("c", 300);
printf("%d\n", search_symtab("c"));
dump_symtab();
load_symtab();
printf("%d\n", search_symtab("c"));
*/

// Intermediate File
struct inttab
{
    int addr;
    char symbol[10];
    char inst[10];
    char value[10];

} inttab[100];

int inttab_len = 0;

void write_intermediate_inst(int addr, char sym[10], char inst[10], char val[])
{

    inttab[inttab_len].addr = addr;
    strcpy(inttab[inttab_len].inst, inst);
    strcpy(inttab[inttab_len].symbol, strcmp(sym, "") == 0 ? "-" : sym);
    strcpy(inttab[inttab_len].value, val);

    inttab_len += 1;
}

void load_inttab()
{
    FILE *inttab_file = fopen("inttab.txt", "r");
    int i = 0;
    while (!(feof(inttab_file)))
    {
        fscanf(inttab_file, "%d %s %s %s", &inttab[i].addr, inttab[i].symbol, inttab[i].inst, inttab[i].value);
        i++;
    }
    inttab_len = i;
    fclose(inttab_file);
}

void dump_inttab()
{
    FILE *inttab_file = fopen("inttab.txt", "w");
    int i = 0;
    while (i < inttab_len)
    {
        fprintf(inttab_file, "%d %s %s %s\n", inttab[i].addr, inttab[i].symbol, inttab[i].inst, inttab[i].value);
        i++;
    }
    fclose(inttab_file);
}

void main()
{
    load_optab();
    FILE *intermediate_file = fopen("inttab.txt", "r");
    char prog_name[10], inst[10], start_addr[10], symbol[10], value[10];
    int locctr = 0;
    fscanf(intermediate_file, "%s %s %s", prog_name, inst, start_addr);
    if (strcmp(inst, "START") == 0)
    {
        locctr = atoi(start_addr);
        write_intermediate_inst(0, prog_name, inst, start_addr);
    }
    while (!(feof(intermediate_file)))
    {
        fscanf(intermediate_file, "%s %s %s", symbol, inst, value);
        if (strcmp(inst, "END") == 0)
        {
            printf("\nProgram size: %d\n", locctr - atoi(start_addr));
            write_intermediate_inst(locctr, symbol, inst, value);
            fclose(intermediate_file);
            break;
        }

        int temp_addr = locctr;

        if (strcmp(symbol, "") != 0 && strcmp(symbol, "-") != 0) 
        {
            // Duplicate Symbol
            if (!add_symbol(symbol, locctr))
                break;
        }

        if (get_opcode(inst) != -1)
        {
            locctr += 3;
        }
        else if (strcmp(inst, "WORD") == 0)
        {
            locctr += 3;
        }
        else if (strcmp(inst, "BYTE") == 0)
        {
            if (value[0] == 'C')
            {
                locctr += strlen(value) - 3;
            }
            else if (value[0] == 'X')
            {
                locctr += (strlen(value) - 3) / 2;
            }
        }
        else if (strcmp(inst, "RESW") == 0)
        {
            locctr += atoi(value) * 3;
        }
        else if (strcmp(inst, "RESB") == 0)
        {
            locctr += atoi(value);
        }
        else
        {
            // Invalid OPCODE
            break;
        }

        write_intermediate_inst(temp_addr, symbol, inst, value);
    }
    dump_inttab();
    dump_symtab();
}