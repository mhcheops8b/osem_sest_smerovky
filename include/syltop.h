#ifndef _OSM_SYL_TOP_H_
#define _OSM_SYL_TOP_H_

#include "topcomm.h"
#include "flosmsyl.h"

int     get_next_word_in_dir_syl (struct osemsm_syl *krz, int pos_x,
				  int pos_y, int dir_num, int *new_pos_x,
				  int *new_pos_y);

int     get_next_word_in_dir3_syl (OSEMSM_SYL * krz, int pos_x, int pos_y,
				   int dir_num, int *new_pos_x,
				   int *new_pos_y, int check_unique_id);

#endif
