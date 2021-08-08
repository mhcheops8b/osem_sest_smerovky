#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flosm.h"
#include "obrfile.h"
#include "cesty.h"
#include "search.h"
#include "osm_ries.h"
#include "chrslovo.h"
#include "findcomm.h"
#include "lustikrz.h"
#include "osmtop.h"

const char *prg_default = "C0";

int
special_solution (OSEMSM * osm, char *line_buf, struct slovo *slv,
		  struct osm_ries *riesenie, int ignore_multiple,
		  int lusti_pocet)
{
	int     que = 0, nonque = 0, amb;
	struct osm_ries *pom_ries, *min_ries = NULL;
	struct chr_slovo_list *chr_list;

	count_quests (riesenie, &que, &nonque);
	if (!ignore_multiple && nonque > 1) {
		fprintf (stderr, "Chyba: V zadani ulohy.\n");
		fprintf (stderr, "Slovo ");
		fprint_slovo (slv, stderr);
		fprintf (stderr, " sa vyskytuje:\n");

		pom_ries = riesenie;
		while (pom_ries) {
			fprintf (stderr, "x:%d y:%d d:%d ?:%d p:%s\n",
				 pom_ries->pos_x, pom_ries->pos_y,
				 pom_ries->dir_num,
				 pom_ries->cnt_question_mark,
				 pom_ries->chr_path);
			pom_ries = pom_ries->next;
		}

		free_osm_ries (riesenie);
		riesenie = NULL;
		free_chr_slovo_list (&chr_list);
		free_kriz (osm);
		return 0;
	}
	else {
		if (nonque == lusti_pocet) {
			pom_ries = riesenie;
			while (pom_ries) {
				if (!pom_ries->cnt_question_mark)
					break;

				pom_ries = pom_ries->next;
			}

			select_kriz3 (osm, pom_ries->chr_path, slv,
				      pom_ries->pos_x, pom_ries->pos_y,
				      pom_ries->dir_num);
		}
		else {		/* * neobsahuje * ziadne * riesenie * bez * ?  */
			if (que > 1) {
				pom_ries = riesenie;
				/* * zisti * riesenie * s * najmensim * poctom * otaznikov */
				while (pom_ries) {
					if (min_ries == NULL)
						min_ries = pom_ries;
					else if (min_ries->cnt_question_mark >
						 pom_ries->cnt_question_mark)
						min_ries = pom_ries;
					pom_ries = pom_ries->next;
				}
				/* * je * takych * rieseni * viac * ?  */
				amb = 0;
				pom_ries = riesenie;
				while (pom_ries) {
					if (pom_ries != min_ries
					    && pom_ries->cnt_question_mark ==
					    min_ries->cnt_question_mark) {
						/* * Ano */
						amb = 1;
						break;
					}

					pom_ries = pom_ries->next;
				}

				if (amb)
					/* * Ak * ano * a nemas ignorovat viacere, tak pridaj * do * zoznamu */
					if (!ignore_multiple)
						add_chr_slovo_list (&chr_list,
								    line_buf);
					else {
						/* * vyskrtni * vsetky * minimalne */
						pom_ries = riesenie;
						while (pom_ries) {
							if (pom_ries->
							    cnt_question_mark
							    ==
							    min_ries->
							    cnt_question_mark)
							{
								dopln_quest
									(osm,
									 pom_ries,
									 slv);
								select_kriz3
									(osm,
									 pom_ries->
									 chr_path,
									 slv,
									 pom_ries->
									 pos_x,
									 pom_ries->
									 pos_y,
									 pom_ries->
									 dir_num);
							}

							pom_ries =
								pom_ries->
								next;
						}
					}
				else {
					dopln_quest (osm, min_ries, slv);
					select_kriz3 (osm, min_ries->chr_path,
						      slv, min_ries->pos_x,
						      min_ries->pos_y,
						      min_ries->dir_num);
				}
			}
			else {
				/* * que==1 * jedine * z * otaznikom */
				dopln_quest (osm, riesenie, slv);
				select_kriz3 (osm, riesenie->chr_path, slv,
					      riesenie->pos_x,
					      riesenie->pos_y,
					      riesenie->dir_num);
			}
		}
	}
	return 1;
}

