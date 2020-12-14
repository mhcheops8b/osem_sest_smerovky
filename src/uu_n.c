#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flosm.h"
#include "specchar.h"
#include "osm_ries.h"
#include "smery.h"
#include "bmp2dulm.h"
#include "q_sect.h"
#include "q_part.h"
#include "num_path.h"
#include "pismeno.h"
#include "slovo.h"
#include "table.h"
#include "charpath.h"
#include "osm_size.h"
#include "syllabe.h"
#include "flosmsyl.h"
#include "osmtop.h"
#include "syltop.h"
#include "find.h"
#include "findsyl.h"
#include "cesty.h"
#include "search.h"
#include "obrfile.h"
#include "lustikrz.h"
#include "version.h"

#ifdef DIR_DELIM_WIN32
#define DIR_DELIM '\\'
#else
#define DIR_DELIM '/'
#endif
#define LINE_BUFFER_SIZE 256
#define OK 1
#define NOT_OK 0
#define IGNORE_MULTIPLE 1
#define NOT_IGNORE_MULTIPLE 0
#define SPECIAL_SOLVE 1
#define NOT_SPECIAL_SOLVE 0

/*#define OSM_DEBUG*/
#undef OSM_DEBUG

char   *
strip_path (char *path)
{
	char   *point;

	point = strrchr (path, DIR_DELIM);
	if (!point)
		return path;
	else
		return (point + 1);
}

/*
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
*/

/*
int
is_non_void_syl (OSEMSM_SYL * krz, int pos_x, int pos_y)
{
  Syllabe *syl;

  syl = get_syllabe (krz, pos_x, pos_y);

  if (syl->slv.count == 0 || syl->slv.pism[0].l == EMPTY_CHAR)
    return 0;
  return 1;
}
*/

/*
void
set_pismeno (OSEMSM * krz, int pos_x, int pos_y, struct pismeno chr)
{
  if (is_in_kriz (krz, pos_x, pos_y))
    krz->pole[get_pos (krz, pos_x, pos_y)] = chr;
}
*/

/*
int
get_next_word_in_dir_syl (struct osemsm_syl *krz, int pos_x, int pos_y,
			  int dir_num, int *new_pos_x, int *new_pos_y)
{

  int id, new_id;

  id = (get_syllabe (krz, pos_x, pos_y))->id;
  new_id = id;

  if (id)
    {
      while (id == new_id)
	{
	  smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
	  if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y) ||
	      !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
	    return 0;

	  new_id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->id;
	  pos_x = *new_pos_x;
	  pos_y = *new_pos_y;
	}
      return 1;
    }
  else
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
	  || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
	return 0;
      return 1;
    }
}


int
get_next_word_in_dir (struct osemsm *krz, int pos_x, int pos_y, int dir_num,
		      int *new_pos_x, int *new_pos_y)
{

  int id, new_id;

  id = (get_pismeno (krz, pos_x, pos_y)).id;
  new_id = id;

  if (id)
    {
      while (id == new_id)
	{
	  smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
	  if (!is_in_kriz (krz, *new_pos_x, *new_pos_y) ||
	      !is_non_void (krz, *new_pos_x, *new_pos_y))
	    return 0;

	  new_id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
	  pos_x = *new_pos_x;
	  pos_y = *new_pos_y;
	}
      return 1;
    }
  else
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
	  || !is_non_void (krz, *new_pos_x, *new_pos_y))
	return 0;
      return 1;
    }
}

int
get_next_word_in_dir2 (struct osemsm *krz, int pos_x,
		       int pos_y, int dir_num, int *new_pos_x, int *new_pos_y)
{

  int id, new_id, pom_i, pom_b;

  id = (get_pismeno (krz, pos_x, pos_y)).id;
  new_id = id;

  if (id)
    {
      pom_i = id / 16;
      pom_b = id % 16;
      if ((krz->id[pom_i] & (1 << pom_b)))
	return 0;
      else
	krz->id[pom_i] |= 1 << pom_b;
      while (id == new_id)
	{
	  smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
	  if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
	      || !is_non_void (krz, *new_pos_x, *new_pos_y))
	    return 0;
	  new_id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
	  pos_x = *new_pos_x;
	  pos_y = *new_pos_y;
	}
      return 1;
    }
  else
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
	  || !is_non_void (krz, *new_pos_x, *new_pos_y))
	return 0;
      id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
      if (id && (krz->id[id / 16] & (1 << (id % 16))))
	return 0;
      return 1;
    }
}

#define CHECK_UNIQUE_ID 1
#define DONT_CHECK_UNIQUE_ID 0

int
get_next_word_in_dir3 (struct osemsm *krz,
		       int pos_x, int pos_y, int dir_num,
		       int *new_pos_x, int *new_pos_y, int check_unique_id)
{

  int id, new_id, pom_i, pom_b;

  id = (get_pismeno (krz, pos_x, pos_y)).id;
  new_id = id;

  if (id)
    {
      if (check_unique_id)
	{
	  pom_i = id / 16;
	  pom_b = id % 16;
	  if ((krz->id[pom_i] & (1 << pom_b)))
	    return 0;
	  else
	    krz->id[pom_i] |= 1 << pom_b;
	}

      while (id == new_id)
	{
	  smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
	  if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
	      || !is_non_void (krz, *new_pos_x, *new_pos_y))
	    return 0;
	  new_id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
	  pos_x = *new_pos_x;
	  pos_y = *new_pos_y;
	}
      return 1;
    }
  else
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
	  || !is_non_void (krz, *new_pos_x, *new_pos_y))
	return 0;
      id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
      if (check_unique_id && id && (krz->id[id / 16] & (1 << (id % 16))))
	return 0;
      return 1;
    }
}
*/

