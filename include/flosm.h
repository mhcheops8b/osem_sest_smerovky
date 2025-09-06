#ifndef _OSM_FLD_H_
#define _OSM_FLD_H_

#include "osm_size.h"
#include "pismeno.h"
#include "bmp2dulm.h"
#include "specchar.h"
#include "q_part.h"
#include "num_path.h"
#include "slovo.h"

typedef struct osemsm
{
	Osm_size osm_size;
	int     ids;
	struct pismeno *pole;
	Bitmap_2D_ulm bitmap;
	struct spec_char *spec;
	unsigned int *id;
	struct q_part *qpart;
	unsigned char *mask;
}
OSEMSM;

/* Prototypes */

void    get_ul_coords (struct osemsm *osm, int sx, int sy, int id, int *new_x,
		       int *new_y);
void    get_br_coords (struct osemsm *osm, int sx, int sy, int id, int *new_x,
		       int *new_y);
int     is_in_kriz (OSEMSM * krz, int pos_x, int pos_y);
struct pismeno get_pismeno (OSEMSM * krz, int pos_x, int pos_y);
void    set_pismeno (OSEMSM * krz, int pos_x, int pos_y, struct pismeno chr);
int     get_pos (OSEMSM * krz, int pos_x, int pos_y);
int     get_pismeno_id (OSEMSM * krz, int pos_x, int pos_y);
int     get_pismeno_l (OSEMSM * krz, int pos_x, int pos_y);
int     is_non_void (OSEMSM * krz, int pos_x, int pos_y);
void    free_kriz (struct osemsm *osm);
void    select_kriz2 (struct osemsm *osm, struct num_path *pth, int sx,
		      int sy, int num);
void    select_kriz3 (struct osemsm *osm, char *pth, struct slovo *wrd,
		      int sx, int sy, int dr);
void	select_kriz4(struct osemsm* osm, char* pth, int word_len,
	int sx, int sy, int dr);
void    write_sused (struct osemsm *krz, int sx, int sy, int kx, int ky,
		     int dir);
void    print_kriz2 (struct osemsm *osm);
void    print_kriz3 (struct osemsm *osm);
void    print_kriz3_log (struct osemsm *osm, FILE * fou);
void    print_kriz3_log2 (struct osemsm *osm, FILE * fou);
void    replace_id (struct osemsm *osm, int sx, int sy, int val);
void print_masks (OSEMSM * krz);

/* */
#endif