int
special_solution_syl (OSEMSM_SYL * osm, char *line_buf, struct slovo *slv,
		      struct osm_ries *riesenie, int ignore_multiple,
		      int lusti_pocet)
{
	int     que = 0, nonque = 0, amb;
	struct osm_ries *pom_ries, *min_ries = NULL;
	struct chr_slovo_list *chr_list;

	count_quests (riesenie, &que, &nonque);
	if (!ignore_multiple && nonque > 1) {
		fprintf (stderr, "Chyba: V zadani ulohy.\n");
		fprintf (stderr, "Slovo ");
		fprint_slovo (slv, stderr);
		fprintf (stderr, " sa vyskytuje:\n");

		pom_ries = riesenie;
		while (pom_ries) {
			fprintf (stderr, "x:%d y:%d d:%d ?:%d p:%s\n",
				 pom_ries->pos_x, pom_ries->pos_y,
				 pom_ries->dir_num,
				 pom_ries->cnt_question_mark,
				 pom_ries->chr_path);
			pom_ries = pom_ries->next;
		}

		free_osm_ries (riesenie);
		riesenie = NULL;
		free_chr_slovo_list (&chr_list);
		free_kriz_syl (osm);
		return 0;
	}
	else {
		if (nonque == lusti_pocet) {
			pom_ries = riesenie;
			while (pom_ries) {
				if (!pom_ries->cnt_question_mark)
					break;

				pom_ries = pom_ries->next;
			}

			select_kriz3_syl (osm, pom_ries->chr_path, slv,
					  pom_ries->pos_x, pom_ries->pos_y,
					  pom_ries->dir_num);
		}
		else {		/* * neobsahuje * ziadne * riesenie * bez * ?  */
			if (que > 1) {
				pom_ries = riesenie;
				/* * zisti * riesenie * s * najmensim * poctom * otaznikov */
				while (pom_ries) {
					if (min_ries == NULL)
						min_ries = pom_ries;
					else if (min_ries->cnt_question_mark >
						 pom_ries->cnt_question_mark)
						min_ries = pom_ries;
					pom_ries = pom_ries->next;
				}
				/* * je * takych * rieseni * viac * ?  */
				amb = 0;
				pom_ries = riesenie;
				while (pom_ries) {
					if (pom_ries != min_ries
					    && pom_ries->cnt_question_mark ==
					    min_ries->cnt_question_mark) {
						/* * Ano */
						amb = 1;
						break;
					}

					pom_ries = pom_ries->next;
				}

				if (amb)
					/* * Ak * ano * a nemas ignorovat viacere, tak pridaj * do * zoznamu */
					if (!ignore_multiple)
						add_chr_slovo_list (&chr_list,
								    line_buf);
					else {
						/* * vyskrtni * vsetky * minimalne */
						pom_ries = riesenie;
						while (pom_ries) {
							if (pom_ries->
							    cnt_question_mark
							    ==
							    min_ries->
							    cnt_question_mark)
							{
								dopln_quest_syl
									(osm,
									 pom_ries,
									 slv);
								select_kriz3_syl
									(osm,
									 pom_ries->
									 chr_path,
									 slv,
									 pom_ries->
									 pos_x,
									 pom_ries->
									 pos_y,
									 pom_ries->
									 dir_num);
							}

							pom_ries =
								pom_ries->
								next;
						}
					}
				else {
					dopln_quest_syl (osm, min_ries, slv);
					select_kriz3_syl (osm,
							  min_ries->chr_path,
							  slv,
							  min_ries->pos_x,
							  min_ries->pos_y,
							  min_ries->dir_num);
				}
			}
			else {
				/* * que==1 * jedine * z * otaznikom */
				dopln_quest_syl (osm, riesenie, slv);
				select_kriz3_syl (osm, riesenie->chr_path,
						  slv, riesenie->pos_x,
						  riesenie->pos_y,
						  riesenie->dir_num);
			}
		}
	}
	return 1;
}