/*
int
get_next_word_in_dir3_syl (OSEMSM_SYL * krz,
			   int pos_x, int pos_y, int dir_num,
			   int *new_pos_x, int *new_pos_y,
			   int check_unique_id)
{

  int id, new_id, pom_i, pom_b;

  id = (get_syllabe (krz, pos_x, pos_y))->id;
  new_id = id;

  if (id)
    {
      if (check_unique_id)
	{
	  pom_i = id / 16;
	  pom_b = id % 16;
	  if ((krz->id[pom_i] & (1 << pom_b)))
	    return 0;
	  else
	    krz->id[pom_i] |= 1 << pom_b;
	}

      while (id == new_id)
	{
	  smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
	  if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
	      || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
	    return 0;
	  new_id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->id;
	  pos_x = *new_pos_x;
	  pos_y = *new_pos_y;
	}
      return 1;
    }
  else
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_kriz_syl (krz, *new_pos_x, *new_pos_y)
	  || !is_non_void_syl (krz, *new_pos_x, *new_pos_y))
	return 0;
      id = (get_syllabe (krz, *new_pos_x, *new_pos_y))->id;
      if (check_unique_id && id && (krz->id[id / 16] & (1 << (id % 16))))
	return 0;
      return 1;
    }
}
*/

#if 0
#define      CHECK_EMPTYNESS 1
#define DONT_CHECK_EMPTYNESS 0

int
get_next_word_in_dir4 (struct osemsm *krz,
		       int pos_x, int pos_y, int dir_num,
		       int *new_pos_x, int *new_pos_y, int check_unique_id,
		       int check_emptyness)
{

	int     id, new_id, pom_i, pom_b;

	id = (get_pismeno (krz, pos_x, pos_y)).id;
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
			if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
			    || (!is_non_void (krz, *new_pos_x, *new_pos_y)
				&& check_emptyness == CHECK_EMPTYNESS))
				return 0;

			new_id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).
				id;
			pos_x = *new_pos_x;
			pos_y = *new_pos_y;
		}

		return 1;
	}
	else {
		smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x,
				    new_pos_y);
		if (!is_in_kriz (krz, *new_pos_x, *new_pos_y)
		    || (!is_non_void (krz, *new_pos_x, *new_pos_y)
			&& check_emptyness == CHECK_EMPTYNESS))
			return 0;

		id = (get_pismeno (krz, *new_pos_x, *new_pos_y)).id;
		if (check_unique_id && id
		    && (krz->id[id / 16] & (1 << (id % 16))))
			return 0;
		return 1;
	}
}
#endif

