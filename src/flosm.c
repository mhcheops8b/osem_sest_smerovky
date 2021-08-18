#include <stdio.h>
#include <stdlib.h>

#include "flosm.h"
#include "slovo.h"
#include "osmtop.h"
#include "smery.h"
void
get_ul_coords (struct osemsm *osm, int sx, int sy, int id,
	       int *new_x, int *new_y)
{
	int     nx = sx, ny = sy;

	while (is_in_kriz (osm, nx - 1, ny - 1)
	       && (get_pismeno (osm, nx - 1, ny - 1)).id == id) {
		nx = nx - 1;
		ny = ny - 1;
	}

	if (is_in_kriz (osm, nx - 1, ny)
	    && (get_pismeno (osm, nx - 1, ny)).id == id) {
		while (is_in_kriz (osm, nx - 1, ny)
		       && (get_pismeno (osm, nx - 1, ny)).id == id)
			nx = nx - 1;
	}
	if (is_in_kriz (osm, nx, ny - 1)
	    && (get_pismeno (osm, nx, ny - 1)).id == id) {
		while (is_in_kriz (osm, nx, ny - 1)
		       && (get_pismeno (osm, nx, ny - 1)).id == id)
			ny = ny - 1;
	}
	*new_x = nx;
	*new_y = ny;
}

void
get_br_coords (struct osemsm *osm, int sx, int sy, int id,
	       int *new_x, int *new_y)
{
	int     nx = sx, ny = sy;

	while (is_in_kriz (osm, nx + 1, ny + 1)
	       && (get_pismeno (osm, nx + 1, ny + 1)).id == id) {
		nx = nx + 1;
		ny = ny + 1;
	}

	if (is_in_kriz (osm, nx + 1, ny)
	    && (get_pismeno (osm, nx + 1, ny)).id == id) {
		while (is_in_kriz (osm, nx + 1, ny)
		       && (get_pismeno (osm, nx + 1, ny)).id == id)
			nx = nx + 1;
	}
	if (is_in_kriz (osm, nx, ny + 1)
	    && (get_pismeno (osm, nx, ny + 1)).id == id) {
		while (is_in_kriz (osm, nx, ny + 1)
		       && (get_pismeno (osm, nx, ny + 1)).id == id)
			ny = ny + 1;
	}
	*new_x = nx;
	*new_y = ny;
}

int
is_in_kriz (OSEMSM * krz, int pos_x, int pos_y)
{
	return is_in_rect_osm_size (&krz->osm_size, pos_x, pos_y);
}

struct pismeno
get_pismeno (OSEMSM * krz, int pos_x, int pos_y)
{
	return krz->pole[get_pos (krz, pos_x, pos_y)];
}

void
set_pismeno (OSEMSM * krz, int pos_x, int pos_y, struct pismeno chr)
{
	if (is_in_kriz (krz, pos_x, pos_y))
		krz->pole[get_pos (krz, pos_x, pos_y)] = chr;
}

int
get_pos (OSEMSM * krz, int pos_x, int pos_y)
{
	return (pos_x + get_size_x_osm_size (&krz->osm_size) * pos_y);
}

int
get_pismeno_id (OSEMSM * krz, int pos_x, int pos_y)
{
	return krz->pole[get_pos (krz, pos_x, pos_y)].id;
}

int
get_pismeno_l (OSEMSM * krz, int pos_x, int pos_y)
{
	return krz->pole[get_pos (krz, pos_x, pos_y)].l;
}

int
is_non_void (OSEMSM * krz, int pos_x, int pos_y)
{

	struct pismeno pom;

	pom = get_pismeno (krz, pos_x, pos_y);
	if (pom.l == EMPTY_CHAR)
		return 0;
	else
		return 1;
}

void
free_kriz (struct osemsm *osm)
{

	if (osm->mask) {
		free ((void *) osm->mask);
		osm->mask = NULL;
	}

	free_bitmap2d_ulm (&osm->bitmap);
	if (osm->id) {
		free ((void *) osm->id);
		osm->id = NULL;
	}
	if (osm->pole) {
		free ((void *) osm->pole);
		osm->pole = NULL;
	}
	if (osm->spec) {
		free_specchar_list (&osm->spec);
		osm->spec = NULL;
	}
	if (osm->qpart)
		free_q_part (&osm->qpart);

	set_size_osm_size (&osm->osm_size, 0, 0);
	osm->ids = 0;
}

