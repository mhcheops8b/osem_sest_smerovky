#ifndef _OSM_FIND_H_
#define _OSM_FIND_H_

#include "findcomm.h"

/* Prototypes */

int     find (OSEMSM * krz, int pos_x, int pos_y, int dir, int count,
	      struct pismeno *wrd);

int     find_2 (OSEMSM * krz, int pos_x, int pos_y, int dir,
		struct slovo *wrd);

int     find_3 (OSEMSM * krz, int pos_x, int pos_y, int dir,
		struct slovo *wrd, int *kon_pos_x, int *kon_pos_y);

int     find_4 (OSEMSM * krz, int pos_x, int pos_y, int dir, int id,
		struct slovo *wrd, int *kon_pos_x, int *kon_pos_y);

int     find_5 (OSEMSM * krz, int pos_x, int pos_y, int dir,
		struct slovo *wrd, int *kon_pos_x, int *kon_pos_y);

int     find2 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	       struct slovo *wrd, int *kon_pos_x, int *kon_pos_y);

int     find2_1 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
		 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y);

int     find2_2 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
		 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y,
		 int *qest2);

int     compare_pism_to_pism_with_quest (struct pismeno *p1,
					 struct pismeno *p2);

int     find2_3 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
		 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y,
		 int *qest2);

int     find2_4 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
		 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y,
		 int *qest2);

int     find3 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	       struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2,
	       int forb_limit);

/* */
#endif
