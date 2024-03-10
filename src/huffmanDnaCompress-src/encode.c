#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "simprob_utils.h"
#include "probabilidad.h"
#include "huffman_tree.h"
#include "huffman.h"
#include "number_utils.h"
#include "coder.h"
#include "string_utils.h"
#include "string_analyzer.h"
#include "string_list.h"
#define BYTES_PER_CHAR 8
char* huffmanSimple(char *input);
char* huffmanComplex(char *input, char *fileName);
int checkInput(char *input);
int checkChar(char character);
int main(int argc, char *argv[])
{
	char *fileName = NULL;
	int c;
	while((c = getopt(argc, argv,"hf:")) != -1){
		switch(c)
		{
			case 'f':
				fileName = argv[2];
				break;
			case 'h':
			default:
				printf("Usage: encode -f fileToCompress\n");
				return -1;
		}
	}
	if(fileName == NULL){
		printf("Usage: encode -f fileToCompress\n");
	}
	FILE *file = fopen(fileName, "r");
	if(file == NULL){
		printf("Error opening file %s.\n", fileName);
		return -1;
	}

	char *input = NULL;
	size_t len = 0;
	getline(&input, &len, file);
	input[strlen(input) - 1] = '\0'; //Eliminar retorno de carro final
	fclose(file);
	if (!checkInput(input)){
		printf("Entry error. Only symbols 'A', 'T', 'C' and 'G' are valid.\n");
		return -1;
	}

	char *binSimple = huffmanSimple(input);
	char *binComplex = huffmanComplex(input, fileName);
	int sizeHuffmanSimple = strlen(binSimple);
	int sizeHuffmanComplex = strlen(binComplex);
	int sizeNoCompress = strlen(input) * BYTES_PER_CHAR;
	double rcSimple = (double)sizeNoCompress / (double)sizeHuffmanSimple;
	double rcComplex = (double)sizeNoCompress / (double)sizeHuffmanComplex;
	// printf("Compression ratio for the simple algorithm: %d/%d=%.3f\n",sizeNoCompress, sizeHuffmanSimple, rcSimple);
	printf("Compression ratio for the algorithm: %d/%d=%.3f\n",sizeNoCompress, sizeHuffmanComplex, rcComplex);

	free(binSimple);
	free(binComplex);
	free(input);
	return 0;
}

int checkInput(char *input)
{
int check = 0, i;
	if(input != NULL){
		for(i = 0; i < strlen(input); i++){
			check = checkChar(input[i]);
			if(check == 0){
				break;
			}
			check = 1;
		}
	}
	return check;
}