void
select_kriz2 (struct osemsm *osm, struct num_path *pth, int sx,
	      int sy, int num)
{

	int     pom_x = sx, pom_y = sy, i;
	int     dir;

	for (i = 0; i < num; i++) {
		set_bit_bitmap2d_ulm (&osm->bitmap, pom_x, pom_y);
		get_dir_num_numpath (pth, i, &dir);
		get_next_word_in_dir (osm, pom_x, pom_y, dir, &pom_x, &pom_y);
	}
}

void
select_kriz3 (struct osemsm *osm, char *pth, struct slovo *wrd,
	      int sx, int sy, int dr)
{

	int     pom_x = sx, pom_y = sy, i;
	int     dir;
	struct num_path pth1;

/*printf("Direction: %d\n", dr);*/
	if (!create_path_str2_numpath (&pth1, dr, pth)) {
		fprintf (stderr, "Nepodarilo sa vytvorit cestu.\n");
		return;
	}

/*	printf("Path: %s\n", pth);
	printf("wrd->count: %d", wrd->count);*/
	for (i = 0; i < wrd->count; i++) {

/*	printf("[%d,%d]",pom_x,pom_y);*/
		set_bit_bitmap2d_ulm (&osm->bitmap, pom_x, pom_y);
		get_dir_num_numpath (&pth1, i, &dir);

/*	printf("(%d) --> ", dir);*/
		get_next_word_in_dir (osm, pom_x, pom_y, dir, &pom_x, &pom_y);
	}
}

void
write_sused (struct osemsm *krz, int sx, int sy, int kx, int ky, int dir)
{
	int     pom_x, pom_y, pom_d;
	struct pismeno pis;

	pom_d = dir - 4;
	if (pom_d < 0)
		pom_d += 8;	/* zmen smer na opacny */

	pom_x = sx;
	pom_y = sy;
	while (is_in_kriz (krz, pom_x, pom_y)) {
		pom_x += smery_get_dx (pom_d);
		pom_y += smery_get_dy (pom_d);
	}

	pom_x += smery_get_dx (dir);
	pom_y += smery_get_dy (dir);

	while (is_in_kriz (krz, pom_x, pom_y)) {
		pis = get_pismeno (krz, pom_x, pom_y);
		if (pom_x == sx && pom_y == sy)
			printf ("->");
		if (!is_non_void (krz, pom_x, pom_y))
			printf (".");
		else
			printf ("%s", get_pism ((int) pis.l));
		if (pom_x == kx && pom_y == ky)
			printf ("<-");
		get_next_word_in_dir (krz, pom_x, pom_y, dir, &pom_x, &pom_y);
	}
	printf ("\n");
}

void
print_kriz2 (struct osemsm *osm)
{
	int     i, j, length = 0;

	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {

			if (!get_bit_bitmap2d_ulm (&osm->bitmap, i, j)) {
				if (!(get_pismeno (osm, i, j).id)) {
					if (is_non_void (osm, i, j)) {
						printf ("%s", get_pism ((int)
									(get_pismeno
									 (osm,
									  i,
									  j)).
									l));

						length++;
					}
					else if (is_in_specchar_list_pos
						 (osm->spec, i, j)) {
						printf ("%c",
							get_char_specchar_list_pos
							(osm->spec, i, j));
						length++;
					}
				}
			}
		}
	printf ("\n%d\n", length);
}

void
print_kriz3 (struct osemsm *osm)
{
	int     i, j, length = 0, x, y;
	struct pismeno pis;

	if (osm->qpart) {
		for (i = 0; i < osm->qpart->count; i++) {
			printf ("P%d:", i);
			for (j = 0; j < osm->qpart->elems[i]->count; j++) {
				x = get_pos_x_position (&osm->qpart->
							elems[i]->
							elements[j]);
				y = get_pos_y_position (&osm->qpart->
							elems[i]->
							elements[j]);
				pis = get_pismeno (osm, x, y);
				printf ("%s", get_pism ((int) pis.l));
			}
			printf ("\n");
		}
	}

#if 0
	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
	{
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {
			printf("%d", get_bit_bitmap2d_ulm (&osm->bitmap, i, j));
		}
		printf("\n");
	}
	printf("\n");
#endif
	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {

			if (!get_bit_bitmap2d_ulm (&osm->bitmap, i, j)) {
				if (!(get_pismeno (osm, i, j).id)) {
					if (is_non_void (osm, i, j)) {
						printf ("%s", get_pism ((int)
									(get_pismeno
									 (osm,
									  i,
									  j)).
									l));
						length++;
					}
					else if (is_in_specchar_list_pos
						 (osm->spec, i, j)) {
						printf ("%c",
							get_char_specchar_list_pos
							(osm->spec, i, j));
						length++;
					}
				}
			}
		}
	printf ("\n%d\n", length);
}

