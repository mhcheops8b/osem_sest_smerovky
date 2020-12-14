#include <stdio.h>
#include <string.h>
#include "table.h"
ELEM    all_elem[] = {
	{ 'A',  1} , { 'B',  4} , { 'C',  5} , { 'D',  7} , { 'E', 11} ,
	{ 'F', 13} , { 'G', 14} , { 'H', 15} , { 'I', 17} , { 'J', 19} ,
	{ 'K', 20} , { 'L', 21} , { 'M', 24} , { 'N', 25} , { 'O', 27} ,
	{ 'P', 30} , { 'Q', 31} , { 'R', 32} , { 'S', 34} , { 'T', 36} ,
	{ 'U', 38} , { 'V', 40} , { 'W', 41} , { 'X', 42} , { 'Y', 43} ,
	{ 'Z', 45}
};

struct table all_table = { 26, all_elem };

ELEM    dlz_elem[] = {
	{ 'A',  2} , { 'E', 12} , { 'I', 18} , { 'L', 22} , { 'O', 28} ,
	{ 'R', 33} , { 'U', 39} , { 'Y', 44} , { '?', 50} 
}; 

struct table dlz_table = { 10, dlz_elem };

ELEM    mak_elem[] = {
	{ 'C',  6} , { 'D',  8} , { 'L', 23} , { 'N', 26} , { 'O', 29} ,
	{ 'S', 35} , { 'T', 37} , { 'Z', 46}
};

struct table mak_table = { 8, mak_elem };

ELEM    bod_elem[] = {
	{ 'A', 3} , { 'U', 49}
};

struct table bod_table = { 2, bod_elem };

int
is_in_table (struct table *tbl, char key)
{
	int     i;

	for (i = 0; i < tbl->pocet; i++)
		if (tbl->hodn[i].key == key)
			return 1;
	return 0;
}

int
get_code_from_table (struct table *tbl, char key, int *code)
{
	int     i;

	for (i = 0; i < tbl->pocet; i++)
		if (tbl->hodn[i].key == key) {
			*code = tbl->hodn[i].value;
			return 1;
		}
	return 0;
}

int
check_char (char **poc, struct table *tbl, char *msg_1, char *msg_2)
{
	char   *pom = *poc;

	pom++;
	if (*pom) {
		if (!is_in_table (tbl, *pom)) {
			fprintf (stderr, "Chyba: %s\n", msg_1);

			/*
			 * Za
			 * ,
			 * musi
			 * byt
			 * pismeno
			 * ktore
			 * ma
			 * dlzen.\n");
			 */
			return 0;
		}
		else {
			*poc = pom;
			return 1;
		}
	}
	else {
		fprintf (stderr, "Chyba: %s\n", msg_2);

		/*
		 * Za
		 * ,
		 * musi
		 * nasledovat
		 * pismeno.\n");
		 */
		return 0;
	}
}
int
check_char_2 (char **poc)
{
	char    pom_s[4] = { 0, 0, 0, 0 };
	char   *pom = *poc;
	int     check = 0;

	pom++;
	if (!*pom) {
		fprintf (stderr, "Chyba: Za ` musia nasledovat 2 pismena.\n");
		return 0;
	}
	pom_s[0] = *pom;
	pom++;
	if (!*pom) {
		fprintf (stderr, "Chyba: Za ` musia nasledovat 2 pismena.\n");
		return 0;
	}
	pom_s[1] = *pom;
	if (!strcmp (pom_s, "CH"))
		check = 1;
	if (!strcmp (pom_s, "DZ"))
		check = 2;
	if (!strcmp (pom_s, "Dv")) {
		pom++;
		if (!*pom) {
			fprintf (stderr,
				 "Chyba: Za `Dv musi nasledovat pismeno.\n");
			return 0;
		}
		if (*pom != 'Z') {
			fprintf (stderr,
				 "Chyba: Za `Dv musi nasledovat pismeno Z.\n");
			return 0;
		}
		else
			check = 3;
	}			/* end DvZ */
	if (!check) {
		fprintf (stderr,
			 "Chyba: Za ` musi nasledovat jedno z CH DZ DvZ.\n");
		return 0;
	}
	*poc = pom;
	return check;
}


/*
    parse_letter - read one `letter` 

    input - beginning of parsing

    int_code - internal representational code
    let_let - length of string reprezentation

*/
int
parse_letter (char *input, int *int_code, int *let_len)
{
	char   *pom = input;

	switch (*input) {
	case '.':
		*int_code = EMPTY_LETTER;
		*let_len = 1;
		break;
	case '?':
		*int_code = QUEST_LETTER;
		*let_len = 1;
		break;
	case ',':
		pom++;
		if (!get_code_from_table (&dlz_table, *pom, int_code)) {
			fprintf (stderr,
				 "Chyba: Za `,` musi nasledovat pismeno, ktore ma dlzen.\n");
			return -1;
		}
		*let_len = 2;
		break;
	case 'v':
		pom++;
		if (!get_code_from_table (&mak_table, *pom, int_code)) {
			fprintf (stderr,
				 "Chyba: Za `v` musi nasledovat pismeno, ktore ma makcen.\n");
			return -1;
		}
		*let_len = 2;
		break;
	case '`':
		pom++;
		if (!strncmp (pom, "CH", 2)) {
			*int_code = CH_LETTER;
			*let_len = 3;
		}

		else if (!strncmp (pom, "DZ", 2)) {
			*int_code = DZ_LETTER;
			*let_len = 3;
		}

		else if (!strncmp (pom, "DvZ", 3)) {
			*int_code = DZZ_LETTER;
			*let_len = 4;
		}

		else {
			fprintf (stderr,
				 "Chyba: Za ` musi nasledovat CH, DZ alebo DvZ.\n");
			return -1;
		}
		break;
	case ':':
		pom++;
		if (!get_code_from_table (&bod_table, *pom, int_code)) {
			fprintf (stderr,
				 "Chyba: Za `:` musi nasledovat pismeno A.\n");
			return -1;
		}
		*let_len = 2;
		break;
	default:
		if (*input >= 'A' && *input <= 'Z') {	/* ASCII */
			get_code_from_table (&all_table, *input, int_code);
			*let_len = 1;
		}

		else {
			fprintf (stderr, "Chyba: Neznamy znak na vstupe.\n");
			return -1;
		}
	}
	return 0;
}