int
normal_solution (OSEMSM * osm, char *line_buf, struct slovo *slv,
		 struct osm_ries *riesenie, int lusti_pocet)
{
	int     que = 0, nonque = 0, i;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	struct osm_ries *pom_ries;
	
	/* TODO: 
	 * 	1.Add thourough checking and eliminationg the solutions 
	 *	2. In case of the greather number of solutions than 
	 *	expected, let user  choose which solutions should
	 *	be accepted.
	 */

	
	/* Number of solutions which do not(noque) or do(que) 
	 * contain questionmarks in them 
	 * */
	count_quests (riesenie, &que, &nonque);

	/* If there is not requested number of solutions - stop solving */
	if (que + nonque != lusti_pocet) {
		if (que + nonque < lusti_pocet)
			fprintf (stderr,
 "Chyba: V krizovke sa slovo `%s` vyskytuje len %d raz, pricom sa ocakava %d vyskytov slov.\n",
				 line_buf, que + nonque, lusti_pocet);
		else
			fprintf (stderr,
 "Chyba: V krizovke sa slovo `%s` vyskytuje %d raz, co je viackrat ako je pozadovane (%d).\n",
				 line_buf, que + nonque, lusti_pocet);
		free_kriz (osm);
		if (!def_cesta)
			free_cesty (&cst);
		return 0;
	}
	else { /* Otherwise mark fields and fill missing letters in case 
		  of question martks */
		
		pom_ries = riesenie;
		for (i = 0; i < que + nonque; i++) {
			if (pom_ries->cnt_question_mark)
				dopln_quest (osm, pom_ries, slv);

			select_kriz3 (osm, pom_ries->chr_path, slv,
				      pom_ries->pos_x, pom_ries->pos_y,
				      pom_ries->dir_num);
			pom_ries = pom_ries->next;
		}
	}
	return 1;
}

int
normal_solution_syl (OSEMSM_SYL * osm, char *line_buf, struct slovo *slv,
		     struct osm_ries *riesenie, int lusti_pocet)
{
	int     que = 0, nonque = 0, i;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	struct osm_ries *pom_ries;

	count_quests (riesenie, &que, &nonque);
	if (que + nonque != lusti_pocet) {
		if (que + nonque < lusti_pocet)
			fprintf (stderr,
				 "Chyba: V krizovke sa slovo vyskytuje len %d(%d) vyskytov slov.\n",
				 que + nonque, lusti_pocet);
		else
			fprintf (stderr,
				 "Chyba: V krizovke sa slovo %s vyskytuje viackrat ako je pozadovane [%d(%d)].\n",
				 line_buf, que + nonque, lusti_pocet);
		free_kriz_syl (osm);
		if (!def_cesta)
			free_cesty (&cst);
		return 0;
	}
	else {
		pom_ries = riesenie;
		for (i = 0; i < que + nonque; i++) {
			if (pom_ries->cnt_question_mark)
				dopln_quest_syl (osm, pom_ries, slv);

			select_kriz3_syl (osm, pom_ries->chr_path, slv,
					  pom_ries->pos_x, pom_ries->pos_y,
					  pom_ries->dir_num);
			pom_ries = pom_ries->next;
		}
	}
	return 1;
}

void
select_forb_limit (char *pom, int *forb_limit)
{
	pom++;
	/* skip ws characters */
	while (*pom && (*pom == ' ' || *pom == '\t'))
		pom++;

	if (!strncmp (pom, "HALF_EQ", strlen ("HALF_EQ"))) {
		*forb_limit = FORB_LIMIT_HALF_EQ;
		return;
	}

	if (!strncmp (pom, "HALF_GT", strlen ("HALF_GT"))) {
		*forb_limit = FORB_LIMIT_HALF_GT;
		return;
	}

	if (!strncmp (pom, "NONE", strlen ("NONE"))) {
		*forb_limit = FORB_LIMIT_NONE;
		return;
	}

	if (sscanf (pom, "%d", forb_limit) != 1) {
		fprintf (stderr, "Error: Unknown forb_limit value.\n");
		fprintf (stderr, "Ignoring forb_limit change.\n");
	}
#ifdef OSM_DEBUG
	printf ("Setting forb_limit to %d.\n", *forb_limit);
#endif
}

