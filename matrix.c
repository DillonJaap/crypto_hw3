#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "matrix.h"


matrix* init_matrix(uint8_t rows, uint8_t cols, uint8_t* data)
{
	// malloc matrix
	matrix* m = malloc(sizeof(m));
	m->rows = rows;
	m->cols = cols;

	// malloc actual table of matrix
	
	m->t = malloc(sizeof(uint8_t*) * rows);
	for (uint8_t i = 0; i < m->rows; i++)
		m->t[i] = malloc(sizeof(uint8_t) * cols);

	// set table to supplied data
	for (uint8_t i = 0; i < m->rows; i++)
	{
		for (uint8_t j = 0; j < m->cols; j++)
			m->t[i][j] = data ? *(data + ((i * m->rows) + j)) : 0;
	}

	return m;
}

void free_matrix(matrix* m)
{
	for (uint8_t i = 0; i < m->rows; i++)
		free(m->t[i]);
	free(m->t);
	free(m);
}

void set_matrix(matrix* m, uint8_t* data)
{
	for (uint8_t i = 0; i < m->rows; i++)
	{
		for (uint8_t j = 0; j < m->cols; j++)
			m->t[i][j] = data ? *(data + ((i * m->cols) + j)) : 0;
	}
}

void print_matrix(matrix* m)
{
	for (uint8_t i = 0; i < m->rows; i++)
	{
		printf("|");
		for (uint8_t j = 0; j < m->cols; j++)
			printf("%#04hhx ", (unsigned)m->t[i][j]);
		printf("|\n");
	}
	printf("\n");

}

uint8_t mult(uint8_t a, uint8_t b)
{
	uint8_t result = 0;

	//  Rusian peasent algo for mult
	//  loop until either a or b is 0
	while (a && b)
	{
		// if b is odd add a to the sum
		if (b & 1)
			result ^= a;

		if (a & 0x80) // a is going to overflow when shifted, must mod
			a = ((a << 1) ^ 0x11b); // 0x11b is the prime polynomial
		else
			a <<= 1;
		b >>= 1;
	}

	//printf("(result: %x)", result);
	return result;
}


uint8_t sum_Lrow_Rcol(matrix* L, matrix* R, uint8_t Lrow, uint8_t Rcol)
{
	uint8_t total = 0;
	for (uint8_t i = 0; i < L->rows; i++)
	{
		total ^= mult(L->t[Lrow][i], R->t[i][Rcol]);
		//printf(" %#04hhx*%#04hhx +",L->t[Lrow][i], R->t[i][Rcol]);
	}
	//printf("\b= %#04hhx\n", total);
	return total;
}

void mult_matrix(matrix* dest, matrix* L, matrix* R)
{
	for (uint8_t i = 0; i < L->rows; i++)
	{
		for (uint8_t j = 0; j < R->cols; j++)
		{
			dest->t[i][j] = sum_Lrow_Rcol(L, R, i, j);
		}
	}
}	

void get_col(matrix* dest, matrix* src, uint8_t col)
{
	uint8_t data[src->rows]; 
	for (uint8_t i = 0; i < src->rows; i++)
		data[i] = src->t[i][col];
	set_matrix(dest, data);
}

void set_col(matrix* dest, matrix* src, uint8_t col)
{
	for (uint8_t i = 0; i < dest->rows; i++)
		dest->t[i][col] = src->t[i][0];
}


void rotate_row(matrix* m, uint8_t row, uint8_t n)
{
	uint8_t temp = 0;
	uint8_t index = 0;

	for (uint8_t i = 0; i < n; i++)
	{
		temp = m->t[row][0];

		for (uint8_t j = 0; j < m->cols - 1; j++)
			m->t[row][j] = m->t[row][j+1];

		m->t[row][m->cols - 1] = temp;
	}

}

void rotate_col(matrix* m, uint8_t col, uint8_t n)
{
	uint8_t temp = 0;
	uint8_t index = 0;

	for (uint8_t i = 0; i < n; i++)
	{
		temp = m->t[0][col];

		for (uint8_t j = 0; j < m->rows - 1; j++)
			m->t[j][col] = m->t[j+1][col];

		m->t[m->rows - 1][col] = temp;
	}

}
