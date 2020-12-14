#include "osm_size.h"
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

int
get_size_x_osm_size (Osm_size * osize)
{
	return osize->size_x;
}

int
get_size_y_osm_size (Osm_size * osize)
{
	return osize->size_y;
}

int
is_in_rect_osm_size (Osm_size * osize, int x, int y)
{
	return (x >= 0 && x < osize->size_x && y >= 0 && y < osize->size_y);
}
