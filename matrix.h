#include <stdint.h>

typedef struct matrix
{
	int16_t  rows;
	int16_t  cols;
	int8_t** t;
} matrix;

matrix* init_matrix(uint16_t rows, uint16_t cols, uint8_t* data);
void set_matrix(matrix* m, uint8_t* data);
void print_matrix(matrix* m);
int sum_Lcol_Rrow(matrix* L, matrix* R, uint16_t Lcol, uint16_t Rrow);
void mult_matrix(matrix* dest, matrix* L, matrix* R);