int
lusti_kriz_word_syl (OSEMSM_SYL * osm, char *line_buf, char *prg_buf,
		     int ignore_multiple, int special_sol, int line,
		     struct cesty *cst, int *def_cesta, int *lusti_pocet,
		     int *forb_lim, int *opt1, int *opt2)
{

	struct slovo slv;
	struct osm_ries *riesenie, *min_ries = NULL;

	strip_nl_char (line_buf);

	if (strlen (line_buf) > 0 && line_buf[0] != '#' && line_buf[0] != 't' && line_buf[0] != 'n' && line_buf[0] != 's' && line_buf[0] != 'f') {	/* ak je slovo */

		if (create_slovo (line_buf, &slv)) {
			min_ries = NULL;
#ifdef OSM_DEBUG
			printf ("Searching: ");
			print_slovo (&slv);
			printf ("\n");
#endif
			if (!search7_syl
			    (osm, &slv, cst, &riesenie, *opt1, *opt2,
			     *forb_lim)) {
				fprintf (stdout, "Nenaslo sa slovo na riadku %d.\n", line);	/* stderr */
				print_slovo (&slv);
				printf ("\n");
			}
			else {
				if (special_sol) {
					if (!special_solution_syl
					    (osm, line_buf, &slv, riesenie,
					     ignore_multiple, *lusti_pocet))
						return 0;
				}
				else {
					if (!normal_solution_syl
					    (osm, line_buf, &slv, riesenie,
					     *lusti_pocet))
						return 0;
				}
			}
			free_slovo (&slv);
			free_osm_ries (riesenie);
		}
		else {
			fprintf (stderr,
				 "Chyba na riadku %d: Pocas vytvarania slova.\n",
				 line);
			fprintf (stderr, "%s\n", line_buf);
		}
	}
	else {
		if (line_buf[0] == 't') {
			if (!select_type
			    (line_buf, prg_buf, line, def_cesta, cst)) {
				free_kriz_syl (osm);
				if (!*def_cesta)
					free_cesty (cst);
				return 0;
			}
		}
		else if (line_buf[0] == 's') {
		}
		else if (line_buf[0] == 'f') {
			select_forb_limit (line_buf, forb_lim);
		}
		else if (line_buf[0] == 'n') {
			if (sscanf (&line_buf[1], "%d", lusti_pocet) < 1) {
				fprintf (stderr,
					 "Chyba na riadku %d: Za 'n' musi nasledovat pocet lusteni daneho slova.\n",
					 line);
				free_kriz_syl (osm);
				if (!*def_cesta)
					free_cesty (cst);
				return 0;
			}

			if (*lusti_pocet < 1) {
				fprintf (stderr,
					 "Chyba na riadku %d: Pocet hladani slova musi byt kladny.(Nastavena default hodnota 1)\n",
					 line);
				*lusti_pocet = 1;
			}
		}
	}
	return 1;
}

