#ifndef _OSM_SYL_FIND_H_
#define _OSM_SYL_FIND_H_

#include "flosmsyl.h"
#include "num_path.h"
#include "slovo.h"
#include "syllabe.h"
#include "findcomm.h"

/* prototypes */

int     find3_syl (OSEMSM_SYL * krz, int pos_x, int pos_y,
		   struct num_path *pth, struct slovo *wrd, int *kon_pos_x,
		   int *kon_pos_y, int *qest2, int forb_limit);

int     compare_syllabe_to_syllabe (Syllabe * syl_1, Syllabe * syl_2);

int     compare_word_to_syllabe (struct slovo *wrd, int wrd_from_position,
				 Syllabe * syl);

int     find2_3_syl (OSEMSM_SYL * krz, int pos_x, int pos_y,
		     struct num_path *pth, struct slovo *wrd, int *kon_pos_x,
		     int *kon_pos_y, int *qest2);

/* */
#endif
