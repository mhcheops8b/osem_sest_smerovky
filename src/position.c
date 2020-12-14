#include "position.h"

/* Init pos to (0,0) */
void
init_position (Position * pos)
{
	pos->pos_x = 0;
	pos->pos_y = 0;
}

/* Init pos to (pos_x,pos_y) */
void
set_pos_xy_position (Position * pos, int pos_x, int pos_y)
{
	pos->pos_x = pos_x;
	pos->pos_y = pos_y;
}

/* return x-coordinate of pos */
int
get_pos_x_position (Position * pos)
{
	return pos->pos_x;
}

/* return y-coordinate of pos */
int
get_pos_y_position (Position * pos)
{
	return pos->pos_y;
}

/* Change only x-coordinate of pos to (pos_x, . ) */
void
set_pos_x_position (Position * pos, int pos_x)
{
	pos->pos_x = pos_x;
}

/* Change only y-coordinate of pos to ( . , pos_y) */
void
set_pos_y_position (Position * pos, int pos_y)
{
	pos->pos_y = pos_y;
}
