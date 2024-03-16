
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SIZE 100


void helloUser(void);
void commandSML(float mem[], int* const exe, float* const accPtr, int* const opCode,
	int* const numOperand, int* const iCount);
void registers(const int* const iReg, const float* const accPtr,
	const int* const opCode, const int* const numOperand, const int* const iCount);
void memoryDump(const float memory[]);

int main(void) {

	// Образцы программ

	// 1. Сумма двух чисел
	//float memory[SIZE] = { 1007, 1008, 2007, 3008, 2109, 1109, 4300, 0000, 0000, 0000, };
	// 2. Деление А на В
	//float memory[SIZE] = { 1007, 1008, 2007, 3208, 2109, 1109, 4300, 0000, 0000, 0000, };
	// 3. Выводит наименьшее из введенных чисел, причем первым параметром задается количество
	// вводимых чисел
	/*
	float memory[SIZE] = { 1021, 2021, 3120, 2121, 1018, 4216, 1019, 2018, 3119, 4112,
						2019, 2118, 2021, 3120, 2121, 4005, 1118, 4300, 0000, 0000,
						0001, 0000, };*/
	// 4. Модуль х 
	//float memory[SIZE] = { 1006, 2006, 3400, 2107, 1107, 4300, 0000, 0000,};
	// 5. Возведение числа в степень
	//float memory[SIZE] = { 1007, 1008, 2007, 3508, 2109, 1109, 4300, 0000, 0000, 0000};
	// 6. Тест перехода на новую строку
	//float memory[SIZE] = { 1008, 1200, 1009, 2008, 3509, 2110, 1110, 4300, 0000, 0000, 0000 };
	// 7. Программа ввода строки 
	//float memory[SIZE] = { 1305, 1400, 4300};
	// end of examples

	//int memory[100] = { 0 };
	float accumulator = +0000;
	int instructionCounter = 00;
	int instructionRegister = +0000;
	int operationCode = 00;
	int operand = 00;
	int execution = 1;

	helloUser();

	printf("\n***Enter your program***\n");

	//Вводим программу в массив memory вручную.
	float memory[SIZE] = { 0 };
	for (instructionCounter; instructionRegister != -99999; instructionCounter++) {

		printf("%02d? ", instructionCounter);
		scanf("%d", &instructionRegister);

		while (instructionRegister != -99999 && (instructionRegister < -9999 || instructionRegister > 9999)) {

			printf("Incorrect data! Please, enter correct word\n");
			printf("%02d? ", instructionCounter);
			scanf("%d", &instructionRegister);

		}
		memory[instructionCounter] = instructionRegister;

	}

	memory[instructionCounter - 1] = 0;
	

	printf("***Program loading complete***\n"
		"***Starting execution program***\n\n");


	//Исполнение программы
	for (instructionCounter = 00; instructionRegister != 4300; instructionCounter++) {

		instructionRegister = memory[instructionCounter];
		operationCode = instructionRegister / 100;
		operand = instructionRegister % 100;
		commandSML(memory, &execution, &accumulator, &operationCode,
			&operand, &instructionCounter);

		if (!execution)
			break;

		if (accumulator < -9999 || accumulator > 9999) {

			printf("\n***Accumulator is overflow. Error in memory cell #%d***\n",
				instructionCounter);
			printf("***Simpletron has terminated the program execution in an emergency***\n");
			break;

		}

	}


	putchar(memory[9]);

	registers(&instructionRegister, &accumulator, &operationCode, &operand, &instructionCounter);
	memoryDump(memory);
	
	return 0;

}

void helloUser(void) {

	printf("***The Sympletron welcomes you***\n"
		"***Please, enter your program, by single command***\n"
		"***(or data word) at a 1 time. I will print as***\n"
		"***help current address and sign (?). Entered***\n"
		"***by you word will be placed by specified address***\n"
		"***To interrupt program's input enter value -99999***\n");

}


