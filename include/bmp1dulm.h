#ifndef _BITMAP_1D_ULM_H_
#define _BITMAP_1D_ULM_H_

typedef struct _bitmap_1d_ulm {
	unsigned long int *bitmap_data;
	int     bit_size_of_data_elem;
	int     size_of_bitmap;
} Bitmap_1D_ulm;

int     create_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int size_of_bitmap);
void    free_bitmap1d_ulm (Bitmap_1D_ulm * bmp);
void    set_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no);
void    clear_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no);
int     get_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int _bit_no);
void    invert_bit_bitmap1d_ulm (Bitmap_1D_ulm * bmp, int bit_no);

#endif /* ifndef _BITMAP_1D_ULM_H_ */
