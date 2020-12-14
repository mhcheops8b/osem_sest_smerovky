#ifndef _OSM_SIZE_H_
#define _OSM_SIZE_H_
typedef struct _osm_size
{
	int     size_x, size_y;
}
Osm_size;
void    set_size_osm_size (Osm_size * osize, int size_x, int size_y);
void    set_size_x_osm_size (Osm_size * osize, int size_x);
void    set_size_y_osm_size (Osm_size * osize, int size_y);
Osm_size get_size_osm_size_osm (Osm_size * osize);
void    get_size_osm_size (Osm_size * osize, int *size_x, int *size_y);
int     get_size_x_osm_size (Osm_size * osize);
int     get_size_y_osm_size (Osm_size * osize);
int     is_in_rect_osm_size (Osm_size * osize, int x, int y);

#endif /*  */
