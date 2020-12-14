#include <stdio.h>
#include <stdlib.h>
#include "q_sect.h"

#define MSG_ERR_MEMALLOC_EN "Error: Could not allocate memory(%s).\n"
#define MSG_ERR_MEMALLOC_SK "Chyba: Nepodarilo sa alokovat pamat(%s).\n"
#define MSG_ERR_OUTRANGE_SK  \
			     "Chyba: Index mimo rozsahu pola(%s).\n" \
			     "        Rozsah: %2d - %2d\n" \
			     "        Index: %2d\n"
#define MSG_ERR_OUTRANGE_EN  \
			     "Error: Index is out of range of the field(%s).\n"\
			     "        Range: %2d - %2d\n"\
			     "        Index: %2d\n"

/*
 * Name: init_vals_q_sect
 * Input: 
 *   qs - struct q_scect *
 *     =pointer to 'q_sect' structure
 * Output: none
 * ---------------------------
 * Fuctionality:
 *   Initialize all stored Position values in 'q_sect'  to
 *   their default values i.e. (0,0)
 *
 */

void
init_vals_q_sect (struct q_sect *qs)
{
	int     i;

	for (i = 0; i < qs->count; i++)
		init_position (&qs->elements[i]);
}

/*
 * Function: create_q_sect
 * Input: 
 *   qs 	- struct q_sect **qs
 *   count 	- number of Position elements in q_sect structure
 * Output:
 *   0 		- Failure
 *   1		- OK 
 * ------------------------------------------
 *  Functionality:
 *    The memory for *qs is allocated
 *    It  tries to allocate memory for (*qs)->elements to store positions
 *    if the memory can not be allocated, returns 0(Failure)
 *    else return 1(Ok) and (*qs)->elements, contains memory for positions
 *    and (*qs)->count is set to count.
 */


int
create_q_sect (struct q_sect **qs, int count)
{
	*qs = (struct q_sect *) malloc (sizeof (struct q_sect));
	if (*qs == NULL) {

#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_MEMALLOC_EN, "qsect");
#else
		fprintf (stderr, MSG_ERR_MEMALLOC_SK, "q_sect");
#endif

		return 0;
	}
	(*qs)->elements = (Position *) malloc (count * sizeof (Position));
	if ((*qs)->elements == NULL) {

#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_MEMALLOC_EN, "qsect.elements");
#else
		fprintf (stderr, MSG_ERR_MEMALLOC_SK, "qsect.elements");
#endif

		free ((void *) *qs);
		*qs = NULL;
		return 0;
	}
	(*qs)->count = count;
	return 1;
}

/*
 * Name:
 * Input:
 *   qs    - struct q_sect * - pointer to q_sect structure(created with create_q_sec)
 *   index - index-th. position is going to be updated(counted from 0)
 *   pos_x - x-coordinate of the position
 *   pos_y - y-coordinate of the position
 * Output:
 * 	0 - failure(out of index range)
 * 	1 - OK
 * --------------------------------------------------------------
 * Functionality:
 *   If index-th(counted from 0) position is in allowed range, the
 *   function sets the index-th position in q_sect structure qs to 
 *   (pos_x,pos_y) and return 1(OK). If not returns failure.
 *   
 */

int
store_val_q_sect (struct q_sect *qs, int index, int pos_x, int pos_y)
{
	if (index >= qs->count || index < 0) {	/* is in range? */
#ifdef LANG_EN
		fprintf (stderr, MSG_ERR_OUTRANGE_EN, "q_sect.elements", 0,
			 qs->count - 1, index);
#else
		fprintf (stderr, MSG_ERR_OUTRANGE_SK, "q_sect.elements", 0,
			 qs->count - 1, index);
#endif
		return 0;
	}
	set_pos_xy_position (&qs->elements[index], pos_x, pos_y);
	return 1;
}

/* Name: free_q_sect
 * Input:
 *   qs - q_sect * - pointer to q_sect structure
 * Output: none
 * ------------------------
 * Functionality:
 *   Frees allocated memory.
 */

void
free_q_sect (struct q_sect *qs)
{
	if (qs != NULL) {
		if (qs->elements) {
			free ((void *) qs->elements);
			qs->elements = NULL;
		}
		qs->count = 0;
	}
}

/* Name: free_q_sect2
 * Input:
 *   qs - struct q_sect **
 * Output: none
 * -----------------------
 * Functionality:
 *   Frees allocated memory in q_sect structure (*qs) and
 *   also frees the (*qs) itselt.
 */

void
free_q_sect2 (struct q_sect **qs)
{
	if (*qs) {
		if ((*qs)->elements) {
			free ((void *) (*qs)->elements);
			(*qs)->elements = NULL;
		}
		(*qs)->count = 0;
		free ((void *) *qs);
		*qs = NULL;
	}
}
