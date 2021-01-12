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
        fprintf(symtab_file, "%s %x\n", symtab[i].symbol, symtab[i].addr);
        i++;
    }
    fclose(symtab_file);
}

// Table to store forward references
struct reftab
{
    char symbol[10];
    int pos;

} reftab[100];

int reftab_len = 0;

void add_ref(char symbol[10], int pos)
{
    strcpy(reftab[reftab_len].symbol, symbol);
    reftab[reftab_len++].pos = pos;
}

void write_ref(FILE *object_file, char sym[10], int value)
{
    for (int i = 0; i < reftab_len; i++)
    {
        if (strcmp(reftab[i].symbol, sym) == 0)
        {
            fseek(object_file, reftab[i].pos + 3, SEEK_SET);
            fprintf(object_file, "%04x", value);
        }
    }
}

void dump_reftab()
{
    FILE *reftab_file = fopen("reftab.txt", "w");
    int i = 0;
    while (i < reftab_len)
    {
        fprintf(reftab_file, "%s %x\n", reftab[i].symbol, reftab[i].pos);
        i++;
    }
    fclose(reftab_file);
}

void resolve_ref(FILE *object_file)
{
    for (int i = 0; i < symtab_len; i++)
    {
        write_ref(object_file, symtab[i].symbol, symtab[i].addr);
    }
}

int get_inst_size(char inst[10], char value[10])
{
    int size = -1;
    if (get_opcode(inst) != -1)
    {
        size = 3;
    }
    else if (strcmp(inst, "WORD") == 0)
    {
        size = 3;
    }
    else if (strcmp(inst, "BYTE") == 0)
    {
        if (value[0] == 'C')
        {
            size = strlen(value) - 3;
        }
        else if (value[0] == 'X')
        {
            size = (strlen(value) - 3) / 2;
        }
    }
    else if (strcmp(inst, "RESW") == 0)
    {
        size = atoi(value) * 3;
    }
    else if (strcmp(inst, "RESB") == 0)
    {
        size = atoi(value);
    }
    return size;
}

void main()
{
    load_optab();
    FILE *input_file = fopen("input.txt", "r");
    FILE *object_file = fopen("output.obj", "w");
    int i = 1, flag = 1, count = 0;
    char prog_name[10], inst[10], start_addr_char[10], symbol[10], value[10];
    int locctr = 0, start_addr, rec_start_addr;
    fscanf(input_file, "%s %s %s", prog_name, inst, start_addr_char);

    if (strcmp(inst, "START") == 0)
    {
        start_addr = (int)strtol(start_addr_char, NULL, 16);
        locctr = start_addr;
        rec_start_addr = locctr;
        fprintf(object_file, "H^%6s^%06x^%06x", prog_name, start_addr, start_addr); // replace with prog len
    }

    while (!(feof(input_file)))
    {
        fscanf(input_file, "%s %s %s", symbol, inst, value);
        if (strcmp(inst, "END") == 0)
        {
            fseek(object_file, rec_start_addr - locctr - 3, SEEK_CUR);
            fprintf(object_file, "%02x", (locctr - rec_start_addr) / 2);
            fseek(object_file, 0, SEEK_END);
            fprintf(object_file, "\nE^%06x", start_addr);
            fclose(input_file);
            break;
        }

        int temp_addr = locctr;

        if (strcmp(symbol, "") != 0 && strcmp(symbol, "-") != 0)
        {
            // Duplicate Symbol
            if (!add_symbol(symbol, locctr))
                break;
        }
        int size = get_inst_size(inst, value);
        if (strcmp(inst, "RESW") == 0 || strcmp(inst, "RESB") == 0)
        {
            locctr += size;
            flag = 1;
            // count = 0;
            continue;
        }
        else if (count + size > 30 || flag == 1)
        {
            fseek(object_file, -count * 3 + 3, SEEK_CUR);
            fprintf(object_file, "%02x", (locctr - rec_start_addr) / 2);
            fseek(object_file, 0, SEEK_END);
            printf("%d\n", locctr - rec_start_addr);
            fprintf(object_file, "\nT^%06x^00", locctr);
            rec_start_addr = locctr;
            flag = 0;
            count = 0;
        }
        else
        {
            count += size;
            i++;
        }

        int opcode = get_opcode(inst);
        if (opcode != -1)
        {
            if (strcmp(value, "-") == 0)
            {
                fprintf(object_file, "^%02x0000", opcode);
            }
            else
            {
                int address = search_symtab(value);
                // if not found add to forward ref table
                if (address == -1)
                {
                    int pos = ftell(object_file);
                    add_ref(value, pos);
                    fprintf(object_file, "^%02x%04x", opcode, 0);
                }
                else
                {
                    fprintf(object_file, "^%02x%04x", opcode, address);
                }
            }
            locctr += 3;
        }
        else if (strcmp(inst, "WORD") == 0)
        {
            fprintf(object_file, "^%06x", atoi(value));
            locctr += 3;
        }
        else if (strcmp(inst, "BYTE") == 0)
        {
            if (value[0] == 'C')
            {
                fprintf(object_file, "^");
                for (int j = 2; j < strlen(value) - 1; j++)
                {
                    fprintf(object_file, "%02x", value[j]);
                }
                locctr += strlen(value) - 3;
            }
            else if (value[0] == 'X')
            {
                fprintf(object_file, "^%x", atoi(value));
                locctr += (strlen(value) - 3) / 2;
            }
        }
        else
        {
            // Invalid OPCODE
            // break;
        }
    }
    fseek(object_file, 16, SEEK_SET);
    fprintf(object_file, "%06x", (locctr - start_addr));
    fprintf(object_file, "     ");
    dump_symtab();
    dump_reftab();
    resolve_ref(object_file);
    fclose(object_file);
}
