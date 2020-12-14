#include "smery.h"

typedef struct smer
{
	int     dx, dy; /* differece x,y */
}
SMER;

struct smer osm_smery[] = {
	{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
};

void
smery_get_next_pos (int old_x, int old_y, int dir_num, int *new_pos_x,
		    int *new_pos_y)
{
	*new_pos_x = old_x + smery_get_dx (dir_num);
	*new_pos_y = old_y + smery_get_dy (dir_num);
}

int
smery_get_dx (int dir_num)
{
	return osm_smery[dir_num].dx;
}

int
smery_get_dy (int dir_num)
{
	return osm_smery[dir_num].dy;
}

void
adjust_pos_dirno (int *dirno, int byvalue)
{
	*dirno += byvalue;
	while (*dirno > 7)
		*dirno -= 8;
}

void
adjust_neg_dirno (int *dirno, int byvalue)
{
	*dirno -= byvalue;
	while (*dirno < 0)
		*dirno += 8;
}
