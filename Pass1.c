#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maxLineSize 100
#define maxOpcodeEntries 1000
#define maxSourceLines 1000

struct optabEntry
{
	char opcode[100];
	char machineCode[100];
} optab[maxOpcodeEntries];
int opcodeindex = 0, opCodeLength = 0;

struct sourceLine
{
	char label[100];
	char opcode[100];
	char operand[100];
} sourceCode[maxSourceLines];
int sourceIndex = 0, sourceLength = 0;

struct symtabEntry
{
	char label[100];
	int address;
} symtab[maxSourceLines];
int symtabIndex = 0, symtabLength = 0;

void parseOptab(FILE *optabFile)
{
	char line[maxLineSize];
	while (fgets(line, maxLineSize, optabFile))
	{
		char *opcode = strtok(line, " ");
		char *machineCode = strtok(NULL, " ");
		strcpy(optab[opcodeindex].opcode, opcode);
		strcpy(optab[opcodeindex].machineCode, machineCode);

		opcodeindex++;
		opCodeLength++;
	}
}

void parseSourceFile(FILE *sourceFile)
{
	char line[maxLineSize];
	int lineNo = 0;
	while (fgets(line, sizeof line, sourceFile))
	{
		char label[10], opcode[10], operand[10];
		if (sscanf(line, "%s %s %s", label, opcode, operand) == 3)
		{
			// printf("%s %s %s \n", label, opcode, operand);
			strcpy(sourceCode[sourceIndex].label, label);
			strcpy(sourceCode[sourceIndex].opcode, opcode);
			strcpy(sourceCode[sourceIndex].operand, operand);
			// printf("%s", sourceCode[sourceIndex].opcode);
		}
		else if (sscanf(line, "%s %s", opcode, operand) == 2)
		{
			strcpy(sourceCode[sourceIndex].label, "");
			strcpy(sourceCode[sourceIndex].opcode, opcode);
			strcpy(sourceCode[sourceIndex].operand, operand);
		}
		else
		{
			printf("INVALID SOURCE LINE at Line: %d\n", lineNo);
			exit(0);
		}

		sourceIndex++;
		sourceLength++;
	}
}

int hasDupEntrySymtab(char label[])
{
	for (int i = 0; i < symtabLength; i++)
	{
		printf("%s %s", symtab[i].label, label);
		if (strcmp(symtab[i].label, label) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int startingAddress = 0x0, LOCCTR = 0x0;

void assemble(FILE *symtabFile, FILE *intermediateFile)
{

	struct sourceLine firstLine = sourceCode[0];
	int sourceStartIndex;
	if (strcmp(firstLine.opcode, "START") == 0)
	{
		startingAddress = atoi(firstLine.operand);
		LOCCTR = startingAddress;
		fprintf(intermediateFile, "%10s %10s %10s", firstLine.label, firstLine.opcode, firstLine.operand);
		sourceStartIndex = 1;
	}
	else
	{
		sourceStartIndex = 0;
	}

	for (int i = sourceStartIndex; i < sourceLength; i++)
	{
		// printf("at line %s", line.label, line.opcode, line.operand);
		struct sourceLine line = sourceCode[i];
		if (strcmp(line.opcode, "END") == 0)
		{
			// DOOOOO
			break;
		}
		else
		{

			if (strcmp(line.label, "") == 0)
			{
				// No Label Present
			}
			else
			{
				if (hasDupEntrySymtab(line.label))
				{
					printf("ERROR: DUPLICATE SYMBOL FOUND. Exitting");
				}
				else
				{
					printf("HELLO");
					strcpy(symtab[symtabIndex].label, line.label);
					symtab[symtabIndex].address = LOCCTR;
					symtabIndex++;
					symtabLength++;
				}
			}
		}
	}
}

int main()
{
	FILE *inputFile = fopen("./input.txt", "r");
	FILE *optabFile = fopen("./optab.txt", "r");
	FILE *symtabFile = fopen("./symtab.txt", "w+");
	FILE *outputFile = fopen("./output.txt", "w+");

	parseOptab(optabFile);
	parseSourceFile(inputFile);

	assemble(symtabFile, outputFile);

	fclose(inputFile);
	fclose(optabFile);
	fclose(symtabFile);
	fclose(outputFile);

	return 0;
}
