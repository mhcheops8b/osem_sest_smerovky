#ifndef _BITMAP_2D_ULM_H_
#define _BITMAP_2D_ULM_H_

#include "bmp1dulm.h"
typedef struct _bitmap_2d_ulm {
	int     size_x, size_y;
	Bitmap_1D_ulm bmp1d;
} Bitmap_2D_ulm;

int     create_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int size_x, int size_y);
void    free_bitmap2d_ulm (Bitmap_2D_ulm * bmp);
void    set_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y);
void    clear_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y);
void    invert_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y);
int     get_bit_bitmap2d_ulm (Bitmap_2D_ulm * bmp, int n_x, int n_y);

#endif /* ifndef _BITMAP_2D_ULM_H_ */
