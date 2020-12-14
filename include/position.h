#ifndef _OSM_POSITION_H_
#define _OSM_POSITION_H_

typedef struct position
{
	int     pos_x, pos_y;
}
Position;

/* Prototypes */
void    init_position (Position * pos);
void    set_pos_xy_position (Position * pos, int pos_x, int pos_y);
int     get_pos_x_position (Position * pos);
int     get_pos_y_position (Position * pos);
void    set_pos_x_position (Position * pos, int pos_x);
void    set_pos_y_position (Position * pos, int pos_y);

#endif /*  */
