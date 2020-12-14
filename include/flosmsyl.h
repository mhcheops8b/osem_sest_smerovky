#ifndef _OSM_FLOSMSYL_H_
#define _OSM_FLOSMSYL_H_

#include "syllabe.h"
#include "osm_size.h"
#include "bmp2dulm.h"
#include "specchar.h"
#include "q_part.h"

typedef struct osemsm_syl
{
	Osm_size osm_size;
	int     ids;
	Syllabe *pole;
	Bitmap_2D_ulm bitmap;
	struct spec_char *spec;
	unsigned int *id;
	struct q_part *qpart;
	unsigned char *mask;
}
OSEMSM_SYL;

/* Prototypes */
Syllabe *get_syllabe (OSEMSM_SYL * krz, int pos_x, int pos_y);
void    set_syllabe_id (OSEMSM_SYL * krz, int pos_x, int pos_y, int id);
int     get_pos_syl (OSEMSM_SYL * krz, int pos_x, int pos_y);
int     is_in_kriz_syl (OSEMSM_SYL * krz, int pos_x, int pos_y);
int     is_non_void_syl (OSEMSM_SYL * krz, int pos_x, int pos_y);
void    write_sused_syl (struct osemsm_syl *krz, int sx, int sy, int kx,
			 int ky, int dir);
void    free_kriz_syl (OSEMSM_SYL * osm);
void    dump_kriz_syl (struct osemsm_syl *osm);
void    print_kriz3_syl (struct osemsm_syl *osm);
void    select_kriz3_syl (struct osemsm_syl *osm, char *pth,
			  struct slovo *wrd, int sx, int sy, int dr);

/* */
#endif
