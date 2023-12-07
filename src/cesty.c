#include <stdio.h>
#include <stdlib.h>
#include "cesty.h"

char   *default_cesta_elems[] = { "C0" };
char   *vlnka_cesta_elems[] = { "CI+", "CI-", "CI++-", "CI--+" };
char   *obdlznik_cesta_elems[] = {
	"F0+0+00000+0+0000", "F0-0-00000-0-0000",
	"F00+0+00000+0+000", "F00-0-00000-0-000",
	"F000+0+00000+0+00", "F000-0-00000-0-00",
	"F0000+0+00000+0+0", "F0000-0-00000-0-0",
	"F00000+0+00000+0+", "F00000-0-00000-0-",
	"F000000+0+00000+0", "F000000-0-00000-0",
	"F0+00000+0+00000+", "F0-00000-0-00000-",
	"F00+00000+0+00000", "F00-00000-0-00000"
};

struct cesty default_cesta = { 1, default_cesta_elems };
struct cesty vlnka_cesta = { 4, vlnka_cesta_elems };
struct cesty obdlznik_cesta = { 16, obdlznik_cesta_elems };
char    cesta_allowed_chars[] =
	{ 'C', 'F', 'O', 'I', '0', '+', '-', 'P', 'M', '1', '2', '3', '4',
	  '5', '6', '7', '8', '9', '\0'
};

int
cesta_is_allowed (char *allowed, char c)
{
	char   *pom = allowed;

	while (*pom) {
		if (*pom == c)
			return 1;
		pom++;
	}
	return 0;
}

int
test_cesta (char *start, char *end)
{
	char   *pom;

	if (start == end)
		return 0;
	for (pom = start; pom <= end; pom++) {
		if (!cesta_is_allowed (cesta_allowed_chars, *pom))
			return 0;
	}
	return 1;
}

int
count_cesta (char *elem)
{
	int     count = 0;
	char   *pom_start = elem, *pom_end, *pom = elem;

	while (*pom) {
		if (*pom == '|') {
			pom_end = pom - 1;
			if (!test_cesta (pom_start, pom_end))
				return 0;
			else
				count++;
			pom_start = pom + 1;
		}
		pom++;
	}
	pom_end = pom - 1;
	if (!test_cesta (pom_start, pom_end))
		return 0;
	else
		count++;
	return count;
}

int
create_cesty (struct cesty *cst, char *elems)
{

	int     count, pe, i = 0, j;
	char   *pom, *pom_st;

	count = count_cesta (elems);
	if (count <= 0) {
		fprintf (stderr,
			 "Chyba: Specifikacia cesty obsahuje chyby.\n");
		cst->elems = NULL;
		cst->count = 0;
		return 0;
	}
	cst->elems = (char **) malloc (sizeof (char *) * count);

	if (cst->elems == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		cst->count = 0;
		return 0;
	}
	pom = elems;
	pom_st = elems;
	while (*pom) {
		if (*pom == '|') {
			pe = (int)(pom - pom_st);
			cst->elems[i] =
				(char *) malloc (sizeof (char) * (pe + 1));

			if (cst->elems[i] == NULL) {
				fprintf (stderr,
					 "Chyba: Nepodarilo sa alokovat pamat.\n");
				for (j = 0; j < i; j++) {
					free ((void *) (cst->elems[j]));
					cst->elems[j] = NULL;
				}
				free ((void *) (cst->elems));
				cst->elems = NULL;
				cst->count = 0;
				return 0;
			}
			for (j = 0; j <= pe - 1; j++) {
				cst->elems[i][j] = *(pom_st + j);
			}
			cst->elems[i][pe] = '\0';
			pom_st = pom + 1;
			i++;
		}
		pom++;
	}
	pe = (int)(pom - pom_st);
	cst->elems[i] = (char *) malloc (sizeof (char) * (pe + 1));

	if (cst->elems[i] == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		for (j = 0; j < i; j++) {
			free ((void *) (cst->elems[j]));
			cst->elems[j] = NULL;
		}
		free ((void *) (cst->elems));
		cst->elems = NULL;
		cst->count = 0;
		return 0;
	}
	for (j = 0; j <= pe - 1; j++) {
		cst->elems[i][j] = *(pom_st + j);
	}
	cst->elems[i][pe] = '\0';
	cst->count = count;
	return 1;
}

void
free_cesty (struct cesty *cst)
{
	int     i;

	for (i = 0; i < cst->count; i++) {
		if (cst->elems[i]) {
			free ((void *) (cst->elems[i]));
			cst->elems[i] = NULL;
		}
	}
	free ((void *) (cst->elems));
	cst->elems = NULL;
	cst->count = 0;
}
