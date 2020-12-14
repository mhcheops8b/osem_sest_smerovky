#include <stdio.h>
#include <stdlib.h>
#include "pismeno.h"
#include "slovo.h"
int
is_palindrom (struct slovo *slv)
{
	int     i;

	i = 0;
	while (i < slv->count - i - 1) {
		if ((slv->pism[i]).l != (slv->pism[slv->count - i - 1]).l)
			return 0;	/* nie je */
		i++;
	}
	return 1;
}

int
create_slovo (char *poc, struct slovo *slv)
{
	int     count;
	int     i = 0, pom_c, val = 0;
	char   *pom = poc;

	if (!(count = count_slovo (poc))) {
		slv->count = 0;
		slv->pism = NULL;
		return 0;
	}
	slv->pism =
		(struct pismeno *) malloc ((count + 1) *
					   sizeof (struct pismeno));
	if (slv->pism == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		return 0;
	}
	slv->count = count;
	while (*pom) {
		switch (*pom) {
		case '-':
			slv->pism[i].l = 48;
			slv->pism[i++].id = 0;
			break;
		case '.':
			slv->pism[i].l = EMPTY_CHAR;
			slv->pism[i++].id = 0;
			break;
		case '?':
			slv->pism[i].l = QUEST_CHAR;
			slv->pism[i++].id = 0;
			break;
		case ',':
			pom++;
			slv->pism[i] = get_table (&dlz_table, *pom);
			slv->pism[i++].id = 0;
			break;
		case 'v':
			pom++;
			slv->pism[i] = get_table (&mak_table, *pom);
			slv->pism[i++].id = 0;
			break;
		case ':':
			pom++;
			slv->pism[i] = get_table (&bod_table, *pom);
			slv->pism[i++].id = 0;
			break;
		default:
			if (*pom == ' ' || *pom == '\t');

			else {
				slv->pism[i] = get_table (&all_table, *pom);
				slv->pism[i++].id = 0;
			}
			break;
		case '`':
			pom_c = check_char_2 (&pom);
			if (pom_c == 1)
				val = 16;
			if (pom_c == 2)
				val = 9;
			if (pom_c == 3)
				val = 10;
			slv->pism[i].id = 0;
			slv->pism[i++].l = val;
			break;
		}
		pom++;
	}
	return 1;
}

int
create_slovo_range (char *from, char *to, struct slovo *slv)
{
	int     count;
	int     i = 0, pom_c, val = 0;
	char   *pom = from;

	if (!(count = count_slovo_range (from, to))) {
		slv->count = 0;
		slv->pism = NULL;
		return 0;
	}

#ifdef OSM_DEBUG
	printf ("Creating syllabe: \n");
	for (pom = from; pom <= to; pom++)
		printf ("%c", *pom);
	printf ("\n");
	printf ("Count: %d\n", count);

#endif /*  */
	slv->pism =
		(struct pismeno *) malloc ((count + 1) *
					   sizeof (struct pismeno));
	if (slv->pism == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa alokovat pamat.\n");
		return 0;
	}
	slv->count = count;

#ifdef OSM_DEBUG
	pom = from;

#endif /*  */
	while (*pom && pom <= to) {
		switch (*pom) {
		case '-':
			slv->pism[i].l = 48;
			slv->pism[i++].id = 0;
			break;
		case '.':
			slv->pism[i].l = EMPTY_CHAR;
			slv->pism[i++].id = 0;
			break;
		case '?':
			slv->pism[i].l = QUEST_CHAR;
			slv->pism[i++].id = 0;
			break;
		case ',':
			pom++;
			slv->pism[i] = get_table (&dlz_table, *pom);
			slv->pism[i++].id = 0;
			break;
		case 'v':
			pom++;
			slv->pism[i] = get_table (&mak_table, *pom);
			slv->pism[i++].id = 0;
			break;
		case ':':
			pom++;
			slv->pism[i] = get_table (&bod_table, *pom);
			slv->pism[i++].id = 0;
			break;
		default:
			if (*pom == ' ' || *pom == '\t');

			else {
				slv->pism[i] = get_table (&all_table, *pom);
				slv->pism[i++].id = 0;
			}
			break;
		case '`':
			pom_c = check_char_2 (&pom);
			if (pom_c == 1)
				val = 16;
			if (pom_c == 2)
				val = 9;
			if (pom_c == 3)
				val = 10;
			slv->pism[i].id = 0;
			slv->pism[i++].l = val;
			break;
		}
		pom++;
	}
	return 1;
}