int
lusti_kriz_word (OSEMSM * osm, char *line_buf, char *prg_buf,
		 int ignore_multiple, int special_sol, int line,
		 struct cesty *cst, int *def_cesta, int *lusti_pocet,
		 int *forb_lim, int *opt1, int *opt2)
{
	/*fprintf(stdout, "Lusti pocet: %d\n", *lusti_pocet);*/
	/*fprintf(stdout, "Special: %d\n", special_sol);*/
	struct slovo slv;
	struct osm_ries *riesenie, *min_ries = NULL;

	strip_nl_char (line_buf);

	if (	strlen (line_buf) > 0 	&& line_buf[0] != '#' 	&& 
		line_buf[0] != 't' 	&& line_buf[0] != 'n' 	&& 
		line_buf[0] != 's' 	&& line_buf[0] != 'f') {	/* ak je slovo */

		if (create_slovo (line_buf, &slv)) {
			min_ries = NULL;
			if (!search7
			    (osm, &slv, cst, &riesenie, *opt1, *opt2,
			     *forb_lim)) {
				fprintf (stdout, "Nenaslo sa slovo na riadku %d.\n", line);	/* stderr */
				print_slovo (&slv);
				printf ("\n");
			}
			else {
				if (special_sol) {
					if (!special_solution
					    (osm, line_buf, &slv, riesenie,
					     ignore_multiple, *lusti_pocet))
						return 0;
				}
				else {
					if (!normal_solution
					    (osm, line_buf, &slv, riesenie,
					     *lusti_pocet))
						return 0;
				}
			}
			free_slovo (&slv);
			free_osm_ries (riesenie);
		}
		else {
			fprintf (stderr,
				 "Chyba na riadku %d: Pocas vytvarania slova.\n",
				 line);
			fprintf (stderr, "%s\n", line_buf);
		}
	}
	else {
		if (line_buf[0] == 't') {
			if (!select_type
			    (line_buf, prg_buf, line, def_cesta, cst)) {
				free_kriz (osm);
				if (!*def_cesta)
					free_cesty (cst);
				return 0;
			}
		}
		else if (line_buf[0] == 's') {
		}
		else if (line_buf[0] == 'f') {
			select_forb_limit (line_buf, forb_lim);
		}
		else if (line_buf[0] == 'n') {
			if (sscanf (&line_buf[1], "%d", lusti_pocet) < 1) {
				fprintf (stderr,
					 "Chyba na riadku %d: Za 'n' musi nasledovat pocet lusteni daneho slova.\n",
					 line);
				
				free_kriz (osm);
				if (!*def_cesta)
					free_cesty (cst);
				return 0;
			}
			
			/*fprintf(stderr, "Lusti pocet: %d\n", *lusti_pocet); */

			if (*lusti_pocet < 1) {
				fprintf (stderr,
					 "Chyba na riadku %d: Pocet hladani slova musi byt kladny.(Nastavena default hodnota 1)\n",
					 line);
				*lusti_pocet = 1;
			}
		}
	}
	return 1;
}

int
lusti_kriz3_6_syl (char *file_1, char *file_2, int ignore_multiple,
		   int special_sol, int opt1, int opt2)
{
	FILE   *fil;
	char    line_buf[LINE_BUFFER_SIZE];
	char    prg_buf[PRG_BUFFER_SIZE];

	int     line = 0;	/* pocitadlo riadkov v subore slov */
	struct osemsm_syl osm;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	int     lusti_pocet = 1;	/* pocet vyskytov daneho slova (default 1) */
	int     forb_lim = FORB_LIMIT_HALF_GT;

	strcpy (prg_buf, prg_default);

	fil = fopen (file_2, "rt");
	if (fil == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa otvorit subor so slovami.\n");
		return 0;
	}

	if (!create_kriz_syl (file_1, &osm)) {
		fprintf (stderr, "Chyba: Nepodarilo sa vytvorit krizovku.\n");
		fclose (fil);
		return 0;
	}

	while (!feof (fil)) {
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			line++;

			if (!lusti_kriz_word_syl
			    (&osm, line_buf, prg_buf, ignore_multiple,
			     special_sol, line, &cst, &def_cesta,
			     &lusti_pocet, &forb_lim, &opt1, &opt2)) {
				return 0;
			}
		}
	}
	print_kriz3_syl (&osm);
	free_kriz_syl (&osm);
	if (!def_cesta)
		free_cesty (&cst);
	fclose (fil);
	return 1;
}

int
lusti_kriz3_6 (char *file_1, char *file_2, int ignore_multiple,
	       int special_sol, int opt1, int opt2)
{
	FILE   *fil;
	char    line_buf[LINE_BUFFER_SIZE];
	char    prg_buf[PRG_BUFFER_SIZE];

	int     line = 0;	/* pocitadlo riadkov v subore slov */
	struct osemsm osm;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	int     lusti_pocet = 1;	/* pocet vyskytov daneho slova (default 1) */
	int     forb_lim = FORB_LIMIT_HALF_GT;

	strcpy (prg_buf, prg_default);

	fil = fopen (file_2, "rt");
	if (fil == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa otvorit subor so slovami.\n");
		return 0;
	}

	if (!create_kriz (file_1, &osm)) {
		fprintf (stderr, "Chyba: Nepodarilo sa vytvorit krizovku.\n");
		fclose (fil);
		return 0;
	}

#ifdef OSM_DEBUG
	print_masks(&osm);
#endif
	while (!feof (fil)) {
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			line++;

			if (!lusti_kriz_word
			    (&osm, line_buf, prg_buf, ignore_multiple,
			     special_sol, line, &cst, &def_cesta,
			     &lusti_pocet, &forb_lim, &opt1, &opt2)) {
				return 0;
			}
		}
	}
	print_kriz3 (&osm);
	free_kriz (&osm);
	if (!def_cesta)
		free_cesty (&cst);
	fclose (fil);
	return 1;
}

