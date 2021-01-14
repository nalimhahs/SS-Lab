#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hexToDecimal(char acHex[])
{
    return (int)strtol(acHex, NULL, 16);
}
void main()
{
    char str[300], hexaddr[30];
    FILE *fp, *f1;
    int i, j, decaddr, start_addr;
    fp = fopen("input.obj", "r");
    f1 = fopen("loader.txt", "w");
    fscanf(fp, "%s", str);
    while (!feof(fp))
    {
        j = 0;
        if (str[0] == 'H')
        {
            for (i = 7; i <= 12; i++)
            {
                hexaddr[j++] = str[i];
            }
            hexaddr[j] = '\0';
            start_addr = hexToDecimal(hexaddr);
        }
        if (str[0] == 'T')
        {
            for (i = 2; i <= 7; i++)
            {
                hexaddr[j++] = str[i];
            }
            hexaddr[j] = '\0';
            decaddr = hexToDecimal(hexaddr);
            while (start_addr != decaddr)
            {
                fprintf(f1, "%x\tXX\n", start_addr);
                start_addr++;
            }
            for (i = 12; i < strlen(str); i = i + 2)
            {
                if (str[i] != '^')
                {
                    fprintf(f1, "%x\t%c%c\n", decaddr, str[i], str[i + 1]);
                    decaddr++;
                }
                else
                {
                    i--;
                }
            }
            start_addr = decaddr;
        }
        fscanf(fp, "%s", str);
    }
    fclose(fp);
    fclose(f1);
}