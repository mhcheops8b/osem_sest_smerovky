#include <stdio.h>
#include "smery.h"
#include "flosm.h"
#include "slovo.h"
#include "find.h"
#include "osmtop.h"
#include "num_path.h"

int check_in_field(OSEMSM *krz, int pos_x, int pos_y) {
	
	if (!is_in_rect_osm_size (&krz->osm_size, pos_x, pos_y)) {

#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_OUTFIELD_EN);
#else
		fprintf (stderr, MSG_ERR_OUTFIELD_SK);
#endif
		return ERR_CODE_OUTFIELD;
	}
	else
		return IS_IN_FIELD;
}

int
find (OSEMSM * krz, int pos_x, int pos_y, int dir, int count,
      struct pismeno *wrd)
{
	int     pos = 0, match_count = 1, match =
		ERR_CODE_DOESMATCH, new_pos_x, new_pos_y, status;
	struct pismeno pom_pis;

	if ((status=check_in_field(krz, pos_x, pos_y))!=IS_IN_FIELD) 
		return status;
/*	if (!is_in_rect_osm_size (&krz->osm_size, pos_x, pos_y)) {

#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_OUTFIELD_EN);
#else
		fprintf (stderr, MSG_ERR_OUTFIELD_SK);
#endif
		return ERR_CODE_OUTFIELD;

	}*/

	while (match == ERR_CODE_DOESMATCH && match_count < count) {
		pos++;
		new_pos_x = smery_get_dx (dir) + pos_x;
		new_pos_y = smery_get_dy (dir) + pos_y;
		if (!is_in_kriz (krz, new_pos_x, new_pos_y))
			match = ERR_CODE_NOTFOUND;

		else {
			pom_pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pom_pis.l == wrd[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = ERR_CODE_NOTFOUND;
		}
	}
	return match;
}

int
find_2 (OSEMSM * krz, int pos_x, int pos_y, int dir, struct slovo *wrd)
{
	int     pos = 0, match_count = 1, match =
		ERR_CODE_DOESMATCH, new_pos_x, new_pos_y;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_OUTFIELD_EN);
#else
		fprintf (stderr, MSG_ERR_OUTFIELD_SK);
#endif
		return ERR_CODE_OUTFIELD;
	}

	while (match == ERR_CODE_DOESMATCH && match_count < wrd->count) {
		pos++;
		new_pos_x = smery_get_dx (dir) + pos_x;
		new_pos_y = smery_get_dy (dir) + pos_y;
		if (!is_in_kriz (krz, new_pos_x, new_pos_y)
		    || !is_non_void (krz, new_pos_x, new_pos_y)) {
			match = ERR_CODE_NOTFOUND;
		}

		else {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = ERR_CODE_NOTFOUND;
		}
	}
	if (match == ERR_CODE_DOESMATCH)
		return ERR_CODE_DOESMATCH;

	else
		return ERR_CODE_NOTFOUND;
}

int
find_3 (OSEMSM * krz, int pos_x, int pos_y, int dir, struct slovo *wrd,
	int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match = 1, new_pos_x, new_pos_y;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		smery_get_next_pos (pos_x, pos_y, dir, &new_pos_x,
				    &new_pos_y);
		if (!is_in_kriz (krz, new_pos_x, new_pos_y)
		    || !is_non_void (krz, new_pos_x, new_pos_y)) {
			match = 0;
		}

		else {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}

	else
		return 0;
}

int
find_4 (OSEMSM * krz, int pos_x, int pos_y, int dir, int id,
	struct slovo *wrd, int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, new_id;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		smery_get_next_pos (pos_x, pos_y, dir, &new_pos_x,
				    &new_pos_y);
		if (id) {
			new_id = (get_pismeno (krz, new_pos_x, new_pos_y)).id;
			while (id == new_id) {
				pos_x = new_pos_x;
				pos_y = new_pos_y;
				smery_get_next_pos (pos_x, pos_y, dir,
						    &new_pos_x, &new_pos_y);
				if (!is_in_kriz (krz, new_pos_x, new_pos_y))
					new_id = -1;

				else
					new_id = (get_pismeno
						  (krz, new_pos_x,
						   new_pos_y)).id;
			}
		}
		if (!is_in_kriz (krz, new_pos_x, new_pos_y)
		    || !is_non_void (krz, new_pos_x, new_pos_y)) {
			match = 0;
		}

		else {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
				id = pis.id;
			}

			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}

	else
		return 0;
}

int
find_5 (OSEMSM * krz, int pos_x, int pos_y, int dir, struct slovo *wrd,
	int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match = 1, new_pos_x, new_pos_y;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (get_next_word_in_dir
		    (krz, pos_x, pos_y, dir, &new_pos_x, &new_pos_y)) {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = 0;
		}

		else
			match = 0;
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}

	else
		return 0;
}