int
lusti_kriz3_6_prompt (char *file_1, int ignore_multiple, int special_sol,
		      int opt1, int opt2)
{
	FILE   *fil;
	char    line_buf[LINE_BUFFER_SIZE];
	char    prg_buf[PRG_BUFFER_SIZE];

	int     line = 0, forb_lim = FORB_LIMIT_NONE;
	struct osemsm osm;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	int     lusti_pocet = 1;

	opt1 = 1;
	opt2 = 1;
	strcpy (prg_buf, prg_default);

	fil = stdin;
	if (fil == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa otvorit subor so slovami.\n");
		return 0;
	}
	if (!create_kriz (file_1, &osm)) {
		fprintf (stderr, "Chyba: Nepodarilo sa vytvorit krizovku.\n");
		fclose (fil);
		return 0;
	}

	while (!feof (fil)) {
		printf ("\nZadajte slovo: ");
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			line++;
			if (!lusti_kriz_word
			    (&osm, line_buf, prg_buf, ignore_multiple,
			     special_sol, line, &cst, &def_cesta,
			     &lusti_pocet, &forb_lim, &opt1, &opt2)) {
				return 0;
			}
		}
	}
	print_kriz3 (&osm);
	free_kriz (&osm);
	if (!def_cesta)
		free_cesty (&cst);
	fclose (fil);
	return 1;
}

int
lusti_kriz3_6_log (char *file_1, char *file_2, char *file_3,
		   int ignore_multiple, int special_sol, int opt1, int opt2)
{
	FILE   *fil, *fou;
	char    line_buf[LINE_BUFFER_SIZE];
	char    prg_buf[PRG_BUFFER_SIZE];

	int     line = 0;
	struct osemsm osm;
	struct cesty cst = default_cesta;
	int     def_cesta = 1;
	int     lusti_pocet = 1;	/* * pocet * vyskytov * daneho * slova * (default * 1) */
	int     forb_lim = FORB_LIMIT_HALF_GT;

	strcpy (prg_buf, prg_default);

	fou = fopen (file_3, "wt");
	if (fou == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa vytvorit log subor.\n");
		return 0;
	}
	fil = fopen (file_2, "rt");
	if (fil == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa otvorit subor so slovami.\n");
		fclose (fou);
		return 0;
	}
	if (!create_kriz (file_1, &osm)) {
		fprintf (stderr, "Chyba: Nepodarilo sa vytvorit krizovku.\n");
		fclose (fil);
		fclose (fou);
		return 0;
	}

	while (!feof (fil)) {
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			line++;

			if (!lusti_kriz_word
			    (&osm, line_buf, prg_buf, ignore_multiple,
			     special_sol, line, &cst, &def_cesta,
			     &lusti_pocet, &forb_lim, &opt1, &opt2)) {
				fclose (fil);
				fclose (fou);
				return 0;

			}

		}
	}
	print_kriz3_log (&osm, fou);
	free_kriz (&osm);
	if (!def_cesta)
		free_cesty (&cst);
	fclose (fil);
	fclose (fou);
	return 1;
}

