#ifndef _OSM_TOP_H_
#define _OSM_TOP_H_

#include "osm_size.h"
typedef struct _osm_top
{
  Osm_size size;
  int ids;			/* no of id */
  unsigned int *id_field;
}
Osm_topology;
int create_osm_topology (Osm_topology * otop, int size_x, int size_y,
			 int ids);
void init_osm_topology (Osm_topology * otop);
void free_osm_topology (Osm_topology * otop);
void set_ids_osm_topology (Osm_topology * otop, int ids);
int get_next_osm_elem_position_in_dir (Osm_topology * otop, int pos_x,
				       int pos_y, int dir_num,
				       int *new_pos_x, int *new_pos_y);

#endif /*  */
