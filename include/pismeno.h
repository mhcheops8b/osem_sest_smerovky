#ifndef _OSM_PISMENO_H_
#define _OSM_PISMENO_H_

#include "table.h"

#define EMPTY_CHAR '\0'
#define QUEST_CHAR 47

/*
 0                          1                        2
 1  2  3 4 5  6 7  8   9    0 1  2 3 4 5   6  7  8 9 0 1  2  3
 a ,a :a b c vc d vd `dz `dvz e ,e f g h `ch  i ,i j k l ,l vl

  2                3                       4                      5
  3 4 5  6 7  8  9 0 1 2  3 4  5 6  7 8  9 0 1 2 3  4 5  6 7 8 9  0 
 vl m n vn o ,o vo p q r ,r s vs t vt u ,u v w x y ,y z vz ? - :u vr
*/

/*const char *pism[];*/
typedef struct pismeno
{
	char    l;
	int     id;
}
Pismeno;
const char *get_pism (int num);
struct pismeno get_table (struct table *tbl, char key);
int     compare_pism_to_pism (Pismeno * pism1, Pismeno * pism2);
int     compare_pism_to_pism_with_quest (struct pismeno *p1,
					 struct pismeno *p2);
#endif /*  */