void
osm_dump_lr (struct osemsm *krz)
{
	int     x, y, new_x, new_y;

	for (y = 0; y < get_size_y_osm_size (&krz->osm_size); y++) {
		new_x = 0;
		new_y = y;
		printf ("%2d->", y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 2, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

}

void
osm_dump_rl (struct osemsm *krz)
{
	int     x, y, new_x, new_y;

	for (y = 0; y < get_size_y_osm_size (&krz->osm_size); y++) {
		new_x = get_size_x_osm_size (&krz->osm_size) - 1;
		new_y = y;
		printf ("%2d<-", y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 6, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

}

void
osm_dump_ud (struct osemsm *krz)
{
	int     x, y, new_x, new_y;

	for (x = 0; x < get_size_x_osm_size (&krz->osm_size); x++) {
		new_x = x;
		new_y = 0;
		printf ("%2d\\/", x);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 4, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

}

void
osm_dump_du (struct osemsm *krz)
{
	int     x, y, new_x, new_y;

	for (x = 0; x < get_size_x_osm_size (&krz->osm_size); x++) {
		new_y = get_size_y_osm_size (&krz->osm_size) - 1;
		new_x = x;
		printf ("%2d/\\", x);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 0, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

}

void
osm_dump_uldr (struct osemsm *krz)
{
	int     x, y, z, new_x, new_y;

	for (z = get_size_y_osm_size (&krz->osm_size) - 1; z >= 0; z--) {
		new_x = 0;
		new_y = z;
		x = new_x;
		printf ("%2d,%2d_\\|", x, z);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 3, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

	for (z = 1; z < get_size_x_osm_size (&krz->osm_size); z++) {
		new_x = z;
		new_y = 0;
		y = new_y;
		printf ("%2d,%2d_\\|", z, y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 3, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}
}

void
osm_dump_drul (struct osemsm *krz)
{
	int     x, y, z, new_x, new_y;

	for (z = 0; z < get_size_x_osm_size (&krz->osm_size); z++) {
		new_x = z;
		new_y = get_size_y_osm_size (&krz->osm_size) - 1;
		y = new_y;
		printf ("%2d,%2d|\\-", z, y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 7, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

	for (z = get_size_y_osm_size (&krz->osm_size) - 2; z >= 0; z--) {
		new_x = get_size_x_osm_size (&krz->osm_size) - 1;
		new_y = z;
		x = new_x;
		printf ("%2d,%2d|\\-", x, z);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 7, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}
}

void
osm_dump_urdl (struct osemsm *krz)
{
	int     x, y, z, new_x, new_y;

	for (z = 0; z < get_size_x_osm_size (&krz->osm_size); z++) {
		new_x = z;
		new_y = 0;
		y = new_y;
		printf ("%2d,%2d|/_", z, y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 5, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

	for (z = 1; z < get_size_y_osm_size (&krz->osm_size); z++) {
		new_x = get_size_x_osm_size (&krz->osm_size) - 1;
		new_y = z;
		x = new_x;
		printf ("%2d,%2d_\\|", x, z);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 5, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}
}

void
osm_dump_dlur (struct osemsm *krz)
{
	int     x, y, z, new_x, new_y;

	for (z = 0; z < get_size_y_osm_size (&krz->osm_size); z++) {
		new_x = 0;
		new_y = z;
		x = new_x;
		printf ("%2d,%2d|\\-", x, z);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 1, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}

	for (z = 1; z < get_size_x_osm_size (&krz->osm_size); z++) {
		new_x = z;
		new_y = get_size_y_osm_size (&krz->osm_size) - 1;
		y = new_y;
		printf ("%2d,%2d|\\-", z, y);
		do {
			x = new_x;
			y = new_y;
			printf ("%s",
				get_pism ((int) get_pismeno (krz, x, y).l));
		}
		while (get_next_word_in_dir4
		       (krz, x, y, 1, &new_x, &new_y, DONT_CHECK_UNIQUE_ID,
			DONT_CHECK_EMPTYNESS));
		printf ("\n");
	}
}

/*#include "find.c"*/


/*
int
is_qword (SLOVO * wrd)
{
  int i;

  for (i = 0; i < wrd->count; i++)
    if (wrd->pism[i].l == QUEST_CHAR)
      return 1;

  return 0;
}
*/

#if 0
void
print_qword_solution (OSEMSM * krz, int st_x, int st_y, struct num_path *pth,
		      SLOVO * wrd)
{
	int     len = 0, pos_x = st_x, pos_y =
		st_y, new_pos_x, new_pos_y, act_dir;

	while (len < wrd->count) {
		if (wrd->pism[len].l == QUEST_CHAR) {
			printf ("%s", get_pism ((int)
						get_pismeno (krz, pos_x,
							     pos_y).l));
			printf ("(%2d,%2d)", pos_x, pos_y);
		}
		else
			printf ("%s", get_pism ((int) wrd->pism[len].l));

		get_dir_num_numpath (pth, len, &act_dir);
		get_next_word_in_dir2 (krz, pos_x, pos_y, act_dir, &new_pos_x,
				       &new_pos_y);

		pos_x = new_pos_x;
		pos_y = new_pos_y;
		len++;
	}			/* end while */

}
#endif

char
get_m_th_elem (char **path, int m)
{

	int     i = 0;
	char   *pom = *path;

	while (*pom && i < m) {
		switch (*pom) {
		case 'P':
		case 'M':
			pom++;
			i++;
			break;
		case 'I':
		case '0':
		case '+':
		case '-':
			i++;
			break;
		}
		pom++;
	}
	if (i < m) {
		fprintf (stderr, "Chyba: m-ty element cesty neexistuje.\n");
		return 0;
	}
	*path = pom;
	return 1;
}

int
copy_m_th_elem (char *path, int m, char *buff)
{
	char   *pom = path;

	if (!get_m_th_elem (&pom, m))
		return 0;
	if (*pom == 'M' || *pom == 'P') {
		buff[0] = *pom;
		buff[1] = *(pom + 1);
	}
	else {
		*buff = *pom;
	}
	return 1;
}

/*
int
rol_path (struct num_path *pth)
{
  int pom, i;

  if (!pth->is_o_path)
    {
      fprintf (stderr,
	       "Chyba: Nemozno rotovat cestu, ktora nie je O cesta.\n");
      return 0;
    }
  pom = pth->for_smery_nums[0];
  for (i = 1; i < pth->for_elems; i++)
    {
      pth->for_smery_nums[i - 1] = pth->for_smery_nums[i];
    }
  pth->for_smery_nums[pth->for_elems - 1] = pom;
  return 1;
}
*/

#if 0
int
rol_miss_dir (char *path)
{
	char   *pom = path;
	int     miss_dir = 0;

	pom++;			/* preskoc 'O' na zaciatku cesty */
	while (*pom) {
		switch (*pom) {
		case 'P':
			pom++;
			miss_dir += *pom - '0';
			break;
		case 'M':
			pom++;
			miss_dir -= *pom - '0';
			break;
		case '+':
			miss_dir += 2;
			break;
		case '-':
			miss_dir -= 2;
			break;
		}
		pom++;

	}

	while (miss_dir >= 8)
		miss_dir -= 8;
	while (miss_dir < 0)
		miss_dir += 8;

	return miss_dir;
}
#endif
#if 0
int
rol_path_str_ver2 (char *path, char *buffer)
{
	int     miss_dir = rol_miss_dir (path);
	char   *pom = path, *pom2 = buffer;

	if (*pom != 'O') {
		fprintf (stderr,
			 "Chyba: Nemozno rolovat cestu, ktora nie je O cesta.\n");
		return 0;
	}
	*pom2++ = *pom++;

	if (*pom != '0') {
		fprintf (stderr, "Chyba: O cesta musi zacinat hodnotou 0.\n");
		return 0;
	}
	*pom2++ = *pom++;

	/*
	 * preskoc specifikaciu smeru - prvy bude vzdy 0
	 */

	if (*pom == 'M' || *pom == 'P')
		pom += 2;
	else
		pom++;

	while (*pom)
		*pom2++ = *pom++;	/* kopiruj zvysok retazca */

	if (miss_dir > 4) {
		if (miss_dir == 6)
			*pom2++ = '+';
		else {
			*pom2++ = 'P';
			*pom2++ = '0' + (8 - miss_dir);
		}
	}
	else {
		if (miss_dir == 0)
			*pom2++ = '0';
		else if (miss_dir == 2)
			*pom2++ = '-';
		else {
			*pom2++ = 'M';
			*pom2++ = '0' + miss_dir;
		}
	}

	*pom2 = '\0';
	return 1;
}

#define BAD_ARG         -2
#define NOT_SAME        -1
#define ARE_SAME         0

int
cmp_o_path_str (char *o_path1, char *o_path2)
{
	char   *pom1 = o_path1, *pom2 = o_path2;
	int     same, diff, val;

	if (*pom1 != 'O') {
		fprintf (stderr,
			 "prvy argument vo funkcii `cmp_o_path_str` nie je O cesta\n");
		return BAD_ARG;
	}

	if (*pom2 != 'O') {
		fprintf (stderr,
			 "druhy argument vo funkcii `cmp_o_path_str` nie je O cesta\n");
		return BAD_ARG;
	}

	pom1++;
	pom2++;

	while (*pom1 && *pom2) {

		switch (*pom1) {
		case '0':
			same = 0;
			diff = 1;

			if (*pom2 == '0')
				same = 1;
			else if (*pom2 == 'P' && *(pom2 + 1) == '0') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'M' && *(pom2 + 1) == '0') {
				diff = 2;
				same = 1;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;
		case '+':
			same = 0;
			diff = 1;

			if (*pom2 == '+')
				same = 1;
			else if (*pom2 == 'P' && *(pom2 + 1) == '2') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'M' && *(pom2 + 1) == '6') {
				same = 1;
				diff = 2;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;
		case '-':
			same = 0;
			diff = 1;

			if (*pom2 == '-')
				same = 1;
			else if (*pom2 == 'M' && *(pom2 + 1) == '2') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'P' && *(pom2 + 1) == '6') {
				same = 1;
				diff = 2;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;

		case 'P':
			same = 0;
			diff = 1;
			val = *(pom1 + 1);
			switch (val) {
			case '0':
				if (*pom2 == '0')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '0')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '0')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;
				break;
			case '2':
				if (*pom2 == '+')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '2')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			case '6':
				if (*pom2 == '-')
					same = 1;
				else if ((*pom2 == 'M' && *(pom2 + 1) == '2')
					 || (*pom2 == 'P'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			default:
				if ((*pom2 == 'P' && *(pom2 + 1) == val) ||
				    (*pom2 == 'M'
				     && *(pom2 + 1) ==
				     '0' + (8 - (val - '0')))) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;
			}
			break;
		case 'M':
			same = 0;
			diff = 1;
			val = *(pom1 + 1);
			switch (val) {
			case '0':
				if (*pom2 == '0')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '0')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '0')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;
				break;
			case '2':
				if (*pom2 == '-')
					same = 1;
				else if ((*pom2 == 'M' && *(pom2 + 1) == '2')
					 || (*pom2 == 'P'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			case '6':
				if (*pom2 == '+')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '2')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			default:
				if ((*pom2 == 'M' && *(pom2 + 1) == val) ||
				    (*pom2 == 'P'
				     && *(pom2 + 1) ==
				     '0' + (8 - (val - '0')))) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;
			}
			break;

		}		/* end switch */

	}			/* end while */


	if (*pom1 == *pom2)
		return ARE_SAME;
	else
		return NOT_SAME;
}				/* end cmp_o_path_str */

int     rol_path_str (char *path, char *buffer);


int
rol_path_str (char *path, char *buffer)
{
	char    buf[3] = { '\0', '\0', '\0' }, *pom = path, *pom2 = buffer;
	int     i = 0;

	if (*pom != 'O') {
		fprintf (stderr,
			 "Chyba: Nemozno rolovat cestu, ktora nie je O cesta.\n");
		return 0;
	}
	buffer[0] = *pom;
	pom2 = &buffer[1];
	pom++;

	if (*pom == 'P' || *pom == 'M') {
		buf[0] = *pom;
		buf[1] = *(pom + 1);
		buf[2] = '\0';
		pom += 2;
	}
	else {
		buf[0] = *pom;
		buf[1] = '\0';
		pom++;
	}

	while (*pom) {
		*pom2 = *pom;
		pom++;
		pom2++;
		i++;
	}
	if (strlen (buf) < 2) {
		pom2[0] = buf[0];
		pom2[1] = '\0';
	}
	else {
		pom2[0] = buf[0];
		pom2[1] = buf[1];
		pom2[2] = '\0';
	}
	return 1;
}
#endif
#if 0
int
is_same_sol_id_syl (OSEMSM_SYL * krz, struct osm_ries *o_ries, int start_x,
		    int start_y, int dir)
{

	Syllabe *syl;
	struct osm_ries *ries = o_ries;
	int     id;

	syl = get_syllabe (krz, start_x, start_y);

	id = syl->id;

	if (!o_ries)		/* no solution -  ignoring */
		return 0;
	if (!id)		/* no set id - ignoring */
		return 0;

	while (ries) {
		syl = get_syllabe (krz, ries->pos_x, ries->pos_y);

		if (syl->id == id && ries->dir_num == dir)
			return 1;	/* solution is already known */

		ries = ries->next;
	}

	return 0;
}

int
is_same_sol_id (struct osemsm *krz, struct osm_ries *o_ries, int start_x,
		int start_y, int dir)
{

	struct pismeno pis;
	struct osm_ries *ries = o_ries;
	int     id;

	pis = get_pismeno (krz, start_x, start_y);
	id = pis.id;
	if (!o_ries)
		return 0;
	if (!pis.id)
		return 0;

	while (ries) {
		pis = get_pismeno (krz, ries->pos_x, ries->pos_y);
		if (pis.id == id && ries->dir_num == dir)
			return 1;	/*
					   * je
					   * rovnake
					   * riesenie
					   * ako
					   * zname
					 */
		ries = ries->next;
	}
	return 0;
}

int
is_same_sol_palindrom (struct osm_ries *o_ries, struct slovo *slv, int end_x,
		       int end_y, int dir_num)
{

	struct osm_ries *ries = o_ries;
	int     dif;

	if (!is_palindrom (slv))
		return 0;

	while (ries) {
		dif = ries->dir_num - dir_num;
		if (dif < 0)
			dif = -dif;
		if (ries->pos_x == end_x && ries->pos_y == end_y && dif == 4)
			return 1;	/*
					   * je
					   * rovnake
					   * palindromicke
					   * riesenie
					 */
		ries = ries->next;
	}
	return 0;
}
#endif

#if 0
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

	osm->siz_x = 0;
	osm->siz_y = 0;
	osm->ids = 0;
}
#endif

/*void print_kriz_2(struct osemsm *osm) {
int i,j,co,co_i,co_b,length=0;
for (j=0;j<osm->siz_y;j++)
for (i=0;i<osm->siz_x;i++) {
  co=i+j*osm->siz_x;
  co_i=co / 16;
  co_b=co % 16;
 // if (co_b) co_i++;
  if (!(osm->bitmap[co_i] & (1 << co_b)) && !(get_pismeno(osm,i,j).id)) {
    printf("%s",pism[(get_pismeno(osm,i,j)).l],(get_pismeno(osm,i,j)).l);
    length++;
  }
}
printf("\n%d\n",length);
}
*/

void
osm_usage (char *filename)
{
	printf ("Osemsmerovky verzia: %s datum: %s, autor: Mgr. Marek Hycko\n", VERSION_STR, VERSION_DATE);
	printf ("\nProgram lusti osemsmerovky v stvorcovej tabulke.\n");
	printf ("\nUsage: %s [options] <osem_file> [<slova_file> [<log_file>]]\n", strip_path (filename));
	printf ("   <options>    - predvolby\n");
	printf ("   --test     - testuje obrazec\n");
	printf ("   --prompt   - priame zadavanie slov z klavesnice\n");
	printf ("   --log      - zapisuje postupy riesenia do suboru <log_file>.\n");
	printf ("   --dump	 - vypis obrazca po riadkoch, stlpcoch a uhloprieckach.\n");
	printf ("   --syllabe  - lusti slabikovu osemsmerovku.\n");
	printf ("   <osem_file> - subor s osemsmerovkou\n");
	printf ("  <slova_file> - subor so slovami\n");
	printf ("    <log_file> - log subor\n\n");
}

int
get_new_dir_value (int st_val, int old_val, char **elem_path, int *new_val)
{
	char   *pom = *elem_path;
	int     val = old_val;

	switch (*pom) {
	case '0':
		pom++;
		break;
	case '+':
		adjust_pos_dirno (&val, 2);
		pom++;
		break;
	case '-':
		adjust_neg_dirno (&val, 2);
		pom++;
		break;
	case 'I':
		val = st_val;
		pom++;
		break;
	case 'P':
		pom++;
		if (*pom < '0' || *pom > '9') {
			fprintf (stderr,
				 "Chyba: Za P musi nasledovat cislo.\n");
			return 0;
		}

		adjust_pos_dirno (&val, *pom - '0');
		pom++;
		break;
	case 'M':
		pom++;
		if (*pom < '0' || *pom > '9') {
			fprintf (stderr,
				 "Chyba: Za M musi nasledovat cislo.\n");
			return 0;
		}
		adjust_neg_dirno (&val, *pom - '0');
		pom++;
		break;
	default:
		fprintf (stderr,
			 "Chyba: V specifikacii cesty sa nachadzaju nezname znaky.\n");
		return 0;
	}
	*new_val = val;
	*elem_path = pom;
	return 1;
}

int
test_o_cesta (char *path)
{
	char   *pom = path;
	int     pom_x, pom_y, pom_d, d;

	pom_x = pom_y = pom_d = d = 0;

	if (*pom != 'O') {
		fprintf (stderr,
			 "Chyba: Specifikacia cesty musi zacinat s O.\n");
		return 0;
	}
	pom++;
	if (!*pom) {
		fprintf (stderr, "Chyba: O cesta je prazdna.\n");
		return 0;
	}

	while (*pom) {
		if (!get_new_dir_value (d, pom_d, &pom, &pom_d))
			return 0;
		pom_x += smery_get_dx (pom_d);
		pom_y += smery_get_dy (pom_d);
	}

	if (pom_x != 0 || pom_y != 0) {
		fprintf (stderr, "Chyba: Cesta musi byt cyklicka.\n");
		return 0;
	}
	return 1;
}

/*#include "lustikrz.c"*/


int
main (int argc, char **argv)
{

	if (argc < 2) {
		fprintf (stderr,
			 "Chyba: program musi byt spusteny s nejakymi volbami.\n");
		osm_usage (argv[0]);
		return -1;
	}
	else {
		if (argv[1][0] != '-') {	/*
						 * bez
						 * predvolieb
						 */
			if (argc < 3) {
				fprintf (stderr,
					 "Chyba: Musi byt specifikovany subor s obrazcom aj so slovami.\n");
				osm_usage (argv[0]);
				return -1;
			}
#define PRINT_SOLUTION	1
#define DO_NOT_PRINT_SOLUTION 0
#define PRINT_SUSED	1
#define DO_NOT_PRINT_SUSED	0
			lusti_kriz3_6 (argv[1], argv[2], NOT_IGNORE_MULTIPLE,
				       SPECIAL_SOLVE, PRINT_SOLUTION, DO_NOT_PRINT_SUSED);
		}
		else {		/*
				 * su
				 * nejake
				 * prevolby
				 */
			if (!strcmp (argv[1], "--test")
			    || !strcmp (argv[1], "--prompt")
			    || !strcmp (argv[1], "--log")
			    || !strcmp (argv[1], "--dump")
			    || !strcmp (argv[1], "--syllabe")) {
				if (!strcmp (argv[1], "--syllabe")) {
					if (argc < 4) {
						fprintf (stderr,
							 "Chyba: ocakava sa nazov suboru s obrazcom i slovami.\n");
						osm_usage (argv[0]);
						return 0;
					}
					lusti_kriz3_6_syl (argv[2], argv[3],
							   NOT_IGNORE_MULTIPLE,
							   NOT_SPECIAL_SOLVE,
							   0, 0);
				}
				else if (!strcmp (argv[1], "--test")) {
					if (argc < 3) {
						fprintf (stderr,
							 "Chyba: Za --test musi nasledovat nazov suboru s obrazcom, ktory sa ma otestovat.\n");
						osm_usage (argv[0]);
						return -1;
					}
					else {
						if (test_osm_obr (argv[2]))
							printf ("Obrazec je v poriadku.\n");
						else
							return 0;
					}
				}
				else if (!strcmp (argv[1], "--prompt")) {
					if (argc < 3) {
						fprintf (stderr,
							 "Chyba: Za --prompt musi nasledovat nazov suboru s obrazcom.\n");
						osm_usage (argv[0]);
						return -1;
					}
					lusti_kriz3_6_prompt (argv[2],
							      NOT_IGNORE_MULTIPLE,
							      NOT_SPECIAL_SOLVE,
							      1, 1);
					return 0;
				}
				else if (!strcmp (argv[1], "--log")) {
					if (argc < 4) {
						fprintf (stderr,
							 "Chyba: Za --log musia nasledovat nazvy suborov <obr_file> <slo_file> <log_file>.\n");
						osm_usage (argv[0]);
						return -1;
					}
					lusti_kriz3_6_log (argv[2], argv[3],
							   argv[4],
							   NOT_IGNORE_MULTIPLE,
							   NOT_SPECIAL_SOLVE,
							   0, 0);
				}
				else if (!strcmp (argv[1], "--dump")) {
					if (argc < 3) {
						fprintf (stderr,
							 "Chyba: Za --dump musi nasledovat <obr_file>.\n");
						osm_usage (argv[0]);
						return -1;
					}
					printf ("%d\n", argc);
					{
						struct osemsm osm;

						if (!create_kriz
						    (argv[2], &osm)) {
							return -1;
						}
						printf ("Left to right:\n");
						osm_dump_lr (&osm);
						printf ("Right to left:\n");
						osm_dump_rl (&osm);
						printf ("Up to down:\n");
						osm_dump_ud (&osm);
						printf ("Down to up:\n");
						osm_dump_du (&osm);
						printf ("UpperLeft to DownRight:\n");
						osm_dump_uldr (&osm);
						printf ("DownRight to UpperLeft:\n");
						osm_dump_drul (&osm);
						printf ("UpperRight to DownLeft:\n");
						osm_dump_urdl (&osm);
						printf ("DownLeft to UpperRight:\n");
						osm_dump_dlur (&osm);
						free_kriz (&osm);

						/*                      osm_dump_rows_lr()*/
					}
				}
			}
			else {
				fprintf (stderr, "Chyba: Neznama volba.\n");
				osm_usage (argv[0]);
				return -1;
			}
		}
	}
	return 0;
}
