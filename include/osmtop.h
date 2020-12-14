#ifndef _OSM_OSM_TOP_H_
#define _OSM_OSM_TOP_H_

#include "topcomm.h"

/* Prototypes */
int     get_next_word_in_dir (struct osemsm *krz, int pos_x, int pos_y,
			      int dir_num, int *new_pos_x, int *new_pos_y);

int     get_next_word_in_dir2 (struct osemsm *krz, int pos_x, int pos_y,
			       int dir_num, int *new_pos_x, int *new_pos_y);

int     get_next_word_in_dir3 (struct osemsm *krz, int pos_x, int pos_y,
			       int dir_num, int *new_pos_x, int *new_pos_y,
			       int check_unique_id);

int     get_next_word_in_dir4 (struct osemsm *krz, int pos_x, int pos_y,
			       int dir_num, int *new_pos_x, int *new_pos_y,
			       int check_unique_id, int check_emptyness);

/* */
#endif
