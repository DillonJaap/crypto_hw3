#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "matrix.h"

matrix* init_sbox()
{
	uint8_t data[] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
	};

	return init_matrix(16, 16, data);
}

void add_round_key(matrix* block, matrix* key)
{
	// Initial round, xor all the entries in the block and key
	for (uint8_t i = 0; i < block->rows; i++)
	{
		for (uint8_t j = 0; j < block->cols; j++)
			block->t[i][j] ^= key->t[i][j];
	}

}


void sub_bytes(matrix* block, matrix* sbox)
{
	uint8_t col = 0;
	uint8_t row = 0;

	for (uint8_t i = 0; i < block->rows; i++)
	{
		for (uint8_t j = 0; j < block->cols; j++)
		{
			col = block->t[i][j] & 0x0F;
			row = (block->t[i][j] & 0xF0) >> 4;
			block->t[i][j] = sbox->t[row][col];
		}
	}
}


void shift_rows(matrix* block)
{
	rotate_row(block, 0, 0);
	rotate_row(block, 1, 1);
	rotate_row(block, 2, 2);
	rotate_row(block, 3, 3);
}

void mix_cols(matrix* block, matrix* mixcol)
{
	matrix* temp = init_matrix(block->rows, 1, NULL);
	matrix* temp2 = init_matrix(block->rows, 1, NULL);
	
	for (uint8_t i; i < block->cols; i++)
	{
		get_col(temp, block, i);
		mult_matrix(temp2, mixcol, temp);
		set_col(block, temp2, i);
	}
	free(temp);
	free(temp2);
}

void get_keys(matrix* keys[], matrix* rcon, matrix* sbox)
{
	matrix* temp1 = init_matrix(keys[0]->rows, 1, NULL);
	matrix* temp2 = init_matrix(keys[0]->rows, 1, NULL);
	for (int i = 1; i < 11; i++)
	{
		// w1
		get_col(temp1, keys[i-1], 3);
		rotate_col(temp1, 0, 1);
		sub_bytes(temp1, sbox);

		get_col(temp2, keys[i-1], 0);
		add_round_key(temp1, temp2);
		get_col(temp2, rcon, i-1);
		add_round_key(temp1, temp2);

		set_col(keys[i], temp1, 0);

		// w2
		get_col(temp1, keys[i-1], 1);
		get_col(temp2, keys[i], 0);

		add_round_key(temp1, temp2);
		set_col(keys[i], temp1, 1);

		// w3
		get_col(temp1, keys[i-1], 2);
		get_col(temp2, keys[i], 1);

		add_round_key(temp1, temp2);
		set_col(keys[i], temp1, 2);

		// w4
		get_col(temp1, keys[i-1], 3);
		get_col(temp2, keys[i], 2);

		add_round_key(temp1, temp2);
		set_col(keys[i], temp1, 3);
	}
	free(temp1);
	free(temp2);
}

int main(int argc, char** argv)
{
	uint8_t block_data[] = {
		0xd4, 0xe0, 0xb8, 0x1e,
		0xbf, 0xb4, 0x41, 0x27,
		0x5d, 0x52, 0x11, 0x98,
		0x30, 0xae, 0xf1, 0xe5
	};

	uint8_t key_data[] = {
		1, 8, 9, 2,
		6, 2, 2, 3,
		5, 2, 1, 0,
		0, 8, 7, 1
	};

	uint8_t mixcol_data[] = {
		0x02, 0x03, 0x01, 0x01,
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02
	};
	
	uint8_t rcon_data[] = {
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	matrix* keys[11];
	keys[0] = init_matrix(4, 4, key_data);
	for (int i = 1; i < 11; i++) 
		keys[i] = init_matrix(4, 4, NULL);

		
		 
	// init block and key matricies
	matrix* block  = init_matrix(4, 4, block_data);
	
	// other matricies needed for AES
	matrix* mixcol = init_matrix(4, 4, mixcol_data);
	matrix* rcon   = init_matrix(4, 10, rcon_data);
	matrix* sbox   = init_sbox(); // get sbox

	get_keys(keys, rcon, sbox);


	printf("block:\n");
	print_matrix(block);
	printf("\ncypther key:\n");
	print_matrix(keys[0]);

	for (int i = 1; i < 11; i++) 
	{
		printf("\nround key %d:\n", i);
		print_matrix(keys[i]);
	}

	// initial round
	printf("\ninitial round:\n");
	add_round_key(block, keys[0]);
	print_matrix(block);

	// 9 main rounds
	for (int i = 1; i < 10; i++) 
	{
		printf("\nsubbytes:\n");
		sub_bytes(block, sbox);
		print_matrix(block);

		printf("\nshift rows:\n");
		shift_rows(block);
		print_matrix(block);
		
		printf("\nmix cols:\n");
		mix_cols(block, mixcol);
		print_matrix(block);
		printf("\naddroundkey:\n");
		add_round_key(block, keys[i]);
		print_matrix(block);
	}

	// 10th round
	printf("\nsubbytes:\n");
	sub_bytes(block, sbox);
	print_matrix(block);

	printf("\nshift rows:\n");
	shift_rows(block);
	print_matrix(block);
	
	printf("\naddroundkey:\n");
	add_round_key(block, keys[10]);
	print_matrix(block);

	printf("\ncypher block:\n");
	print_matrix(block);

	//printf("result: %x\n", mult(0xca, 0x53));
	return 0;
}

