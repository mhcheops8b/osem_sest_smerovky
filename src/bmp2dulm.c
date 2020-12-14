#include <stdio.h>
#include "bmp2dulm.h"

int create_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int size_x, int size_y)
{
	if (create_bitmap1d_ulm (&bmp->bmp1d, size_x * size_y) < 0) {
		fprintf (stderr, "Error: Creating bimap.\n");
		return -1;
	}
	bmp->size_x = size_x;
	bmp->size_y = size_y;
	return 0;
}

void free_bitmap2d_ulm (Bitmap_2D_ulm * bmp) {

	free_bitmap1d_ulm (&bmp->bmp1d);
}

void set_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y) {

	set_bit_bitmap1d_ulm (&bmp->bmp1d, n_y * bmp->size_x + n_x);
}

void clear_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y) {

	clear_bit_bitmap1d_ulm (&bmp->bmp1d, n_y * bmp->size_x + n_x);
}

void invert_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y) {

	invert_bit_bitmap1d_ulm (&bmp->bmp1d, n_y * bmp->size_x + n_x);
}

int get_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y) {

	return get_bit_bitmap1d_ulm (&bmp->bmp1d, n_y * bmp->size_x + n_x);
}
