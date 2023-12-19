#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "num_path.h"
#include "smery.h"

void
init_path_numpath (struct num_path *pth)
{
	pth->is_o_path = 0;
	pth->for_elems = 0;
	pth->cyc_elems = 0;
	pth->for_smery_nums = NULL;
	pth->cyc_smery_nums = NULL;
}

void
free_path_numpath (struct num_path *pth)
{
	pth->is_o_path = 0;
	pth->for_elems = 0;
	pth->cyc_elems = 0;
	if (pth->for_smery_nums) {
		free ((void *) pth->for_smery_nums);
		pth->for_smery_nums = NULL;
	}
	if (pth->cyc_smery_nums) {
		free ((void *) pth->cyc_smery_nums);
		pth->cyc_smery_nums = NULL;
	}
}


/* Allocates memory to store numeric path representation 
 * Parameters: 
 * 		pth - pointer to numeric path structure
 * 		for_elems - number of elements in for_part
 * 		cyc_elems - number of elements in cyc_part
 */
int
create_path_numpath (struct num_path *pth, int for_elems, int cyc_elems)
{
	if (for_elems) {
		pth->for_smery_nums =
			(int *) malloc (for_elems * sizeof (int));
		if (pth->for_smery_nums == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate pri vytvarani pola for_smerov.\n");
			init_path_numpath (pth);
			return 0;
		}
	}
	else
		pth->for_smery_nums = NULL;
	if (cyc_elems) {
		pth->cyc_smery_nums =
			(int *) malloc (cyc_elems * sizeof (int));
		if (pth->cyc_smery_nums == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate pri vytvarani pola cyc_smerov.\n");
			if (for_elems)
				free_path_numpath (pth);

			else
				init_path_numpath (pth);
			return 0;
		}
	}
	else
		pth->cyc_smery_nums = NULL;

	if (for_elems || cyc_elems) {
		pth->for_elems = for_elems;
		pth->cyc_elems = cyc_elems;
		return 1;
	}
	else {
		fprintf (stderr, "Chyba: Vytvara sa cesta nulovej dlzky.\n");
		init_path_numpath (pth);
		return 0;
	}
}

int
create_path2_numpath (struct num_path *pth, int for_elems, int cyc_elems,
		      int is_o_path)
{
	pth->is_o_path = is_o_path;
	if (for_elems) {
		pth->for_smery_nums =
			(int *) malloc (for_elems * sizeof (int));
		if (pth->for_smery_nums == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate pri vytvarani pola for_smerov.\n");
			init_path_numpath (pth);
			return 0;
		}
	}
	else
		pth->for_smery_nums = NULL;
	if (cyc_elems) {
		pth->cyc_smery_nums =
			(int *) malloc (cyc_elems * sizeof (int));
		if (pth->cyc_smery_nums == NULL) {
			fprintf (stderr,
				 "Chyba: Nedostatok pamate pri vytvarani pola cyc_smerov.\n");
			if (for_elems)
				free_path_numpath (pth);

			else
				init_path_numpath (pth);
			return 0;
		}
	}
	else
		pth->cyc_smery_nums = NULL;
	if (for_elems || cyc_elems) {
		pth->for_elems = for_elems;
		pth->cyc_elems = cyc_elems;
		return 1;
	}
	else {
		fprintf (stderr, "Chyba: Vytvara sa cesta nulovej dlzky.\n");
		init_path_numpath (pth);
		return 0;
	}
}

int
add_to_path_for_numpath (struct num_path *pth, int for_pos, int dir_num)
{
	if (pth->for_elems <= for_pos) {
		fprintf (stderr,
			 "Chyba: Nemozno pridat na poziciu %d, pretoze cesta ma %d prvkov.\n",
			 for_pos, pth->for_elems);
		return 0;
	}
	pth->for_smery_nums[for_pos] = dir_num;
	return 1;
}

int
add_to_path_cyc_numpath (struct num_path *pth, int cyc_pos, int dir_num)
{
	if (pth->cyc_elems <= cyc_pos) {
		fprintf (stderr,
			 "Chyba: Nemozno pridat na poziciu %d, pretoze cesta ma %d prvkov.\n",
			 cyc_pos, pth->cyc_elems);
		return 0;
	}
	pth->cyc_smery_nums[cyc_pos] = dir_num;
	return 1;
}

