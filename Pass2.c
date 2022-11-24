#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int numPlaces(int n)
{
    if (n == 0)
        return 1;
    return floor(log10(abs(n))) + 1;
}

int main()
{
    FILE *fint, *ftab, *flen, *fsym, *fobj;
    int op1[10], txtlen, txtlen1, i, j = 0, len;
    char add[5], symadd[5], op[5], start[10], temp[30], line[20], label[20], mne[10], operand[10], symtab[10], opmne[10];
    fint = fopen("input.txt", "r");
    flen = fopen("length.txt", "r");
    ftab = fopen("optab.txt", "r");
    fsym = fopen("symbol.txt", "r");
    fobj = fopen("objectCode.txt", "w+");
    fscanf(fint, "%s%s%s%s", add, label, mne, operand);
    if (strcmp(mne, "START") == 0)
    {
        strcpy(start, operand);
        fscanf(flen, "%d", &len);
    }
    char paddedLabel[] = "______";
    int padLength = 6 - strlen(label);

    fprintf(fobj, "H^%s%*.*s^%06d^%06d\nT^00%s^", label, padLength, padLength, paddedLabel, atoi(start), len, start);
    fscanf(fint, "%s%s%s%s", add, label, mne, operand);
    int textLength = 0;
    int extraLengthToMove = 0;
    while (strcmp(mne, "END") != 0)
    {
        fscanf(ftab, "%s%s", opmne, op);
        while (!feof(ftab))
        {
            if (strcmp(mne, opmne) == 0)
            {
                fclose(ftab);
                fscanf(fsym, "%s%s", symadd, symtab);
                while (!feof(fsym))
                {
                    if (strcmp(operand, symtab) == 0)
                    {
                        fprintf(fobj, "%s%s^", op, symadd);
                        textLength += strlen(op) + strlen(symadd);
                        extraLengthToMove++;
                        break;
                    }
                    else
                        fscanf(fsym, "%s%s", symadd, symtab);
                }
                break;
            }
            else
                fscanf(ftab, "%s%s", opmne, op);
        }
        if ((strcmp(mne, "BYTE") == 0) || (strcmp(mne, "WORD") == 0))
        {
            if (strcmp(mne, "WORD") == 0)
            {
                fprintf(fobj, "00000%s^", operand);
                textLength += (strlen(operand) + 5);
                extraLengthToMove++;
            }
            else
            {
                len = strlen(operand);
                for (i = 2; i < len; i++)
                {
                    fprintf(fobj, "%d", operand[i]);
                    textLength += numPlaces(operand[i]);
                    extraLengthToMove++;
                }
                fprintf(fobj, "^");
            }
        }

        fscanf(fint, "%s%s%s%s", add, label, mne, operand);
        ftab = fopen("optab.txt", "r");
        fseek(ftab, SEEK_SET, 0);
    }

    // printf("Length: %d\n", textLength);
    fseek(fobj, -1 * (textLength + extraLengthToMove), SEEK_CUR);
    char currTextLine[100];
    fscanf(fobj, "%s", currTextLine);
    fseek(fobj, -1 * (textLength + extraLengthToMove), SEEK_CUR);
    // printf("\n%s\n", currTextLine);
    // printf("%ld", ftell(fobj));

    fprintf(fobj, "^%d%s", (int)ceil((float)textLength / 2), currTextLine);

    fprintf(fobj, "\nE^00%s", start);
    fclose(fint);
    fclose(ftab);
    fclose(fsym);
    fclose(flen);
    fclose(fobj);
    return 0;
}
