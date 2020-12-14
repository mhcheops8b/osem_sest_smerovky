#ifndef _OSM_SMERY_H_
#define _OSM_SMERY_H_

#define smN	0
#define smNE	1
#define smE	2
#define smSE	3
#define smS	4
#define smSW	5
#define smW	6
#define smNW	7

void    smery_get_next_pos (int old_pos_x, int old_pos_y, int dir_num,
			    int *new_pos_x, int *new_pos_y);
int     smery_get_dx (int dir_num);
int     smery_get_dy (int dir_num);
void    adjust_pos_dirno (int *dirno, int byvalue);
void    adjust_neg_dirno (int *dirno, int byvalue);

#endif /*  */