int checkChar(char character)
{
	return character == ADENINE || character == GUANINE || character == CYTOSINE || character == THYMINE;
}





	//"ACCTTCATACCTCACAACCTTACGACCT";
	//"CTCGAGGGGCCTAGACATTGCCCTCCAGAGAGAGCACCCAACACCCTCCAGGCTTGACCGGCCAGGGTGTCCCCTTCCTACCTTGGAGAGAGCAGCCCCAGGGCATCCTGCAGGGGGTGCTGGGACACCAGCTGGCCTTCAAGGTCTCTGCCTCCCTCCAGCCACCCCACTACACGCTGCTGGGATCCTGGATCTCAGCTCCCTGGCCGACAACACTGGCAAACTCCTACTCATCCACGAAGGCCCTCCTGGGCATGGTGGTCCTTCCCAGCCTGGCAGTCTGTTCCTCACACACCTTGTTAGTGCCCAGCCCCTGAGGTTGCAGCTGGGGGTGTCTCTGAAGGGCTGTGAGCCCCCAGGAAGCCCTGGGGAAGTGCCTGCCTTGCCTCCCCCCGGCCCTGCCAGCGCCTGGCTCTGCCCTCCTACCTGGGCTCCCCCCATCCAGCCTCCCTCCCTACACACTCCTCTCAAGGAGGCACCCATGTCCTCTCCAGCTGCCGGGCCTCAGAGCACTGTGGCGTCCTGGGGCAGCCACCGCATGTCCTGCTGTGGCATGGCTCAGGGTGGAAAGGGCGGAAGGGAGGGGTCCTGCAGATAGCTGGTGCCCACTACCAAACCCGCTCGGGGCAGGAGAGCCAAAGGCTGGGTGTGTGCAGAGCGGCCCCGAGAGGTTCCGAGGCTGAGGCCAGGGTGGGACATAGGGATGCGAGGGGCCGGGGCACAGGATACTCCAACCTGCCTGCCCCCATGGTCTCATCCTCCTGCTTCTGGGACCTCCTGATCCTGCCCCTGGTGCTAAGAGGCAGGTAAGGGGCTGCAGGCAGCAGGGCTCGGAGCCCATGCCCCCTCACCATGGGTCAGGCTGGACCTCCAGGTGCCTGTTCTGGGGAGCTGGGAGGGCCGGAGGGGTGTACCCCAGGGGCTCAGCCCAGATGACACTATGGGGGTGATGGTGTCATGGGACCTGGCCAGGAGAGGGGAGATGGGCTCCCAGAAGAGGAGTGGGGGCTGAGAGGGTGCCTGGGGGGCCAGGACGGAGCTGGGCCAGTGCACAGCTTCCCACACCTGCCCACCCCCAGAGTCCTGCCGCCACCCCCAGATCACACGGAAGATGAGGTCCGAGTGGCCTGCTGAGGACTTGCTGCTTGTCCCCAGGTCCCCAGGTCATGCCCTCCTTCTGCCACCCTGGGGAGCTGAGGGCCTCAGCTGGGGCTGCTGTCCTAAGGCAGGGTGGGAACTAGGCAGCCAGCAGGGAGGGGACCCCTCCCTCACTCCCACTCTCCCACCCCCACCACCTTGGCCCATCCATGGCGGCATCTTGGGCCATCCGGGACTGGGGACAGGGGTCCTGGGGACAGGGGTCCGGGGACAGGGTCCTGGGGACAGGGGTGTGGGGACAGGGGTCTGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTCTGGGGACAGGGGTGTGGGGACAGGGGTCCGGGGACAGGGGTGTGGGGACAGGGGTCTGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTCTGGGGACAGGGGTGTGGGGACAGGGGTCCTGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTCCTGGGGATAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTCCCGGGGACAGGGGTGTGGGGACAGGGGTGTGGGGACAGGGGTCCTGGGGACAGGGGTCTGAGGACAGGGGTGTGGGCACAGGGGTCCTGGGGACAGGGGTCCTGGGGACAGGGGTCCTGGGGACAGGGGTCTGGGGACAGCAGCGCAAAGAGCCCCGCCCTGCAGCCTCCAGCTCTCCTGGTCTAATGTGGAAAGTGGCCCAGGTGAGGGCTTTGCTCTCCTGGAGACATTTGCCCCCAGCTGTGAGCAGGGACAGGTCTGGCCACCGGGCCCCTGGTTAAGACTCTAATGACCCGCTGGTCCTGAGGAAGAGGTGCTGACGACCAAGGAGATCTTCCCACAGACCCAGCACCAGGGAAATGGTCCGGAAATTGCAGCCTCAGCCCCCAGCCATCTGCCGACCCCCCCACCCCGCCCTAATGGGCCAGGCGGCAGGGGTTGACAGGTAGGGGAGATGGGCTCTGAGACTATAAAGCCAGCGGGGGCCCAGCAGCCCTCAGCCCTCCAGGACAGGCTGCATCAGAAGAGGCCATCAAGCAGGTCTGTTCCAAGGGCCTTTGCGTCAGGTGGGCTCAGGGTTCCAGGGTGGCTGGACCCCAGGCCCCAGCTCTGCAGCAGGGAGGACGTGGCTGGGCTCGTGAAGCATGTGGGGGTGAGCCCAGGGGCCCCAAGGCAGGGCACCTGGCCTTCAGCCTGCCTCAGCCCTGCCTGTCTCCCAGATCACTGTCCTTCTGCCATGGCCCTGTGGATGCGCCTCCTGCCCCTGCTGGCGCTGCTGGCCCTCTGGGGACCTGACCCAGCCGCAGCCTTTGTGAACCAACACCTGTGCGGCTCACACCTGGTGGAAGCTCTCTACCTAGTGTGCGGGGAACGAGGCTTCTTCTACACACCCAAGACCCGCCGGGAGGCAGAGGACCTGCAGGGTGAGCCAACCGCCCATTGCTGCCCCTGGCCGCCCCCAGCCACCCCCTGCTCCTGGCGCTCCCACCCAGCATGGGCAGAAGGGGGCAGGAGGCTGCCACCCAGCAGGGGGTCAGGTGCACTTTTTTAAAAAGAAGTTCTCTTGGTCACGTCCTAAAAGTGACCAGCTCCCTGTGGCCCAGTCAGAATCTCAGCCTGAGGACGGTGTTGGCTTCGGCAGCCCCGAGATACATCAGAGGGTGGGCACGCTCCTCCCTCCACTCGCCCCTCAAACAAATGCCCCGCAGCCCATTTCTCCACCCTCATTTGATGACCGCAGATTCAAGTGTTTTGTTAAGTAAAGTCCTGGGTGACCTGGGGTCACAGGGTGCCCCACGCTGCCTGCCTCTGGGCGAACACCCCATCACGCCCGGAGGAGGGCGTGGCTGCCTGCCTGAGTGGGCCAGACCCCTGTCGCCAGCCTCACGGCAGCTCCATAGTCAGGAGATGGGGAAGATGCTGGGGACAGGCCCTGGGGAGAAGTACTGGGATCACCTGTTCAGGCTCCCACTGTGACGCTGCCCCGGGGCGGGGGAAGGAGGTGGGACATGTGGGCGTTGGGGCCTGTAGGTCCACACCCAGTGTGGGTGACCCTCCCTCTAACCTGGGTCCAGCCCGGCTGGAGATGGGTGGGAGTGCGACCTAGGGCTGGCGGGCAGGCGGGCACTGTGTCTCCCTGACTGTGTCCTCCTGTGTCCCTCTGCCTCGCCGCTGTTCCGGAACCTGCTCTGCGCGGCACGTCCTGGCAGTGGGGCAGGTGGAGCTGGGCGGGGGCCCTGGTGCAGGCAGCCTGCAGCCCTTGGCCCTGGAGGGGTCCCTGCAGAAGCGTGGCATTGTGGAACAATGCTGTACCAGCATCTGCTCCCTCTACCAGCTGGAGAACTACTGCAACTAGACGCAGCCTGCAGGCAGCCCCACACCCGCCGCCTCCTGCACCGAGAGAGATGGAATAAAGCCCTTGAACCAGCCCTGCTGTGCCGTCTGTGTGTCTTGGGGGCCCTGGGCCAAGCCCCACTTCCCGGCACTGTTGTGAGCCCCTCCCAGCTCTCTCCACGCTCTCTGGGTGCCCACAGGTGCCAACGCCGGCCAGGCCCAGCATGCAGTGGCTCTCCCCAAAGCGGCCATGCCTGTTGGCTGCCTGCTGCCCCCACCCTGTGGCTCAGGGTCCAGTATGGGAGCTTCGGGGGTCTCTGAGGGGCCAGGGATGGTGGGGCCACTGAGAAGTGACTTCTTGTTCAGTAGCTCTGGACTCTTGGAGTCCCCAGAGACCTTGTTCAGGAAAGGGAATGAGAACATTCCAGCAATTTTCCCCCCACCTAGCCCTCCCAGGTTCTATTTTTAGAGTTATTTCTGATGGAGTCCCTGTGGAGGGAGGAGGCTGGGCTGAGGGAGGGGGT";
	//"ATCCCCATCCCCATCCCCATCCCCATCCCCATCCCC";


