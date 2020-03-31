#include <stdint.h>

typedef struct matrix
{
	int8_t  rows;
	int8_t  cols;
	int8_t** t;
} matrix;

matrix* init_matrix(uint8_t rows, uint8_t cols, uint8_t* data);
void free_matrix(matrix* m);
void set_matrix(matrix* m, uint8_t* data);
void print_matrix(matrix* m);
uint8_t sum_Lrow_Rcol(matrix* L, matrix* R, uint8_t Lrow, uint8_t Rcol);
uint8_t mult(uint8_t a, uint8_t b);
void mult_matrix(matrix* dest, matrix* L, matrix* R);

void get_col(matrix* dest, matrix* src, uint8_t col);
void set_col(matrix* dest, matrix* src, uint8_t col);
void rotate_row(matrix* m, uint8_t row, uint8_t n);
void rotate_col(matrix* m, uint8_t col, uint8_t n);