void
print_kriz3_log (struct osemsm *osm, FILE * fou)
{
	int     i, j, length = 0, x, y;
	struct pismeno pis;

	if (osm->qpart) {
		for (i = 0; i < osm->qpart->count; i++) {
			printf ("P%d:", i);
			for (j = 0; j < osm->qpart->elems[i]->count; j++) {
				x = get_pos_x_position (&osm->qpart->
							elems[i]->
							elements[j]);
				y = get_pos_y_position (&osm->qpart->
							elems[i]->
							elements[j]);
				pis = get_pismeno (osm, x, y);
				printf ("%s", get_pism ((int) pis.l));
			}
			printf ("\n");
		}
	}

	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {

			if (!get_bit_bitmap2d_ulm (&osm->bitmap, i, j)) {
				if (!(get_pismeno (osm, i, j).id)) {
					if (is_non_void (osm, i, j)) {
						printf ("%s", get_pism ((int)
									(get_pismeno
									 (osm,
									  i,
									  j)).
									l));
						fprintf (fou, "<%s> %d %d\n",
							 get_pism ((int)
								   (get_pismeno
								    (osm, i,
								     j)).l),
							 i, j);
						length++;
					}
					else if (is_in_specchar_list_pos
						 (osm->spec, i, j)) {
						printf ("%c",
							get_char_specchar_list_pos
							(osm->spec, i, j));
						length++;
					}
				}
			}
		}
}

void
print_kriz3_log2 (struct osemsm *osm, FILE * fou)
{
	int     i, j, length = 0, x, y;
	struct pismeno pis;

	if (osm->qpart) {
		for (i = 0; i < osm->qpart->count; i++) {
			printf ("P%d:", i);
			for (j = 0; j < osm->qpart->elems[i]->count; j++) {
				x = get_pos_x_position (&osm->qpart->
							elems[i]->
							elements[j]);
				y = get_pos_y_position (&osm->qpart->
							elems[i]->
							elements[j]);
				pis = get_pismeno (osm, x, y);
				printf ("%s", get_pism ((int) pis.l));
			}
			printf ("\n");
		}
	}

	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {

			if (!get_bit_bitmap2d_ulm (&osm->bitmap, i, j)) {
				if (!(get_pismeno (osm, i, j).id)) {
					if (is_non_void (osm, i, j)) {
						printf ("%s", get_pism ((int)
									(get_pismeno
									 (osm,
									  i,
									  j)).
									l));
						fprintf (fou, "<%s>(%d %d)\n",
							 get_pism ((int)
								   (get_pismeno
								    (osm, i,
								     j)).l),
							 i, j);
						length++;
					}
					else if (is_in_specchar_list_pos
						 (osm->spec, i, j)) {
						printf ("%c",
							get_char_specchar_list_pos
							(osm->spec, i, j));
						length++;
					}
				}
			}
		}
}

void
replace_id (struct osemsm *osm, int sx, int sy, int val)
{
	struct pismeno pis;
	int     ul_x, ul_y, br_x, br_y;
	int     i, j;
	int     id = (get_pismeno (osm, sx, sy)).id;

	if (!id) {
		pis.l = val;
		pis.id = 0;
		set_pismeno (osm, sx, sy, pis);
		return;
	}
	else {
		get_ul_coords (osm, sx, sy, id, &ul_x, &ul_y);
		get_br_coords (osm, sx, sy, id, &br_x, &br_y);
		pis.id = id;
		pis.l = val;
		for (i = ul_x; i <= br_x; i++)
			for (j = ul_y; j <= br_y; j++)
				set_pismeno (osm, i, j, pis);
	}
}

void
print_masks (OSEMSM * krz)
{
	int     i, j;

	if (krz->mask != NULL) {
		for (i = 0; i < get_size_y_osm_size (&krz->osm_size); i++)
			for (j = 0; j < get_size_x_osm_size (&krz->osm_size);
			     j++)

				printf ("%d %d -> %d\n", j, i,
					krz->mask[j +
						  i *
						  get_size_x_osm_size (&krz->
								       osm_size)]);
	}
}
