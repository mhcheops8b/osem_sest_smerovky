#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chrslovo.h"

int
add_chr_slovo_list (struct chr_slovo_list **list, char *slovo)
{

	struct chr_slovo_list **pom, *pom1 = *list;

	pom = (struct chr_slovo_list **)
		malloc (sizeof (struct chr_slovo_list *));

	if (pom == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(chr_slovo_list).\n");
		return 0;
	}
	*pom = (struct chr_slovo_list *)
		malloc (sizeof (struct chr_slovo_list));

	if (*pom == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(chr_slovo_list).\n");
		free ((void *) pom);
		pom = NULL;
		return 0;
	}
	(*pom)->slovo = (char *) malloc (strlen (slovo) + 1);
	if ((*pom)->slovo == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat(chr_slovo_list).\n");
		free ((void *) *pom);
		*pom = NULL;
		return 0;
	}
	else {
		strcpy ((*pom)->slovo, slovo);
		(*pom)->next = pom1;
	}
	*list = *pom;
	return 1;
}

void
free_chr_slovo_list (struct chr_slovo_list **list)
{

	struct chr_slovo_list *pom = *list;

	while (pom) {
		pom = pom->next;
		if ((*list)->slovo) {
			free ((void *) (*list)->slovo);
			(*list)->slovo = NULL;
		}
		free ((void *) *list);
		*list = pom;
	}
}

int
remove_chr_slovo_from_list (struct chr_slovo_list **list,
			    struct chr_slovo_list *rm)
{

	struct chr_slovo_list *pom = *list, *pom_prev = NULL;

	if (!rm) {
		fprintf (stderr,
			 "Chyba: Nemozno zo zoznamu odstranit NULL!.\n");
		return 0;
	}

	while (pom && pom != rm) {
		pom_prev = pom;
		pom = pom->next;
	}
	if (!pom) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa najst zlozku na odstranenie.\n");
		return 0;
	}
	else {
		if (pom_prev) {
			pom_prev->next = pom->next;
			free ((void *) pom->slovo);
			pom->slovo = NULL;
			free ((void *) pom);
			pom = NULL;
			return 1;
		}
		else {
			*list = pom->next;
			free ((void *) pom->slovo);
			pom->slovo = NULL;
			free ((void *) pom);
			pom = NULL;
			return 1;
		}
	}
}