int
create_path_str_numpath (struct num_path *pth, int zac_dir, char *prg)
{
	char   *pom = prg, *for_s, *cyc_s, *end_s;
	int     for_l,		/* dlzka fyzickej  casti */
	        cyc_l,		/* dlzka cyklickej casti */
	        fors = 0, cycs = 0, i, j, dir_val = zac_dir;

	if (*prg == '\0') {
		fprintf (stderr, "Chyba: Prazdny retazec.\n");
		init_path_numpath (pth);
		return 0;
	}
	if (*pom != 'C' && *pom != 'F') {
		fprintf (stderr,
			 "Chyba: Retazec musi zacinat bud s F alebo C.\n");
		init_path_numpath (pth);
		return 0;
	}
	for_s = strchr (pom, 'F');	/* zaciatok fyzickej Ëasti, prÌpadne NULL, ak nie je */
	cyc_s = strchr (pom, 'C');	/* zaËiatok cyklickej Ëasti, prÌpadne NULL, ak nie je */
	end_s = &pom[strlen (pom) - 1];	/* koniec reùazca öpecifikuj˙ceho cestu */

	/* nastav dÂûku fyzickej Ëasti */
	if (for_s == NULL)	/* neobsahuje fyzick˙ Ëasù */
		for_l = 0;

	else if (cyc_s == NULL)	/* neobsahuje cyklick˙ Ëasù t.j.
				   * dÂûka je po koniec reùazca
				 */
		for_l = (int)(end_s - for_s);

	else			/* dÂûka po zaËiatok cyklickej Ëasti */
		for_l = (int)(cyc_s - for_s);

	/* nastav dlzku cyklickej Ëasti */
	if (cyc_s == NULL)	/* neobsahuje cyklick˙ Ëasù */
		cyc_l = 0;

	else
		cyc_l = (int)(end_s - cyc_s);	/* od zaËiatku po koniec reùazca */
	if (for_s)
		fors = (int)(for_s - pom);
	if (cyc_s)
		cycs = (int)(cyc_s - pom);
	if (!create_path_numpath (pth, for_l, cyc_l)) {
		init_path_numpath (pth);
		return 0;
	}
	else {
		j = -1;
		for (i = 0; i < for_l; i++) {
			switch (prg[fors + i + 1]) {
			case 'P':
				j++;
				i++;
				if (prg[fors + i + 1] < '0'
				    || prg[fors + i + 1] > '9') {
					fprintf (stderr,
						 "Chyba: V specifikacii cesty.\n");
					init_path_numpath (pth);
					return 0;
				}
				adjust_pos_dirno (&dir_val,
						  prg[fors + i + 1] - '0');
				break;
			case 'M':
				j++;
				i++;
				if (prg[fors + i + 1] < '0'
				    || prg[fors + i + 1] > '9') {
					fprintf (stderr,
						 "Chyba: V specifikacii cesty.\n");
					init_path_numpath (pth);
					return 0;
				}
				adjust_neg_dirno (&dir_val,
						  prg[fors + i + 1] - '0');
				break;
			case '+':
				j++;
				adjust_pos_dirno (&dir_val, 2);
				break;
			case '-':
				j++;
				adjust_neg_dirno (&dir_val, 2);
				break;
			case '0':
				j++;
				break;
			case 'I':
				dir_val = zac_dir;
				j++;
				break;
			default:
				fprintf (stderr,
					 "Chyba: retazec obsahuje nepovoleny znak.\n");
				init_path_numpath (pth);
				return 0;
			}
			add_to_path_for_numpath (pth, j, dir_val);
		}
		for (i = 0; i < cyc_l; i++) {
			switch (prg[cycs + i + 1]) {
			case '+':
				adjust_pos_dirno (&dir_val, 2);
				break;
			case '-':
				adjust_neg_dirno (&dir_val, 2);
				break;
			case '0':
				break;
			case 'I':
				dir_val = zac_dir;
				break;
			default:
				fprintf (stderr,
					 "Chyba: retazec obsahuje nepovoleny znak.\n");
				init_path_numpath (pth);
				return 0;
			}
			add_to_path_cyc_numpath (pth, i, dir_val);
		}
		return 1;
	}
}
int
create_path_str_ver2_numpath (struct num_path *pth, int zac_dir, char *prg)
{
	char   *pom = prg, *for_s, *cyc_s, *end_s;
	int     for_l,		/* length of physical part */
	        cyc_l,		/* length of cyclic part  */
		fors = 0, cycs = 0, i, j, dir_val =
		zac_dir, new_dir_val, dir_len;

	if (*prg == '\0') {
		fprintf (stderr, "Chyba: Prazdny retazec.\n");
		init_path_numpath (pth);
		return 0;
	}
	if (*pom != 'C' && *pom != 'F') {
		fprintf (stderr,
			 "Chyba: Retazec musi zacinat bud s F alebo C.\n");
		init_path_numpath (pth);
		return 0;
	}
	for_s = strchr (pom, 'F');	/* zaËiatok fyzickej Ëasti, prÌpadne NULL, ak nie je */
	cyc_s = strchr (pom, 'C');	/* zaËiatok cyklickej Ëasti, prÌpadne NULL, ak nie je */
	end_s = &pom[strlen (pom) - 1];	/* koniec reùazca öpecifikuj˙ceho cestu */

	/* nastav dÂûku fyzickej Ëasti */
	if (for_s == NULL)	/* neobsahuje fyzick˙ Ëasù */
		for_l = 0;

	else if (cyc_s == NULL)	/* neobsahuje cyklick˙ Ëasù t.j.
				   * dÂûka je po koniec reùazca
				 */
		for_l = (int)(end_s - for_s);

	else			/* dÂûka po zaËiatok cyklickej Ëasti */
		for_l = (int)(cyc_s - for_s);

	/* nastav dÂûku cyklickej Ëasti */
	if (cyc_s == NULL)	/* neobsahuje cyklick˙ Ëasù */
		cyc_l = 0;

	else
		cyc_l = (int)(end_s - cyc_s);	/* od zaËiatku po koniec reùazca */
	if (for_s)
		fors = (int)(for_s - pom);
	if (cyc_s)
		cycs = (int)(cyc_s - pom);
	if (!create_path_numpath (pth, for_l, cyc_l)) {
		init_path_numpath (pth);
		return 0;
	}

	else {
		j = 0;
		for (i = 0; i < for_l; i++) {
			if (parse_path_dir_str
			    (&prg[fors + i + 1], zac_dir, dir_val, 1,
			     &dir_len, &new_dir_val) != PATH_OK) {
				init_path_numpath (pth);
				return 0;
			};
			dir_val = new_dir_val;
			i += dir_len - 1;
			add_to_path_for_numpath (pth, j, dir_val);
			j++;
		}
		j = 0;
		for (i = 0; i < cyc_l; i++) {
			if (parse_path_dir_str
			    (&prg[cycs + i + 1], zac_dir, dir_val, 1,
			     &dir_len, &new_dir_val) != PATH_OK) {
				init_path_numpath (pth);
				return 0;
			}
			dir_val = new_dir_val;
			i += dir_len - 1;
			add_to_path_cyc_numpath (pth, j, dir_val);
			j++;
		}
		return 1;
	}
}