void commandSML(float mem[], int* const exe, float* const accPtr, int* const opCode,
	int* const numOperand, int* const iCount) {

	int count;	
	int cell;
	int symbol;
	int startStr;

	switch (*opCode) {

	case +10: // Read value
		printf("Read: ");
		scanf("%f", &mem[*numOperand]);
		break;
	case +11: // Write value
		printf("Write: %.2f\n", mem[*numOperand]);
		break;
	case +12: // Enter new line
		puts("");
		break;
	case +13: // Read string
		count = *numOperand;
		cell = 00;
		for (int i = 0; mem[i] != 4300; i++)
			cell++;		

		for (count; count > 0; count--) {
			
			printf("Enter symbol code in memory cell %02d (Remain %d symbols to finish word): ", ++cell, count);
			scanf("%d", &symbol);					
			mem[cell] = symbol;
			
		}			
		break;
	case +14: // Write string
				
		count = 0;
		for (int i = 0; mem[i] != 4300; i++)
			count++;

		printf("WriteStr: ");
		for (++count; mem[count] != 00; count++) {

			if (mem[count] < 28)
				mem[count] += 100;
			putchar(mem[count]);

		}
		puts("");
		break;
	case +20: // Load to accumulator
		*accPtr = mem[*numOperand];
		//printf("\nLoaded: %f\n", *accPtr);
		break;
	case +21: // Store from accumulator to address
		mem[*numOperand] = *accPtr;
		//printf("\nStored: %f\n", *accPtr);
		break;
	case +30: // Accumulator + value (save in acc)
		*accPtr += mem[*numOperand];
		break;
	case +31: // Accumulator - value (save in acc)
		*accPtr -= mem[*numOperand];
		break;
	case +32: // Divide acc by value
		if (mem[*numOperand] == 0) {

			printf("\n***Attempt to divide by zero. Error in memory cell #%d***\n",
				*iCount);
			printf("***Simpletron has terminated the program execution in an emergency***\n");
			*exe = 0;

		}
		*accPtr /= mem[*numOperand];
		break;
	case +33: // Multiply acc by value
		*accPtr *= mem[*numOperand];
		break;
	case +34: // Abs x
		*accPtr = fabs(*accPtr);
		break;
	case +35: // x power y
		*accPtr = pow(*accPtr, mem[*numOperand]);
		break;
	case +40: // Transition to specified address
		*iCount = *numOperand - 1;
		break;
	case +41: // Transition to specified address if acc<0
		if (*accPtr < 0)
			*iCount = *numOperand - 1;
		break;
	case +42: // Transition to specified address if acc=0
		if (*accPtr == 0)
			*iCount = *numOperand - 1;
		break;
	case +43:
		printf("\n***Simpletron has finished its calculations***\n");
		break;
	default:
		printf("\n***Unknown operation code entered. Error in memory cell #%d***\n",
			*iCount);
		printf("***Simpletron has terminated the program execution in an emergency***\n");
		*exe = 0;
	}



}


void registers(const int* const iReg, const float* const accPtr,
	const int* const opCode, const int* const numOperand, const int* const iCount) {

	printf("\n   REGISTERS:\n");
	printf("   %-20s%+08.2f\n", "accumulator", *accPtr);
	printf("   %-24s%4d\n", "instructionCounter", *iCount - 1);
	printf("   %-23s%+05d\n", "instructionRegister", *iReg);
	printf("   %-24s  %02d\n", "operationCode", *opCode);
	printf("   %-24s  %02d\n", "operand", *numOperand);
	puts("\n");

}

void memoryDump(const float memory[]) {

	printf("   MEMORY:\n");
	printf("   ");
	for (int i = 0; i < 10; i++) {

		printf("%10i", i);

	}
	puts("");

	for (int i = 0; i < SIZE / 10; i++) {

		printf("%3i", 10 * i);

		for (int j = 0; j < 10; j++) {

			printf("  ");
			printf("%+08.2f", memory[10 * i + j]);

		}
		puts("");
	}
}

