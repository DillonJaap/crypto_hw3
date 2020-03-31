#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct matrix
{
	int16_t  rows;
	int16_t  cols;
	uint8_t** t;
} matrix;

matrix* init_matrix(uint16_t rows, uint16_t cols, uint8_t* data)
{
	// malloc matrix
	matrix* m = malloc(sizeof(m));
	m->rows = rows;
	m->cols = cols;

	// malloc actual table of matrix
	
	m->t = malloc(sizeof(uint8_t*) * rows);
	for (uint16_t i = 0; i < m->rows; i++)
		m->t[i] = malloc(sizeof(uint8_t) * cols);

	// set table to supplied data
	for (int16_t i = 0; i < m->rows; i++)
	{
		for (int16_t j = 0; j < m->cols; j++)
			m->t[i][j] = data ? *(data + ((i * m->rows) + j)) : 0;
	}

	return m;
}

void set_matrix(matrix* m, uint8_t* data)
{
	for (int16_t i = 0; i < m->rows; i++)
	{
		for (int16_t j = 0; j < m->cols; j++)
		{
			m->t[i][j] = data ? *(data + ((i * m->rows) + j)) : 0;
		}
	}
}

void print_matrix(matrix* m)
{
	for (int i = 0; i < m->rows; i++)
	{
		printf("|");
		for (int j = 0; j < m->cols; j++)
			printf("%2.2x ", m->t[i][j]);
		printf("|\n");
	}

}

int sum_Lrow_Rcol(matrix* L, matrix* R, uint16_t Lrow, uint16_t Rcol)
{
	int total = 0;
	for (int i = 0; i < L->rows; i++)
		total += L->t[Lrow][i] * R->t[i][Rcol];
	return total;
}

void mult_matrix(matrix* dest, matrix* L, matrix* R)
{
	set_matrix(dest, NULL);
	for (uint16_t i = 0; i < R->cols; i++)
	{
		for (uint16_t j = 0; j < L->rows; j++)
		{
			dest->t[i][j] = sum_Lrow_Rcol(L, R, i, j);
		}
	}
}	