int
get_dir_num_numpath (struct num_path *pth, int dir_pos, int *new_dir_num)
{
	if (dir_pos < pth->for_elems) {
		*new_dir_num = pth->for_smery_nums[dir_pos];
		return 1;
	}
	dir_pos -= pth->for_elems;
	if (!pth->cyc_elems) {
		fprintf (stderr, "Chyba: Neexistujuca zlozka v ceste.\n");
		return 0;
	}

	else {
		*new_dir_num = pth->cyc_smery_nums[dir_pos % pth->cyc_elems];
		return 1;
	}
}


/* 
    Parse path direction

    path        - current position
    start_dir   - start direction - important for I
    current_dir - current direction - relative shift after +,-,P,M,0

    i_allowed   - YES (=1)
                  NO  (=0)  
             
    dir_len     - lenght of readed direction  
                    one for +,-,0,I, two for P<NUM>, M<NUM>
    dir_val     - direction after change


    Return value:
        0 - ok path has correct syntax
       -1 - syntax error
*/
int
parse_path_dir_str (char *path, int zac_dir, int cur_dir, int i_allowed,
		    int *dir_len, int *dir_val)
{
	char   *pom_path = path;
	int     pom_dir_val = cur_dir, pom_val;

	switch (*pom_path) {
	case '0':
		*dir_len = 1;
		*dir_val = cur_dir;
		return PATH_OK;
	case 'I':
		if (i_allowed) {
			*dir_len = 1;
			*dir_val = zac_dir;
			return PATH_OK;
		}
		fprintf (stderr,
			 "`I` could not be used in path specification.\n");
		return PATH_ER;
	case '+':
		*dir_len = 1;
		adjust_pos_dirno (&pom_dir_val, 2);
		*dir_val = pom_dir_val;
		return PATH_OK;
	case '-':
		*dir_len = 1;
		adjust_neg_dirno (&pom_dir_val, 2);
		*dir_val = pom_dir_val;
		return PATH_OK;
	case 'P':
		pom_path++;
		if (*pom_path < '0' || *pom_path > '7') {
			fprintf (stderr,
				 "Chyba: za P musi nasledovat cislo smeru (0-7).\n");
			return PATH_ER;
		}
		pom_val = *pom_path - '0';
		adjust_pos_dirno (&pom_dir_val, pom_val);
		*dir_len = 2;
		*dir_val = pom_dir_val;
		return PATH_OK;
	case 'M':
		pom_path++;
		if (*pom_path < '0' || *pom_path > '7') {
			fprintf (stderr,
				 "Chyba: za P musi nasledovat cislo smeru (0-7).\n");
			return PATH_ER;
		}
		pom_val = *pom_path - '0';
		adjust_neg_dirno (&pom_dir_val, pom_val);
		*dir_len = 2;
		*dir_val = pom_dir_val;
		return PATH_OK;
	default:
		fprintf (stderr,
			 "Chyba: Neznamy znak v specifikacii cesty. Povolene su 0,I,P,M,+,-,1,2,3,4,5,6,7.\n");
		return PATH_ER;
	}
}
int
create_path_str2_numpath (struct num_path *pth, int zac_dir, char *prg)
{
	char   *pom = prg;
	int     for_l, cyc_l, i, dir_val = zac_dir;

	if (*prg == '\0') {
		fprintf (stderr, "Chyba: Prazdny retazec.\n");
		init_path_numpath (pth);
		return 0;
	}
	if (*pom != 'C' && *pom != 'F' && *pom != 'O') {
		fprintf (stderr,
			 "Chyba: Retazec musi zacinat bud s F , C  alebo O.\n");
		init_path_numpath (pth);
		return 0;
	}
	if (!test_program_sections (prg)) {
		init_path_numpath (pth);
		return 0;
	}
	if (prg[0] == 'C' || prg[0] == 'F') {
		char   *for_s, *cyc_s;

		for_s = strchr (pom, 'F');
		cyc_s = strchr (pom, 'C');
		for_l = get_length_path (for_s);
		cyc_l = get_length_path (cyc_s);
		if (for_s == NULL)
			for_l = 0;
		if (cyc_s == NULL)
			cyc_l = 0;
		if (!create_path_numpath (pth, for_l, cyc_l)) {
			init_path_numpath (pth);
			return 0;
		}
		else {
			if (for_l) {
				pom = for_s + 1;
				i = 0;
				while (*pom && *pom != 'C') {
					if (!get_dir_val
					    (&pom, zac_dir, dir_val,
					     &dir_val)) {
						init_path_numpath (pth);
						return 0;
					}
					add_to_path_for_numpath (pth, i,
								 dir_val);
					pom++;
					i++;
				}
			}
			if (cyc_l) {
				pom = cyc_s + 1;
				i = 0;
				while (*pom) {
					if (!get_dir_val
					    (&pom, zac_dir, dir_val,
					     &dir_val)) {
						init_path_numpath (pth);
						return 0;
					}
					add_to_path_cyc_numpath (pth, i,
								 dir_val);
					pom++;
					i++;
				}
			}
		}
	}

	else {			/* O cesta */
		int     o_l;	/* dlzka O cesty */

		o_l = get_length_path (prg);
		if (!o_l) {
			fprintf (stderr, "Chyba: Cesta je prazdna.\n");
			init_path_numpath (pth);
			return 0;
		}
		if (!create_path2_numpath (pth, o_l, 0, IS_O_PATH)) {
			init_path_numpath (pth);
			return 0;
		}
		pom = prg + 1;
		i = 0;
		while (*pom) {
			if (!get_dir_val (&pom, zac_dir, dir_val, &dir_val)) {
				init_path_numpath (pth);
				return 0;
			}
			add_to_path_for_numpath (pth, i, dir_val);
			pom++;
			i++;
		}
	}
	return 1;
}

