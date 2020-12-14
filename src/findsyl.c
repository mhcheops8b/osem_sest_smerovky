#include <stdio.h>
#include <stdlib.h>

#include "findsyl.h"
#include "flosmsyl.h"
#include "syllabe.h"
#include "syltop.h"
#include "pismeno.h"

int
find3_syl (OSEMSM_SYL * krz, int pos_x, int pos_y, struct num_path *pth,
	   struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2,
	   int forb_limit)
{
	int     pos = 0, match_count = 0, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0, forb = 0;
	Syllabe *syl;

	if (!is_in_kriz_syl (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	syl = get_syllabe (krz, pos_x, pos_y);
	match_count = syl->slv.count;
	while (match && match_count < wrd->count) {
		pos += syl->slv.count;

#ifdef OSM_DEBUG
		printf ("pos: %d\n", pos);

#endif /*  */
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {
			if (!
			    (krz->
			     mask[get_pos_syl (krz, pos_x, pos_y)] & (1 <<
								      act_dir)))
			{
				forb++;
			}
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
			if (get_next_word_in_dir3_syl
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y, DONT_CHECK_UNIQUE_ID)) {
				syl = get_syllabe (krz, new_pos_x, new_pos_y);

#ifdef OSM_DEBUG
				printf ("Syllabe: ");
				print_slovo (&syl->slv);
				printf ("\n");

#endif /*  */
				if (compare_word_to_syllabe (wrd, pos, syl)) {

/*				if (pis.l == QUEST_CHAR)
			  		quest2++;*/
					match_count += syl->slv.count;
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


int
compare_syllabe_to_syllabe (Syllabe * syl_1, Syllabe * syl_2)
{
	return compare_word_to_syllabe (&syl_1->slv, 0, syl_2);
}

int
compare_word_to_syllabe (struct slovo *wrd, int wrd_from_position,
			 Syllabe * syl)
{
	int     idx = 0, match = 1;

#ifdef OSM_DEBUG
	printf ("Comparing: ");
	print_slovo (wrd);
	printf (" Length: %d", wrd->count);
	printf ("\n");
	printf ("From position: %d\n", wrd_from_position);
	printf ("To syllabe: ");
	print_slovo (&syl->slv);
	printf (" Length: %d", syl->slv.count);
	printf ("\n");

#endif /*  */
	while (match
	       && idx + wrd_from_position < wrd->count
	       && idx < syl->slv.count) {

#ifdef OSM_DEBUG
		printf ("Comparing: letter %d(idx %d) to letter %d(idx %d).\n", wrd->pism[idx + wrd_from_position].l, idx + wrd_from_position, syl->slv.pism[idx].l, idx);

#endif /*  */
		if (!compare_pism_to_pism_with_quest
		    (&wrd->pism[idx + wrd_from_position],
		     &syl->slv.pism[idx])) {
			match = 0;

#ifdef OSM_DEBUG
			printf ("Don't match match = 0.\n");

#endif /*  */
		}
		idx++;
	}

#ifdef OSM_DEBUG
	if (match && idx == syl->slv.count)
		printf ("Match.\n");
	if (idx != syl->slv.count)
		printf ("Don't match idx.\n");

#endif /*  */
	return (match && idx == syl->slv.count);
}

int
find2_3_syl (OSEMSM_SYL * krz, int pos_x, int pos_y, struct num_path *pth,
	     struct slovo *wrd, int *kon_pos_x, int *kon_pos_y, int *qest2)
{
	int     pos = 0, match_count = 1, match =
		1, new_pos_x, new_pos_y, dir_pos = 0, act_dir, i;
	int     quest2 = 0;
	Syllabe *syl;

	if (!is_in_kriz_syl (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	for (i = 0; i < ((krz->ids / 16) + ((krz->ids % 16) ? 1 : 0)); i++) {
		krz->id[i] = 0;
	}
	syl = get_syllabe (krz, pos_x, pos_y);
	match_count = syl->slv.count;
	while (match && match_count < wrd->count) {
		pos += syl->slv.count;
		if (!get_dir_num_numpath (pth, dir_pos, &act_dir)) {
			match = 0;	/* chyba v definicii cesty */
		}

		else {
			if (get_next_word_in_dir3_syl
			    (krz, pos_x, pos_y, act_dir, &new_pos_x,
			     &new_pos_y, DONT_CHECK_UNIQUE_ID)) {

/*		    pis = get_pismeno(krz, new_pos_x, new_pos_y);*/
				syl = get_syllabe (krz, new_pos_x, new_pos_y);
				if (compare_word_to_syllabe (wrd, pos, syl)) {
					for (i = 0; i < syl->slv.count; i++)
						if (syl->slv.pism[i].l ==
						    QUEST_CHAR)
							quest2++;
					match_count += syl->slv.count;
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
