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
        fscanf(optab_file, "%s %x", optab[i].mnemonic, &optab[i].opcode);
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
        fscanf(symtab_file, "%s %x", symtab[i].symbol, &symtab[i].addr);

        if (strcmp(symtab[i].symbol, "") == 0)
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
        fprintf(symtab_file, "%s %x\n", symtab[i].symbol, symtab[i].addr);
        i++;
    }
    fclose(symtab_file);
}


struct inttab
{
    int addr;
    char symbol[10];
    char inst[10];
    char value[10];

} inttab[100], int_inst;

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
        fscanf(inttab_file, "%x %s %s %s", &inttab[i].addr, inttab[i].symbol, inttab[i].inst, inttab[i].value);
        i++;
    }
    inttab_len = i - 2;
    fclose(inttab_file);
}

void dump_inttab()
{
    FILE *inttab_file = fopen("inttab.txt", "w");
    int i = 0;
    while (i < inttab_len)
    {
        fprintf(inttab_file, "%x %s %s %s\n", inttab[i].addr, inttab[i].symbol, inttab[i].inst, inttab[i].value);
        i++;
    }
    fclose(inttab_file);
}

void main()
{
    load_optab();
    load_inttab();
    load_symtab();

    FILE *object_file = fopen("output.obj", "w");
    char prog_name[10], inst[10], symbol[10], value[10];
    int i = 1, flag = 1, count = 0;

    int start_addr = (int)strtol(inttab[0].value, NULL, 16);

    if (strcmp(inttab[0].inst, "START") == 0)
    {
        fprintf(object_file, "H^%6s^%06x^%06x", inttab[0].symbol, start_addr, inttab[inttab_len].addr - start_addr);
    }
    while (1)
    {
        if (strcmp(inttab[i].inst, "END") == 0)
        {
            fprintf(object_file, "\nE^%06x", start_addr);
            break;
        }

        int size = inttab[i + 1].addr - inttab[i].addr;
        if (count + size > 30 || flag == 1)
        {
            fprintf(object_file, "\nT^%06x^00", inttab[i].addr);
            flag = 0;
            count = 0;
        }
        else
        {
            count += size;
            i++;
        }

        int opcode = get_opcode(inttab[i].inst);
        if (opcode != -1)
        {
            if (strcmp(inttab[i].value, "-") == 0)
            {
                fprintf(object_file, "^%02x0000", opcode);
            }
            else
            {
                int address = search_symtab(inttab[i].value);
                fprintf(object_file, "^%02x%04x", opcode, address);
            }
        }
        else if (strcmp(inttab[i].inst, "BYTE") == 0)
        {
            if (inttab[i].value[0] == 'C')
            {
                fprintf(object_file, "^");
                printf("%s", inttab[i].value);
                for (int j = 2; j < strlen(inttab[i].value) - 1; j++)
                {
                    fprintf(object_file, "%02x", inttab[i].value[j]);
                }
            }
            else if (inttab[i].value[0] == 'X')
            {
                fprintf(object_file, "^%x", atoi(inttab[i].value));
            }
        }
        else if (strcmp(inttab[i].inst, "WORD") == 0)
        {
            fprintf(object_file, "^%06x", atoi(inttab[i].value));
        }
    }
    fclose(object_file);
}