int
test_program_sections (char *prg)
{
	int     c_count, f_count, o_count;
	char   *pom = prg;

	c_count = f_count = o_count = 0;
	while (*pom) {
		if (*pom == 'C')
			c_count++;
		if (*pom == 'F')
			f_count++;
		if (*pom == 'O')
			o_count++;
		pom++;
	}
	if (c_count > 1 || f_count > 1 || o_count > 1) {
		fprintf (stderr,
			 "Chyba: C, F , O sa mozu v programe nachadzat nanajvys raz.\n");
		return 0;
	}
	if (o_count && (c_count || f_count)) {
		fprintf (stderr,
			 "Chyba: Pri specifikatore O sa nemozu nachadzat dalsie ine.\n");
		return 0;
	}
	return 1;
}

int
get_length_path (char *path)
{
	char   *pom = path;
	int     pocet = 0;

	if (pom == NULL)
		return 0;
	if (*pom == 'C' || *pom == 'F' || *pom == 'O')
		pom++;

	else
		return 0;
	while (*pom && *pom != 'C' && *pom != 'F' && *pom != 'O') {
		switch (*pom) {
		case 'I':
		case '0':
		case '+':
		case '-':
			pocet++;
			pom++;
			break;
		case 'P':
		case 'M':
			pocet++;
			pom += 2;
			break;
		}
	}
	return pocet;
}

