#ifndef _BITMAP_1D_UL_H_
#define _BITMAP_1D_UL_H_
typedef struct _bitmap1d_ul
{
	unsigned long bitmap_data;
	int     bit_size_of_data;
	int     size_of_bitmap;
}
Bitmap1D_ul;
int     create_bitmap_bitmap1d_ul (Bitmap1D_ul * bmp, int size_of_bitmap);
void    set_bit_bitmap1d_ul (Bitmap1D_ul * bmp, int bit_no);
void    clear_bit_bitmap1d_ul (Bitmap1D_ul * bmp, int bit_no);
void    invert_bit_bitmap1d_ul (Bitmap1D_ul * bmp, int bit_no);
int     get_bit_bitmap1d_ul (Bitmap1D_ul * bmp, int bit_no);

#endif /*  */
