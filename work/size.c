#ifndef _OSM_SIZE_H_
#define _OSM_SIZE_H_
typedef struct _osm_size
{
  int size_x, size_y;
}
Osm_size;
void
set_size_osm_size (Osm_size * osize, int size_x, int size_y)
{
  osize->size_x = size_x;
  osize->size_y = size_y;
}

void
set_size_x_osm_size (Osm_size * osize, int size_x)
{
  osize->size_x = size_x;
}

void
set_size_y_osm_size (Osm_size * osize, int size_y)
{
  osize->size_y = size_y;
}

Osm_size
get_size_osm_size_osm (Osm_size * osize)
{
  Osm_size new_size;

  new_size.size_x = osize->size_x;
  new_size.size_y = osize->size_y;
  return new_size;
}

void
get_size_osm_size (Osm_size * osize, int *size_x, int *size_y)
{
  *size_x = osize->size_x;
  *size_y = osize->size_y;
}
#endif
