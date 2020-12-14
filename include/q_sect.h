#ifndef _OSM_Q_SECT_H_
#define _OSM_Q_SECT_H_

#include "position.h"
typedef struct q_sect
{
	int     count;		/* count of data elements */
	Position *elements;
}
Q_SECT;

/* Prototypes */
void    init_vals_q_sect (struct q_sect *qs);
int     create_q_sect (struct q_sect **qs, int count);
int     store_val_q_sect (struct q_sect *qs, int index, int pos_x, int pos_y);
void    free_q_sect (struct q_sect *qs);
void    free_q_sect2 (struct q_sect **qs);

#endif /*  */