void
free_slovo (struct slovo *slv)
{
	if (slv->pism != NULL) {
		free ((void *) slv->pism);
		slv->pism = NULL;
		slv->count = 0;
	}
}
int
check_slovo (char *poc)
{
	char   *pom = poc;

	while (*pom) {
		switch (*pom) {
		case '-':
			break;
		case '.':
			break;
		case '?':
			break;
		case ',':
			if (!check_char
			    (&pom, &dlz_table,
			     "Za , musi byt pismeno ktore ma dlzen.",
			     "Za , musi nasledovat pismeno."))
				return 0;
			break;
		case 'v':
			if (!check_char
			    (&pom, &mak_table,
			     "Za v musi byt pismeno ktore ma makcen.",
			     "Za v musi nasledovat pismeno."))
				return 0;
			break;
		case ':':
			if (!check_char
			    (&pom, &bod_table,
			     "Za : musi byt pismeno ktore dvojbodku (a).",
			     "Za : musi nasledovat pismeno."))
				return 0;
			break;
		case '`':
			if (!check_char_2 (&pom))
				return 0;
			break;
		default:
			if (*pom != ' ' && *pom != '\t') {
				if (!is_in_table (&all_table, *pom)) {
					fprintf (stderr,
						 "Chyba: Zadanie obsahuje nepripustne znaky.\n");
					return 0;
				}
			}
			break;
		}		/*
				 * end
				 * switch
				 */
		pom++;
	}			/*
				   * end
				   * while
				 */
	return 1;
}

int
count_slovo (char *poc)
{
	int     count = 0;
	char   *pom = poc;

	if (!check_slovo (pom))
		return 0;
	pom = poc;
	while (*pom) {
		switch (*pom) {
		case '-':
			count++;
			break;
		case '.':
			count++;
			break;
		case '?':
			count++;
			break;
		case ',':
			check_char (&pom, &dlz_table,
				    "Za , musi byt pismeno ktore ma dlzen.",
				    "Za , musi nasledovat pismeno.");
			count++;
			break;
		case 'v':
			check_char (&pom, &mak_table,
				    "Za v musi byt pismeno ktore ma makcen.",
				    "Za v musi nasledovat pismeno.");
			count++;
			break;
		case ':':
			check_char (&pom, &bod_table,
				    "Za : musi byt pismeno ktore dvojbodku (a).",
				    "Za : musi nasledovat pismeno.");
			count++;
			break;
		case '`':
			check_char_2 (&pom);
			count++;
			break;
		default:
			if (*pom != ' ' && *pom != '\t')
				count++;
			break;
		}		/*
				 * end
				 * switch
				 */
		pom++;
	}			/*
				   * end
				   * while
				 */
	return count;
}

int
count_slovo_range (char *from, char *to)
{
	int     count = 0;
	char   *pom = from;

	if (!check_slovo (pom))
		return 0;
	pom = from;
	while (*pom && pom <= to) {
		switch (*pom) {
		case '-':
			count++;
			break;
		case '.':
			count++;
			break;
		case '?':
			count++;
			break;
		case ',':
			check_char (&pom, &dlz_table,
				    "Za , musi byt pismeno ktore ma dlzen.",
				    "Za , musi nasledovat pismeno.");
			count++;
			break;
		case 'v':
			check_char (&pom, &mak_table,
				    "Za v musi byt pismeno ktore ma makcen.",
				    "Za v musi nasledovat pismeno.");
			count++;
			break;
		case ':':
			check_char (&pom, &bod_table,
				    "Za : musi byt pismeno ktore dvojbodku (a).",
				    "Za : musi nasledovat pismeno.");
			count++;
			break;
		case '`':
			check_char_2 (&pom);
			count++;
			break;
		default:
			if (*pom != ' ' && *pom != '\t')
				count++;
			break;
		}		/* end switch */
		pom++;
	}			/* end while */
	return count;
}

void
print_slovo (SLOVO * slv)
{
	int     i;

	for (i = 0; i < slv->count; i++)
		printf ("%s", get_pism ((int) slv->pism[i].l));
}

void
fprint_slovo (SLOVO * slv, FILE * fil)
{
	int     i;

	for (i = 0; i < slv->count; i++)
		fprintf (fil, "%s", get_pism ((int) slv->pism[i].l));
}

/* Returns 1 - if they are same, 0 - in other cases */
int
compare_wrd_to_wrd (SLOVO * slv1, SLOVO * slv2)
{
	int     i;

	/* They have not same length */
	if (slv1->count != slv2->count)
		return 0;
	for (i = 0; i < slv1->count; i++)
		if (!compare_pism_to_pism (&slv1->pism[i], &slv2->pism[i]))
			return 0;
	return 1;
}

int
is_qword (SLOVO * wrd)
{
	int     i;

	for (i = 0; i < wrd->count; i++)
		if (wrd->pism[i].l == QUEST_CHAR)
			return 1;

	return 0;
}

int copy_slovo(SLOVO *new_sl, SLOVO *old_sl) {
	int i;	

	if (!old_sl) {
		fprintf(stderr, "Error: Could not copy NULL.\n");
		return -1;
	}

	if (old_sl->count==0) {
		new_sl->count=0;
		new_sl->pism=NULL;
		return 0;
	}
	new_sl->pism=(struct pismeno *)malloc(sizeof(struct pismeno)*old_sl->count);
	if (new_sl->pism == NULL) {
		fprintf(stderr, "Could not allocate memory(copy_slovo).\n");
		return -1;
	}
	
	for (i=0;i<old_sl->count;i++)
		new_sl->pism[i]=old_sl->pism[i];
	return 0;
}
