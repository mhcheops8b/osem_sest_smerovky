#ifndef _OSM_Q_PART_H_
#define _OSM_Q_PART_H_

#include "q_sect.h"
typedef struct q_part
{
	int     count;
	struct q_sect **elems;
}
Q_PART;
int     create_q_part (struct q_part **qp, int count);
void    free_q_part (struct q_part **qp);

#endif /*  */
