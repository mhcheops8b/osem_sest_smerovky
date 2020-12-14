#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osm_ries.h"

int
create_osm_ries_list (struct osm_ries *ries,
		      int pos_x, int pos_y, int dir_num, int cnt_quest,
		      char *path)
{

	/* alocate memory for path specification */

	ries->chr_path = (char *) malloc (strlen (path) + 1);
	if (ries->chr_path == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(osm_ries).\n");
		return OSM_RIES_ERR;
	}

	ries->pos_x = pos_x;
	ries->pos_y = pos_y;
	ries->dir_num = dir_num;
	ries->cnt_question_mark = cnt_quest;
	strcpy (ries->chr_path, path);
	ries->next = NULL;
	return OSM_RIES_OK;
}

int
add_osm_ries_list (struct osm_ries **ries,
		   int pos_x, int pos_y, int dir_num, int cnt_quest,
		   char *path)
{

	struct osm_ries *pom;

	if (*ries == NULL) {
		*ries = (struct osm_ries *) malloc (sizeof (struct osm_ries));

		if (*ries == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate(add_osm_ries_list).\n");
			return OSM_RIES_ERR;
		}

		return create_osm_ries_list (*ries, pos_x, pos_y, dir_num,
					     cnt_quest, path);
	}
	else {
		pom = (struct osm_ries *) malloc (sizeof (struct osm_ries));

		if (pom == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate(add_osm_ries_list).\n");
			return OSM_RIES_ERR;
		}

		if (create_osm_ries_list
		    (pom, pos_x, pos_y, dir_num, cnt_quest, path)
		    != OSM_RIES_OK)
			return OSM_RIES_ERR;

		pom->next = *ries;
		*ries = pom;
		return OSM_RIES_OK;
	}
}

void
free_osm_ries (struct osm_ries *list)
{

	struct osm_ries *pom = list;

	while (pom) {
		pom = list->next;

		if (list->chr_path != NULL) {
			free ((void *) list->chr_path);
			list->chr_path = NULL;
		}

		if (list != NULL) {
			free ((void *) list);
		}
		list = pom;
	}
}

int
has_quest_ries (struct osm_ries *ries)
{

	struct osm_ries *pom = ries;

	while (pom) {
		if (pom->cnt_question_mark)
			return 1;
		pom = pom->next;
	}
	return 0;
}

void
count_quests (struct osm_ries *ries, int *has_q, int *non_q)
{

	struct osm_ries *pom = ries;
	int     q = 0, nq = 0;

	while (pom) {
		if (pom->cnt_question_mark)
			q++;
		else
			nq++;
		pom = pom->next;
	}

	*has_q = q;
	*non_q = nq;
}

int
get_size_of_osm_ries (struct osm_ries *ries)
{
	struct osm_ries *pom = ries;
	int     pocet = 0;

	while (pom) {
		pocet++;
		pom = pom->next;
	}
	return pocet;
}

int
get_min_ries_count (struct osm_ries *ries)
{
	struct osm_ries *pom_ries;
	int     pocet = 0, min_val = 0;

	pom_ries = ries;
	while (pom_ries) {
		if (pom_ries->cnt_question_mark
		    && min_val > pom_ries->cnt_question_mark) {
			min_val = pom_ries->cnt_question_mark;
			pocet = 1;
		}
		else if (pom_ries->cnt_question_mark
			 && min_val == pom_ries->cnt_question_mark)
			pocet++;
		pom_ries = pom_ries->next;
	}
	return pocet;
}

int
test_riesenie (struct osm_ries *ries, int test_count)
{
	int     size = get_size_of_osm_ries (ries);
	int     que, nonque;

	que = nonque = 0;

	if (!size) {
		return -1;	/* nenaslo sa slovo */
	}
	if (size != test_count) {
		if (size < test_count) {
			fprintf (stderr,
				 "Chyba: Nenaslo sa dostatok vyskytov daneho slova.\n");
			return 0;
		}
		else {
			fprintf (stderr,
				 "Chyba: Prilis vela vyskytov daneho slova.\n");
			return 0;
		}
	}
	return nonque + que;
}
