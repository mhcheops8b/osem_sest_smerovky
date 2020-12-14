#include <stdio.h>
#include <stdlib.h>

#include "q_part.h"

int
create_q_part (struct q_part **qp, int count)
{
	*qp = (struct q_part *) malloc (sizeof (struct q_part));
	if (*qp == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(q_part).\n");
		return 0;
	}
	(*qp)->elems =
		(struct q_sect **) malloc (count * sizeof (struct q_sect *));
	if ((*qp)->elems == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(q_part.qsect).\n");
		free ((void *) *qp);
		*qp = NULL;
		return 0;
	}
	(*qp)->count = count;
	return 1;
}

void
free_q_part (struct q_part **qp)
{
	int     i;

	if (*qp) {
		if ((*qp)->elems) {
			for (i = 0; i < (*qp)->count; i++)
				free_q_sect2 (&((*qp)->elems[i]));
			free ((void *) (*qp)->elems);
			(*qp)->elems = NULL;
			(*qp)->count = 0;
			free ((void *) *qp);
			*qp = NULL;
		}
	}
}
