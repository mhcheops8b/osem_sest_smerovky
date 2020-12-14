#include <stdio.h>
#include <stdlib.h>

#include "syltop.h"
#include "flosmsyl.h"
#include "smery.h"

int
get_next_word_in_dir_syl (struct osemsm_syl *krz, int pos_x, int pos_y,
			  int dir_num, int *new_pos_x, int *new_pos_y)
{

	int     id, new_id;

	id = (get_syllabe (krz, pos_x, pos_y))->id;
	new_id = id;

	if (id) {
		while (id == new_id) {
			smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x,
					    new_pos_y);
			if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
			    || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
				return 0;

			new_id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->
				id;
			pos_x = *new_pos_x;
			pos_y = *new_pos_y;
		}
		return 1;
	}
	else {
		smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x,
				    new_pos_y);
		if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
		    || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
			return 0;
		return 1;
	}
}
int
get_next_word_in_dir3_syl (OSEMSM_SYL * krz,
			   int pos_x, int pos_y, int dir_num,
			   int *new_pos_x, int *new_pos_y,
			   int check_unique_id)
{

	int     id, new_id, pom_i, pom_b;

	id = (get_syllabe (krz, pos_x, pos_y))->id;
	new_id = id;

	if (id) {
		if (check_unique_id) {
			pom_i = id / 16;
			pom_b = id % 16;
			if ((krz->id[pom_i] & (1 << pom_b)))
				return 0;
			else
				krz->id[pom_i] |= 1 << pom_b;
		}

		while (id == new_id) {
			smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x,
					    new_pos_y);
			if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
			    || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
				return 0;
			new_id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->
				id;
			pos_x = *new_pos_x;
			pos_y = *new_pos_y;
		}
		return 1;
	}
	else {
		smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x,
				    new_pos_y);
		if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
		    || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
			return 0;
		id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->id;
		if (check_unique_id && id
		    && (krz->id[id / 16] & (1 << (id % 16))))
			return 0;
		return 1;
	}
}
