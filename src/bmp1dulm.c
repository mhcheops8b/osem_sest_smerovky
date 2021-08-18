#include <stdio.h>
#include <stdlib.h>
#include "bmp1dulm.h"

int create_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int size_of_bitmap) {
	int     data_size, pom;

	bmp->size_of_bitmap = size_of_bitmap;
	bmp->bit_size_of_data_elem = 8 * sizeof (unsigned long int);
	data_size = size_of_bitmap / bmp->bit_size_of_data_elem +
		(size_of_bitmap % bmp->bit_size_of_data_elem ? 1 : 0);
	data_size++;

	bmp->bitmap_data = (unsigned long int *) malloc (data_size * sizeof (unsigned long int));
	if (bmp->bitmap_data == NULL) {
		fprintf (stderr, "Error: Could not allocate memory(create_bitmap_1d_uml).\n");
		return -1;
	}

	for (pom = 0; pom < data_size; pom++)
		bmp->bitmap_data[pom] = 0;
	return 0;
}

void free_bitmap1d_ulm (Bitmap_1D_ulm * bmp) {

	if (bmp->bitmap_data)
		free ((void *) bmp->bitmap_data);

	bmp->bit_size_of_data_elem = 0;
	bmp->size_of_bitmap = 0;
	bmp->bitmap_data = NULL;
}

void set_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no) {

	int     data_no, bit_no;

	data_no = _bit_no / bmp->bit_size_of_data_elem;
	bit_no = _bit_no % bmp->bit_size_of_data_elem;
	bmp->bitmap_data[data_no] |= (unsigned long int)1 << bit_no;
}

void clear_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no) {
	
	int     data_no, bit_no;

	data_no = _bit_no / bmp->bit_size_of_data_elem;
	bit_no = _bit_no % bmp->bit_size_of_data_elem;
	bmp->bitmap_data[data_no] &= ~((unsigned long int)1 << bit_no);
}

int get_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no) {

	int     data_no, bit_no;

	data_no = _bit_no / bmp->bit_size_of_data_elem;
	bit_no = _bit_no % bmp->bit_size_of_data_elem;

	return (bmp->bitmap_data[data_no] & (unsigned long int)1 << bit_no) >> bit_no;
}

void invert_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int bit_no) {

	if (get_bit_bitmap1d_ulm (bmp, bit_no))
		clear_bit_bitmap1d_ulm (bmp, bit_no);
	else
		set_bit_bitmap1d_ulm (bmp, bit_no);
}
