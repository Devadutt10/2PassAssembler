#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]);
void display();

int main()
{
    // for reading from input
    char label[10], opcode[10], operand[10];
    // for reading from optab
    char code[10], mnemonic[3];
    // call the function
    passOne(label, opcode, operand, code, mnemonic);

    return 0;
}

void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]) 
{
    int locctr, start, length;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    // Open files and check for success
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symtab.txt", "w");
    fp4 = fopen("intermediate.txt", "w");
    fp5 = fopen("length.txt", "w");

    if (!fp1 || !fp2 || !fp3 || !fp4 || !fp5) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        locctr = start;
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    } else {
        locctr = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        if (strcmp(label, "**") != 0) {
            fprintf(fp3, "%s\t%d\n", label, locctr);
        }

        // Reset fp2 for each new opcode lookup
        rewind(fp2);

        while (fscanf(fp2, "%s\t%s", code, mnemonic) != EOF) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;
                break;
            }
        }

        if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += (3 * atoi(operand));
        } else if (strcmp(opcode, "BYTE") == 0) {
            ++locctr;
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        }

        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    }

    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Close files
    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    display();

    length = locctr - start;
    fprintf(fp5, "%d", length);
    fclose(fp5);
    printf("\nThe length of the code : %d\n", length);
}

void display() {

    char str;
    FILE *fp1, *fp2, *fp3;

    // Input Table
    printf("\nThe contents of Input Table :\n\n");
    fp1 = fopen("input.txt", "r");
    if (!fp1) {
        perror("Error opening input.txt");
        exit(EXIT_FAILURE);
    }
    while ((str = fgetc(fp1)) != EOF) {
        putchar(str);
    }
    fclose(fp1);

    // Output table
    printf("\n\nThe contents of Output Table :\n\n");
    fp2 = fopen("intermediate.txt", "r");
    if (!fp2) {
        perror("Error opening intermediate.txt");
        exit(EXIT_FAILURE);
    }
    while ((str = fgetc(fp2)) != EOF) {
        putchar(str);
    }
    fclose(fp2);

    // Symtable
    printf("\n\nThe contents of Symbol Table :\n\n");
    fp3 = fopen("symtab.txt", "r");
    if (!fp3) {
        perror("Error opening symtab.txt");
        exit(EXIT_FAILURE);
    }
    while ((str = fgetc(fp3)) != EOF) {
        putchar(str);
    }
    fclose(fp3);
}