char* huffmanSimple(char *input)
{
	char *alphabet[] = {"A", "T", "C", "G"};
	double *symbolFrequency = charFreq(input, "ATCG");
	HuffmanTable *table = huffman(symbolFrequency, alphabet, 4);
	printf("Huffman table for the simple algorithm:\n");
	printHuffmanTable(table);
	char *bin = getBinaryString(input, table);
	char* original = getOriginalMessage(bin, table);
	if(strcmp(input, original) != 0){
		printf("There are errors compressing and decompressing the string using simple Huffman.\n");
	} 
	// printf("%zu bits occupied when compressing using simple algorithm.\n\n", strlen(bin));
	freeHuffmanTable(table);
	free(original);
	free(symbolFrequency);
	return bin;
}

char* huffmanComplex(char *input, char *fileName)
{
	char *tableFileName = concat(fileName, "_table");
	char *binFileName = concat(fileName, "_hbin");
	char *alphabet = "ATCG";
	StringValueList *symbols = groupSymbols(input, alphabet);
	HuffmanTable *table = huffman(symbols->value, symbols->string, symbols->length);
	exportHuffmanTableToFile(table, tableFileName);
	printf("Huffman table for the complex algotithm:\n");
	printHuffmanTable(table);
	char *bin = getBinaryString(input, table);
	FILE *file = fopen(binFileName, "w");
	if (file != NULL){
		fputs(bin, file);
		fclose(file);
	}
	char* original = getOriginalMessage(bin, table);
	if(strcmp(input, original) != 0){
		printf("There are errors compressing and decomressing the string.\n");
	} 
	// printf("%zu bits occupied when compressing using complex algorithm.\n\n", strlen(bin));
	freeStringValueList(symbols);
	freeHuffmanTable(table);
	free(original);
	free(tableFileName);
	free(binFileName);
	return bin;
}