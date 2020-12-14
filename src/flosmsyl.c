#include <stdio.h>
#include <stdlib.h>
#include "flosmsyl.h"
#include "syltop.h"
#include "pismeno.h"
#include "smery.h"
#include "num_path.h"

int
get_pos_syl (OSEMSM_SYL * krz, int pos_x, int pos_y)
{
	return (pos_x + get_size_x_osm_size (&krz->osm_size) * pos_y);
}

Syllabe *
get_syllabe (OSEMSM_SYL * krz, int pos_x, int pos_y)
{
	return &krz->pole[get_pos_syl (krz, pos_x, pos_y)];
}

void
set_syllabe_id (OSEMSM_SYL * krz, int pos_x, int pos_y, int id)
{
	krz->pole[get_pos_syl (krz, pos_x, pos_y)].id = id;
}

int
is_in_kriz_syl (OSEMSM_SYL * krz, int pos_x, int pos_y)
{
	return is_in_rect_osm_size (&krz->osm_size, pos_x, pos_y);
}

int
is_non_void_syl (OSEMSM_SYL * krz, int pos_x, int pos_y)
{
	Syllabe *syl;

	syl = get_syllabe (krz, pos_x, pos_y);

	if (syl->slv.count == 0 || syl->slv.pism[0].l == EMPTY_CHAR)
		return 0;
	return 1;
}

void
write_sused_syl (struct osemsm_syl *krz, int sx, int sy, int kx, int ky,
		 int dir)
{
	int     pom_x, pom_y, pom_d;
	Syllabe *syl;

	pom_d = dir - 4;
	if (pom_d < 0)
		pom_d += 8;	/* zmen smer na opacny */

	pom_x = sx;
	pom_y = sy;
	while (is_in_kriz_syl (krz, pom_x, pom_y)) {
		pom_x += smery_get_dx (pom_d);
		pom_y += smery_get_dy (pom_d);
	}

	pom_x += smery_get_dx (dir);
	pom_y += smery_get_dy (dir);

	while (is_in_kriz_syl (krz, pom_x, pom_y)) {
		syl = get_syllabe (krz, pom_x, pom_y);
		if (pom_x == sx && pom_y == sy)
			printf ("->");
		if (!is_non_void_syl (krz, pom_x, pom_y))
			printf (".");
		else
			print_slovo (&syl->slv);

		if (pom_x == kx && pom_y == ky)
			printf ("<-");
		get_next_word_in_dir_syl (krz, pom_x, pom_y, dir, &pom_x,
					  &pom_y);
	}
	printf ("\n");
}

void
free_kriz_syl (OSEMSM_SYL * osm)
{
	int     i;

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
		for (i = 0;
		     i <
		     get_size_x_osm_size (&osm->osm_size) *
		     get_size_y_osm_size (&osm->osm_size); i++)
			free ((void *) osm->pole[i].slv.pism);

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
dump_kriz_syl (struct osemsm_syl *osm)
{
	int     x, y;

	printf ("X: %2d, Y: %2d\n", get_size_x_osm_size (&osm->osm_size),
		get_size_y_osm_size (&osm->osm_size));
	for (y = 0; y < get_size_y_osm_size (&osm->osm_size); y++) {
		for (x = 0; x < get_size_x_osm_size (&osm->osm_size); x++)
			print_slovo (&get_syllabe (osm, x, y)->slv);
		printf ("\n");
	}

}

void
print_kriz3_syl (struct osemsm_syl *osm)
{
	int     i, j, length = 0, x, y;
	Syllabe *syl;

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
				syl = get_syllabe (osm, x, y);
				print_slovo (&syl->slv);
			}
			printf ("\n");
		}
	}

	for (j = 0; j < get_size_y_osm_size (&osm->osm_size); j++)
		for (i = 0; i < get_size_x_osm_size (&osm->osm_size); i++) {

			if (!get_bit_bitmap2d_ulm (&osm->bitmap, i, j)) {
				if (!(get_syllabe (osm, i, j)->id)) {
					if (is_non_void_syl (osm, i, j)) {
						print_slovo (&get_syllabe
							     (osm, i,
							      j)->slv);
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
select_kriz3_syl (struct osemsm_syl *osm, char *pth, struct slovo *wrd,
		  int sx, int sy, int dr)
{

	int     pom_x = sx, pom_y = sy, i, dir_pos = 0;
	int     dir;
	struct num_path pth1;

	if (!create_path_str2_numpath (&pth1, dr, pth)) {
		fprintf (stderr, "Nepodarilo sa vytvorit cestu.\n");
		return;
	}

	i = 0;
	while (i < wrd->count) {
		set_bit_bitmap2d_ulm (&osm->bitmap, pom_x, pom_y);
		i += get_syllabe (osm, pom_x, pom_y)->slv.count;
		get_dir_num_numpath (&pth1, dir_pos, &dir);
		get_next_word_in_dir_syl (osm, pom_x, pom_y, dir, &pom_x,
					  &pom_y);
		dir_pos++;
	}
}
