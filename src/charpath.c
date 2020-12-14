#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "charpath.h"
//#define _CRT_SECURE_NO_WARNINGS
char   *default_char[] = { "C0" };
char   *vlnka_char[] = { "CI+", "CI-", "CI++-", "CI--+" };
int
create_char_path (struct char_path **pth, char *elem)
{
	*pth = (struct char_path *) malloc (sizeof (struct char_path));
	if (*pth == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre char_paths.\n");
		return 0;
	}
	(*pth)->elem = (char *) malloc (strlen (elem) + 1);
	if ((*pth)->elem == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		free ((void *) *pth);
		*pth = NULL;
		return 0;
	}
	strcpy ((*pth)->elem, elem);
	(*pth)->next = NULL;
	return 1;
}

int
add_char_path (struct char_path **pth, char *elem)
{
	struct char_path *pom = *pth;
	*pth = (struct char_path *) malloc (sizeof (struct char_path));
	if (*pth == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre char_paths.\n");
		return 0;
	}
	(*pth)->elem = (char *) malloc (strlen (elem) + 1);
	if ((*pth)->elem == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		free ((void *) *pth);
		*pth = NULL;
		return 0;
	}
	strcpy ((*pth)->elem, elem);
	(*pth)->next = pom;
	return 1;
}

void
free_char_path (struct char_path *pth)
{
	struct char_path *pom = pth;

	while (pom) {
		pth = pth->next;
		free ((void *) pom->elem);
		pom->elem = NULL;
		free ((void *) pom);
		pom = pth;
	}
}
int
create_char_path_chrs (struct char_path **pth, int count, char **ps)
{
	int     i;

	if (count < 1) {
		fprintf (stderr, "Chyba: Pocet musi byt nezaporny.\n");
		return 0;
	}
	if (!create_char_path (pth, ps[0]))
		return 0;
	for (i = 1; i < count; i++) {
		if (!add_char_path (pth, ps[i])) {
			free_char_path (*pth);
			return 0;
		}
	}
	return 1;
}

int
init_char_path_defaults ()
{
	if (!create_char_path_chrs (&default_char_path, 1, default_char)) {
		return 0;
	}
	if (!create_char_path_chrs (&vlnka_char_path, 4, vlnka_char)) {
		free_char_path (default_char_path);
		return 0;
	}
	return 1;
}