int
find2 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
       struct slovo *wrd, int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/*
					 * chyba
					 * v
					 * definicii
					 * cesty
					 */
		}
		else {
			if (get_next_word_in_dir
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y)) {
				pis = get_pismeno (krz, new_pos_x, new_pos_y);
				if (pis.l == wrd->pism[pos].l) {
					match_count++;
					pos_x = new_pos_x;
					pos_y = new_pos_y;
					dir_pos++;
				}
				else
					match = 0;
			}
			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}
	else
		return 0;
}

int
find2_1 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/*
					 * chyba
					 * v
					 * definicii
					 * cesty
					 */
		}
		else {
			if (get_next_word_in_dir2
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y)) {
				pis = get_pismeno (krz, new_pos_x, new_pos_y);
				if (pis.l == wrd->pism[pos].l) {
					match_count++;
					pos_x = new_pos_x;
					pos_y = new_pos_y;
					dir_pos++;
				}
				else
					match = 0;
			}
			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}
	else
		return 0;
}

int
find2_2 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {
			if (get_next_word_in_dir2
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y)) {
				pis = get_pismeno (krz, new_pos_x, new_pos_y);
				if (pis.l == wrd->pism[pos].l
				    || pis.l == QUEST_CHAR) {
					if (pis.l == QUEST_CHAR)
						quest2++;
					match_count++;
					pos_x = new_pos_x;
					pos_y = new_pos_y;
					dir_pos++;
				}

				else
					match = 0;
			}

			else
				match = 0;
		}
	}			/* end while */
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		*qest2 = quest2;
		return 1;
	}

	else
		return 0;
}

int
find2_3 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {

/*		 if (get_next_word_in_dir2
			 (krz, pos_x, pos_y, act_dir, &new_pos_x, &new_pos_y)) {*/
			if (get_next_word_in_dir3
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y, DONT_CHECK_UNIQUE_ID)) {
				pis = get_pismeno (krz, new_pos_x, new_pos_y);
				if (compare_pism_to_pism_with_quest
				    (&pis, &wrd->pism[pos])) {

/*pis.l == wrd->pism[pos].l || pis.l == QUEST_CHAR || wrd->pism[pos].l == QUEST_CHAR) {*/
					if (pis.l == QUEST_CHAR)
						quest2++;
					match_count++;
					pos_x = new_pos_x;
					pos_y = new_pos_y;
					dir_pos++;
				}

				else
					match = 0;
			}

			else
				match = 0;
		}
	}			/* end while */
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		*qest2 = quest2;
		return 1;
	}

	else
		return 0;
}

int
find2_4 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
	 struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {
			if (krz->
			    mask[get_pos (krz, pos_x, pos_y)] & (1 <<
								 act_dir)) {
				if (get_next_word_in_dir3
				    (krz, pos_x, pos_y, act_dir, &new_pos_x,
				     &new_pos_y, DONT_CHECK_UNIQUE_ID)) {
					pis = get_pismeno (krz, new_pos_x,
							   new_pos_y);
					if (compare_pism_to_pism_with_quest
					    (&pis, &wrd->pism[pos])) {
						if (pis.l == QUEST_CHAR)
							quest2++;
						match_count++;
						pos_x = new_pos_x;
						pos_y = new_pos_y;
						dir_pos++;
					}

					else
						match = 0;
				}

				else
					match = 0;
			}

			else
				match = 0;
		}
	}			/* end while */
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		*qest2 = quest2;
		return 1;
	}

	else
		return 0;
}


int
find3 (OSEMSM * krz, int pos_x, int pos_y, struct num_path *pth,
       struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2,
       int forb_limit)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0, forb = 0;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	while (match && match_count < wrd->count) {
		pos++;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {
			if (!
			    (krz->
			     mask[get_pos (krz, pos_x, pos_y)] & (1 <<
								  act_dir))) {
				forb++;
			}
#ifdef OSM_DEBUG
			printf("\tActual FORB/LIMIT: %d/%d\n", forb, forb_limit);
#endif
			switch (forb_limit) {
			case FORB_LIMIT_NONE:
				break;
			case FORB_LIMIT_HALF_GT:
				if (forb > wrd->count / 2)
					match = 0;
				break;
			case FORB_LIMIT_HALF_EQ:
				if (forb >= wrd->count / 2)
					match = 0;
				break;
			default:
				if (forb > forb_limit)
					match = 0;
				break;
			}
			if (get_next_word_in_dir3
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y, DONT_CHECK_UNIQUE_ID)) {
				pis = get_pismeno (krz, new_pos_x, new_pos_y);
				if (compare_pism_to_pism_with_quest
				    (&pis, &wrd->pism[pos])) {
					if (pis.l == QUEST_CHAR)
						quest2++;
					match_count++;
					pos_x = new_pos_x;
					pos_y = new_pos_y;
					dir_pos++;
				}

				else
					match = 0;
			}

			else
				match = 0;

/*	  	}
		else
			match = 0;*/
		}
	}			/* end while */
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		*qest2 = quest2;
		return 1;
	}

	else
		return 0;
}