int
select_type (char *line_buf, char *prg_buf, int line,
	     int *def_cesta, struct cesty *cst)
{

	if (line_buf[0] == 't') {
		if (line_buf[1] != 'T') {
			fprintf (stderr,
				 "Chyba na riadku %d: Chybne oznacenie typu",
				 line);
			return 0;
		}
		else
			switch (line_buf[2]) {
			case 'C':
				if (!sscanf (&line_buf[3], "%s", prg_buf)) {
					fprintf (stderr,
						 "Chyba na riadku %d: Za tTC musi nasledovat program(y).\n",
						 line);
					return 0;
				}
				if (!*def_cesta)
					free_cesty (cst);

#ifdef OSM_DEBUG
				fprintf(stderr, "Trying to create ... '%s'.\n", prg_buf);
#endif
				if (!create_cesty (cst, prg_buf)) {
					fprintf (stderr,
						 "Chyba na riadku %d: Chybne zadanie programu(ov).\n",
						 line);
					return 0;
				}
				*def_cesta = 0;
				break;
			case '0':
				if (!*def_cesta)
					free_cesty (cst);
				*cst = default_cesta;
				*def_cesta = 1;
				break;
			case '1':
				if (!def_cesta)
					free_cesty (cst);
				*cst = vlnka_cesta;
				*def_cesta = 1;
				break;
			case '2':
				if (!def_cesta)
					free_cesty (cst);
				*cst = obdlznik_cesta;
				*def_cesta = 1;
				break;
			default:
				fprintf (stderr,
					 "Chyba na riadku %d: Neznamy typ programu.\n",
					 line);
				return 0;
			}
	}
	return 1;
}

void
strip_nl_char (char *line)
{

	while (line[strlen (line) - 1] == '\n'
	       || line[strlen (line) - 1] == '\r')
		line[strlen (line) - 1] = '\0';
}

/* ! vracia pocet doplnenych otaznikov !*/
int
dopln_quest (struct osemsm *osm, struct osm_ries *ries, struct slovo *slv)
{
	struct num_path pth;
	int     i, new_dir, nx, ny, val, q_poc = 0;
	struct pismeno pis;

	if (ries->cnt_question_mark) {

		if (!create_path_str2_numpath
		    (&pth, ries->dir_num, ries->chr_path)) {
			fprintf (stderr,
				 "Chyba: Pocas vytvarania cesty(dopln_quest).\n");
			return 0;
		}

		nx = ries->pos_x;
		ny = ries->pos_y;

		for (i = 0; i < slv->count; i++) {

			if (!get_dir_num_numpath (&pth, i, &new_dir)) {
				fprintf (stderr,
					 "Chyba: Nepodarilo sa ziskat prislusny smer.\n");
				return 0;
			}

			pis = get_pismeno (osm, nx, ny);

			if (pis.l == QUEST_CHAR) {
				val = (slv->pism[i]).l;
				replace_id (osm, nx, ny, val);
				q_poc++;
			}

			if (!get_next_word_in_dir
			    (osm, nx, ny, new_dir, &nx, &ny)) {
				return 0;
			}

		}
		return q_poc;
	}
	else
		return 0;

}

int
dopln_quest_syl (struct osemsm_syl *osm, struct osm_ries *ries,
		 struct slovo *slv)
{
	return 0;
/* TODO: Implement this */

/*	struct num_path     pth;
   int             i, new_dir, nx, ny, val, q_poc = 0;
   struct pismeno  pis;

   if (ries->cnt_question_mark)
   {

	  if (!create_path_str2_numpath(&pth, ries->dir_num, ries->chr_path))
	  {
		 fprintf(stderr, "Chyba: Pocas vytvarania cesty(dopln_quest).\n");
		 return 0;
	  }

	  nx = ries->pos_x;
	  ny = ries->pos_y;

	  for (i = 0; i < slv->count; i++)
	  {

		 if (!get_dir_num_numpath(&pth, i, &new_dir))
		 {
			fprintf(stderr, "Chyba: Nepodarilo sa ziskat prislusny smer.\n");
			return 0;
		 }

		 pis = get_pismeno(osm, nx, ny);

		 if (pis.l == QUEST_CHAR)
		 {
			val = (slv->pism[i]).l;
			replace_id(osm, nx, ny, val);
			q_poc++;
		 }

		 if (!get_next_word_in_dir(osm, nx, ny, new_dir, &nx, &ny))
		 {
			return 0;
		 }

	  }
	  return q_poc;
   } else
	  return 0;
*/
}
