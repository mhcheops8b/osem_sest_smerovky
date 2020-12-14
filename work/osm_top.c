#include <stdio.h>
#include <stdlib.h>
#include "smery.h"
#include "osm_top.h"
#include "osm_size.h"
int
create_osm_topology (Osm_topology * otop, int size_x, int size_y, int ids)
{
  set_size_osm_size (&otop->size, size_x, size_y);
  otop->ids = ids;
  otop->id_field =
    (unsigned int *) malloc (sizeof (unsigned int) * size_x * size_y);
  if (otop->id_field)
    return 0;

  else
    {
      fprintf (stderr, "Error: Memory allocating(create_osm_topology).\n");
      return -1;
    }
}
void
init_osm_topology (Osm_topology * otop)
{
  set_size_osm_size (&otop->size, 0, 0);
  otop->ids = 0;
  otop->id_field = NULL;
}

void
free_osm_topology (Osm_topology * otop)
{
  if (otop)
    {
      if (otop->id_field)
	free ((void *) otop->id_field);
      init_osm_topology (otop);
    }
}
void
set_ids_osm_topology (Osm_topology * otop, int ids)
{
  otop->ids = ids;
}

void
set_id_osm_topology (Osm_topology * otop, int pos_x, int pos_y,
		     unsigned int id_value)
{
  if (otop->ids)
    otop->id_field[pos_x + get_size_x_osm_size (&otop->size) * pos_y] =
      id_value;

  else
    {
      fprintf (stderr,
	       "Error: Memory probably not allocated(set_id_osm_topology).\n");
    }
}
unsigned
get_id_osm_topology (Osm_topology * otop, int pos_x, int pos_y)
{
  if (otop->ids)
    return otop->id_field[pos_x + get_size_x_osm_size (&otop->size) * pos_y];

  else
    return 0;
}

int
get_next_osm_elem_position_in_dir (Osm_topology * otop, int pos_x, int pos_y,
				   int dir_num, int *new_pos_x,
				   int *new_pos_y)
{
  int id, new_id;

  id = get_id_osm_topology (otop, pos_x, pos_y);

  do
    {
      smery_get_next_pos (pos_x, pos_y, dir_num, new_pos_x, new_pos_y);
      if (!is_in_rect_osm_size (&otop->size, *new_pos_x, *new_pos_y))
	return 0;
      new_id = get_id_osm_topology (otop, *new_pos_x, *new_pos_y);
      pos_x = *new_pos_x;
      pos_y = *new_pos_y;
    }
  while (id && id == new_id);
  return 1;
}