int
get_dir_val (char **prg, int zac_dir, int old_val, int *new_val)
{
	char   *pom = *prg;
	int     dir_val = old_val;

	switch (*pom) {
	case 'P':
		pom++;
		if (*pom < '0' || *pom > '9') {
			fprintf (stderr,
				 "Chyba: V specifikacii cesty. Za P(M) sa ocakava cislo od 0-9.\n");
			return 0;
		}
		adjust_pos_dirno (&dir_val, *pom - '0');
		break;
	case 'M':
		pom++;
		if (*pom < '0' || *pom > '9') {
			fprintf (stderr,
				 "Chyba: V specifikacii cesty. Za P(M) sa ocakava cislo od 0-9.\n");
			return 0;
		}
		adjust_neg_dirno (&dir_val, *pom - '0');
		break;
	case '+':
		adjust_pos_dirno (&dir_val, 2);
		break;
	case '-':
		adjust_neg_dirno (&dir_val, 2);
		break;
	case '0':
		break;
	case 'I':
		dir_val = zac_dir;
		break;
	default:
		fprintf (stderr,
			 "Chyba: retazec obsahuje nepovoleny znak.\n");
		return 0;
		break;
	}
	*prg = pom;
	*new_val = dir_val;
	return 1;
}

void
print_path_numpath (struct num_path *pth)
{
	int     i;

	if (pth->is_o_path) {
		printf ("O cesta.\n");
	}
	printf ("F: %d\n", pth->for_elems);
	for (i = 0; i < pth->for_elems; i++)
		printf ("%d ", pth->for_smery_nums[i]);
	printf ("\n");

	printf ("C: %d\n", pth->cyc_elems);
	for (i = 0; i < pth->cyc_elems; i++)
		printf ("%d ", pth->cyc_smery_nums[i]);
	printf ("\n");

}

int
rol_path (struct num_path *pth)
{
	int     pom, i;

	if (!pth->is_o_path) {
		fprintf (stderr,
			 "Chyba: Nemozno rotovat cestu, ktora nie je O cesta.\n");
		return 0;
	}
	pom = pth->for_smery_nums[0];
	for (i = 1; i < pth->for_elems; i++) {
		pth->for_smery_nums[i - 1] = pth->for_smery_nums[i];
	}
	pth->for_smery_nums[pth->for_elems - 1] = pom;
	return 1;
}
