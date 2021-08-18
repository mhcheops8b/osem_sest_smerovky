#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flosm.h"
#include "flosmsyl.h"
#include "obrfile.h"

#define LINE_BUFFER_SIZE 256

/* 
 * Strips newline characters ('\n' or '\r') from the end of line 
 */

void
strip_nl (char *line)
{

	int     c;

	while ((c = line[strlen (line) - 1]) == '\n' || c == '\r')
		line[strlen (line) - 1] = '\0';
}

/*
 * Returns position of first non whitespace character in line
 * If line does not contain non whitespace charecter returns NULL
 */
char   *
skip_ws (char *line)
{
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	return *line ? line : NULL;
}

/*
 * Skips emty lines and comment lines and read the size of osm_field
 */
#define BAD_SIZE_FMT 	-1
#define BAD_SIZE_ARG 	-2
#define OK_SIZE 	0

int
read_size_osm (FILE * fil, int *line, int *size_x, int *size_y)
{

	char    line_buf[LINE_BUFFER_SIZE], *line_pom;
	int     try = 0, siz_x, siz_y;

	do {
		try++;
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil) == NULL) {
			fprintf (stderr,
				 (try == 1) ? "Chyba: Subor je prazdny.\n" :
				 "Chyba: Neznamy format suboru.\nOcakavaju sa rozmery obrazca osemsmerovky na zaciatku suboru.\n");
			return BAD_SIZE_FMT;
		}

		(*line)++;
		strip_nl (line_buf);
		line_pom = skip_ws (line_buf);
	}
	while (!line_pom || line_pom[0] == '#');

	if (sscanf (line_pom, "%d %d", &siz_x, &siz_y) != 2) {
		fprintf (stderr,
			 "Chyba: Na riadku %d. Ocakavaju sa rozmery krizovky ako prvy udaj po preskoceni prazdnych riadkov a komentarov.\n",
			 *line);
		return BAD_SIZE_FMT;
	}

	if (siz_x < 1 || siz_y < 1) {
		fprintf (stderr,
			 "Chyba: Rozmery krizovky musia byt kladne.\n");
		return BAD_SIZE_ARG;
	}
#ifdef OSM_DEBUG
	printf ("%d,%d\n", siz_x, siz_y);
#endif
	*size_x = siz_x;
	*size_y = siz_y;
	return OK_SIZE;
}

/*
 * Read osm_fld data from file
 */
#define BAD_DATA_CHRS		-1
#define BAD_DATA_LINE_CNT 	-2
#define BAD_DATA		-3
#define BAD_DATA_COUNT 		-4
#define OK_DATA			 0

const char *pismeno_skl[] = {
	"pismen",		/* 0, 5-... */
	"pismeno",		/* 1 */
	"pismena"		/* 2, 3, 4 */
};
const char *ocakavany_skl[] = {
	"ocakavanych",
	"ocakavane",
	"ocakavane"
};

const char *
sklonuj (char *zaklad, int count)
{
	if (!strcmp (zaklad, "pismeno")) {
		if (count == 0 || count >= 5)
			return pismeno_skl[0];
		else if (count == 1)
			return pismeno_skl[1];
		else
			return pismeno_skl[2];
	}
	else if (!strcmp (zaklad, "ocakavany")) {
		if (count == 0 || count >= 5)
			return ocakavany_skl[0];
		else if (count == 1)
			return ocakavany_skl[1];
		else
			return ocakavany_skl[2];

	}
	return "";
}

int
check_line_size (int size_x, char *filename, int cnt, int tot_cnt,
		 int tot_interval, int line, int tot_line_beg,
		 int tot_line_end)
{

	if (tot_cnt != size_x) {
		if (!tot_interval)
			fprintf (stderr,
				 "Chyba na riadku %d v subore %s - obsahuje %d %s, co je %s ako %s %d %s.\n",
				 line,
				 filename,
				 cnt,
				 sklonuj ("pismeno", cnt),
				 (tot_cnt < size_x) ? "menej" : "viacej",
				 sklonuj ("ocakavany", size_x),
				 size_x, sklonuj ("pismeno", size_x));
		else
			fprintf (stderr,
				 "Chyba na riadkoch %d - %d v subore %s - obsahuje %d %s, co je %s ako %s %d %s.\n",
				 tot_line_beg,
				 tot_line_end,
				 filename,
				 tot_cnt,
				 sklonuj ("pismeno", tot_cnt),
				 (tot_cnt < size_x) ? "menej" : "viacej",
				 sklonuj ("ocakavany", size_x),
				 size_x, sklonuj ("pismeno", size_x));

		return -1;
	}

	return 0;
}

int
read_data_osm (FILE * fil, int *line, char *filename, struct osemsm *osm)
{

	char    line_buf[LINE_BUFFER_SIZE], *line_pom;

	int     kriz_line = 0,	/* aktualne spracovavany riadok krizovky */
	        tot_line = 1, cnt = 0, tot_cnt,	/* pocet pismen na aktualnom riadku v subore, na riadku v krizovke */
	        tot_line_beg = 0, tot_line_end = 0, tot_interval,	/* zaciatok a koniec dlheho riadku */
	        ix, iy, ii,	/* iy-ty riadok a ix-ty stlpec v krizovke, pomocna premenna cyklu */
	        count;		/* pocet ocakavanych znakov v krizovke */
	struct slovo pom;

/*	struct pismeno  pis, chk_pis;*/

	count = get_size_x_osm_size (&osm->osm_size) *
		get_size_y_osm_size (&osm->osm_size);

	tot_cnt = 0;
	ix = 0;
	iy = 0, tot_interval = 0;
	while (!feof (fil)
	       && kriz_line < get_size_y_osm_size (&osm->osm_size)) {

		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			(*line)++;
			strip_nl (line_buf);
			line_pom = skip_ws (line_buf);

			if (strlen (line_buf) != 0 && line_pom[0] != '#') {
				if (line_pom[strlen (line_pom) - 1] == '\\') {
					if (tot_line) {
						tot_line_beg = *line;
						tot_interval = 1;
					}
					tot_line = 0;
				}
				else {
					if (!tot_line)
						tot_line_end = *line;

					tot_line = 1;
				}

				if (tot_line)
					kriz_line++;

				if (!tot_line)
					line_pom[strlen (line_pom) - 1] =
						'\0';

				if (!check_slovo (line_pom)) {
					fprintf (stderr,
						 "Chyba na riadku %d v subore %s - chybne znaky.\n",
						 *line, filename);
					return BAD_DATA_CHRS;
				}

				cnt = count_slovo (line_pom);
				tot_cnt += cnt;
				/*
				   printf("Line: %d, Znakov %d\n", *line, cnt);
				 */

				if (cnt == 0) {	/* Ignore emty lines */
					if (tot_line) {
						kriz_line--;
						tot_line = 0;
					}
				}
				else {
					if (create_slovo (line_buf, &pom)) {
						for (ii = 0; ii < cnt; ii++) {
							set_pismeno (osm,
								     ix + ii,
								     iy,
								     pom.
								     pism
								     [ii]);
						}
						ix += cnt;
						count -= cnt;

						if (tot_line)
							iy++;

						free_slovo (&pom);
					}
					else {
						fprintf (stderr,
							 "Chyba: Pocas vytvarania slova.\n");
						return BAD_DATA;
					}

				}

				if (tot_line) {

					if (check_line_size
					    (get_size_x_osm_size
					     (&osm->osm_size), filename, cnt,
					     tot_cnt, tot_interval, *line,
					     tot_line_beg, tot_line_end) < 0)
						return BAD_DATA_LINE_CNT;
					else {
						ix = 0;
						tot_cnt = 0;
						tot_line = 1;
						tot_interval = 0;
					}
				}
			}
			else {
				if (!tot_line) {
					tot_line_end = *line;

					if (check_line_size
					    (get_size_x_osm_size
					     (&osm->osm_size), filename, cnt,
					     tot_cnt, tot_interval, *line,
					     tot_line_beg, tot_line_end) < 0)
						return BAD_DATA_LINE_CNT;

					tot_line = 1;
				}
			}
		}
	}			/* end while */

	if (count != 0) {
		fprintf (stderr, "Chyba: Krizovka obsahuje ");
		if (count > 0)
			fprintf (stderr, "menej pismen ");
		else
			fprintf (stderr, "viacej pismen ");
		fprintf (stderr, "ako treba.\n");
		return BAD_DATA_COUNT;
	}

	return OK_DATA;
}

int
read_syllabe_data_osm (FILE * fil, int *line, char *filename,
		       struct osemsm_syl *osm, char syllabe_delimiter)
{

	char    line_buf[LINE_BUFFER_SIZE], *line_pom, *pom_beg, *pom_end;

	int     kriz_line = 0,	/* aktualne spracovavany riadok krizovky */
	        tot_line = 1, cnt = 0, tot_cnt,	/* pocet pismen na aktualnom riadku v subore, na riadku v krizovke */
	        tot_line_beg = 0, tot_line_end = 0, tot_interval,	/* zaciatok a koniec dlheho riadku */
	        ix, iy,		/*ii, *//* iy-ty riadok a ix-ty stlpec v krizovke, pomocna premenna cyklu */
	        count;		/* pocet ocakavanych znakov v krizovke */

/*   	struct slovo    pom;*/

	count = get_size_x_osm_size (&osm->osm_size) *
		get_size_y_osm_size (&osm->osm_size);

	tot_cnt = 0;
	ix = 0;
	iy = 0, tot_interval = 0;
	while (!feof (fil)
	       && kriz_line < get_size_y_osm_size (&osm->osm_size)) {

		if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
			(*line)++;
			strip_nl (line_buf);
			line_pom = skip_ws (line_buf);
#ifdef OSM_DEBUG
			printf ("%s", line_pom);
#endif
			if (strlen (line_pom) != 0 && line_pom[0] != '#') {
				if (line_pom[strlen (line_pom) - 1] == '\\') {
					if (tot_line) {
						tot_line_beg = *line;
						tot_interval = 1;
					}
					tot_line = 0;
				}
				else {
					if (!tot_line)
						tot_line_end = *line;

					tot_line = 1;
				}

				if (tot_line)
					kriz_line++;

				if (!tot_line)
					line_pom[strlen (line_pom) - 1] =
						'\0';

				if (!check_slovo (line_pom)) {
					fprintf (stderr,
						 "Chyba na riadku %d v subore %s - chybne znaky.\n",
						 *line, filename);
					return BAD_DATA_CHRS;
				}

/* */
				pom_end = line_pom;
				pom_beg = line_pom;

				while (*pom_beg) {
					while (*pom_end
					       && *pom_end !=
					       syllabe_delimiter)
						pom_end++;
					/*osm->pole[get_pos_syl(osm, ix, iy)].slv.count = count_slovo_range(pom_beg, pom_end-1); */
					create_slovo_range (pom_beg,
							    pom_end - 1,
							    &osm->
							    pole[get_pos_syl
								 (osm, ix,
								  iy)].slv);
					osm->pole[get_pos_syl (osm, ix, iy)].
						id = 0;

					/* add syllabe pom_beg -  pom_end-1 to obr */


					if (*pom_end) {
						pom_beg = pom_end + 1;
						pom_end++;
					}
					else
						pom_beg = pom_end;
					ix++;
					/*                                      if (opt_skip_ws) {
					   while (*pom_beg && (*pom_beg ==' ' || *pom_beg == '\t')) pom_beg++;
					   } */
				}

/* */

				/*cnt = count_slovo(line_pom); */
				cnt = ix;
				tot_cnt += cnt;
				/*
				   printf("Line: %d, Znakov %d\n", *line, cnt);
				 */

				if (cnt == 0) {	/* Ignore emty lines */
					if (tot_line) {
						kriz_line--;
						tot_line = 0;
					}
				}
				else {

/*					if (create_slovo(line_buf, &pom)) {
						for (ii = 0; ii < cnt; ii++) {
				  			set_pismeno(osm, ix+ii, iy, pom.pism[ii]);
			   			}
						ix+=cnt;*/
					count -= cnt;

					if (tot_line)
						iy++;

/*			   			free_slovo(&pom);
					} 
					else {
			   			fprintf(stderr, "Chyba: Pocas vytvarania slova.\n");
						return BAD_DATA;
					}*/

				}

				if (tot_line) {

					if (check_line_size
					    (get_size_x_osm_size
					     (&osm->osm_size), filename, cnt,
					     tot_cnt, tot_interval, *line,
					     tot_line_beg, tot_line_end) < 0)
						return BAD_DATA_LINE_CNT;
					else {
						ix = 0;
						tot_cnt = 0;
						tot_line = 1;
						tot_interval = 0;
					}
				}
			}
			else {
				if (!tot_line) {
					tot_line_end = *line;

					if (check_line_size
					    (get_size_x_osm_size
					     (&osm->osm_size), filename, cnt,
					     tot_cnt, tot_interval, *line,
					     tot_line_beg, tot_line_end) < 0)
						return BAD_DATA_LINE_CNT;

					tot_line = 1;
				}
			}
		}
	}			/* end while */

	if (count != 0) {
		fprintf (stderr, "Chyba: Krizovka obsahuje ");
		if (count > 0)
			fprintf (stderr, "menej pismen ");
		else
			fprintf (stderr, "viacej pismen ");
		fprintf (stderr, "ako treba.\n");
		return BAD_DATA_COUNT;
	}

	return OK_DATA;
}

#define MASK_MODE 	1
#define ID_MODE  	0
#define YES 	1
#define NO	0

/*
 * Reads obr file <filename> and creates osemsm structure <osm>
 */
int
create_kriz (char *filename, struct osemsm *osm)
{

	FILE   *fil;
	int     siz_x, siz_y, count, pom_count, i;

	int     line = 0, id = 0;

	int     mode = ID_MODE, read_again = YES;
	char    line_buf[LINE_BUFFER_SIZE];

	struct pismeno pis, chk_pis;

	osm->qpart = NULL;
	osm->id = NULL;

	fil = fopen (filename, "rt");
	if (fil == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa otvorit subor '%s'.\n",
			 filename);
		return 0;
	}

	if (read_size_osm (fil, &line, &siz_x, &siz_y) != OK_SIZE) {
		fclose (fil);
		return 0;
	}

	count = siz_x * siz_y;

	osm->mask = (unsigned char *) malloc (sizeof (unsigned char) * count);
	if (osm->mask == NULL) {
		fprintf (stderr, "Nepodarilo sa alokovat pamat pre masky.\n");
		fclose (fil);
		return 0;
	}

	for (i = 0; i < count; i++)
		osm->mask[i] = 255;

	pom_count = count / 16 + ((count % 16) ? 1 : 0);

	osm->pole =
		(struct pismeno *) malloc (count * sizeof (struct pismeno));

	if (osm->pole == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre pole.\n");
		free ((void *) osm->mask);
		fclose (fil);
		return 0;
	}

	if (create_bitmap2d_ulm (&osm->bitmap, siz_x, siz_y) < 0) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre bitmap.\n");
		free ((void *) osm->mask);
		free ((void *) osm->pole);
		fclose (fil);
		return 0;
	}
	
#if 0
	printf("%d\n", osm->bitmap.size_x);	
	printf("%d\n", osm->bitmap.size_y);	
	printf("%d\n", osm->bitmap.bmp1d.bit_size_of_data_elem);
	printf("%d\n", osm->bitmap.bmp1d.size_of_bitmap);
#endif

/*	if (create_bitmap(osm, pom_count + 1))
		init_bitmap(osm, pom_count + 1);
  	 else {
	 	fprintf(stderr, "Chyba: Nepodarilo sa alokovat pamat pre bitmap.\n");
	  	free((void *) osm->pole);
	  	fclose(fil);
	  	return 0;
   	}
*/

	set_size_osm_size (&osm->osm_size, siz_x, siz_y);
	osm->spec = NULL;

	if (read_data_osm (fil, &line, filename, osm) != OK_DATA) {
		free_kriz (osm);
		fclose (fil);
		return 0;
	}

	while (!feof (fil)) {

		if (!read_again
		    || fgets (line_buf, LINE_BUFFER_SIZE, fil) != NULL) {
			int     x, y, r, s, i, j;

			read_again = YES;
			line++;
			if (line_buf[strlen (line_buf) - 1] == '\n')
				line_buf[strlen (line_buf) - 1] = '\0';
			if (line_buf[strlen (line_buf) - 1] == '\r')
				line_buf[strlen (line_buf) - 1] = '\0';
			if (strlen (line_buf) && line_buf[0] != '#'
			    && line_buf[0] != 's' && line_buf[0] != 'Q'
			    && line_buf[0] != 'M') {
				if (mode == ID_MODE) {
					if (sscanf
					    (line_buf, "%d %d %d %d", &x, &y,
					     &s, &r) < 4) {
						fprintf (stderr,
							 "Chyba na riadku %d - ocakavaju sa x y dx dy\n",
							 line);
						free_kriz (osm);
						return 0;
					}

					id++;
					chk_pis = get_pismeno (osm, x, y);
					for (i = 0; i < s; i++)
						for (j = 0; j < r; j++) {
							pis = get_pismeno
								(osm, x + i,
								 y + j);
							if (chk_pis.l !=
							    pis.l) {
								fprintf (stderr, "Chyba: Skupina (%d %d %d %d) na riadku %d neobsahuje rovnake pismena.\n", x, y, r, s, line);
								free_kriz
									(osm);
								return 0;
							}
							pis.id = id;
							set_pismeno (osm,
								     x + i,
								     y + j,
								     pis);
						}
				}
				else {
					if (!read_m_section
					    (osm, fil, line_buf, &line)) {
						free_kriz (osm);
						return 0;
					}
					read_again = NO;
				}
			}
			else {
				if (line_buf[0] == 's') {
					int     i = 0, pocet, x, y;
					char    buf[2] = { 0, 0 };

					if (sscanf
					    (&line_buf[1], "%d",
					     &pocet) < 1) {
						fprintf (stderr,
							 "Chyba: Za s musi nasledovat pocet spec. skupin");
						free_kriz (osm);
						fclose (fil);
						return 0;
					}
					if (pocet == 0) {
						fprintf (stderr,
							 "Chyba: Pocet spec. skupin musi byt nenulovy");
						free_kriz (osm);
						fclose (fil);
						return 0;
					}
					while (!feof (fil) && i < pocet) {
						if (fgets
						    (line_buf,
						     LINE_BUFFER_SIZE, fil)) {
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\n')
								line_buf[strlen (line_buf) - 1] = '\0';
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\r')
								line_buf[strlen (line_buf) - 1] = '\0';

							if (strlen (line_buf)
							    && line_buf[0] !=
							    '#') {
								if (sscanf
								    (line_buf,
								     "%d %d %1s",
								     &x, &y,
								     buf)
								    < 3) {
									fprintf (stderr, "Chyba v udajoch `x y char`.\n");
									free_kriz
										(osm);
									fclose (fil);
									return 0;
								}
								else if (add_specchar_list (&osm->spec, x, y, buf[0]) < 0) {
									free_kriz
										(osm);
									fclose (fil);
									return 0;
								}
								else
									i++;
							}

						}
					}
				}
				else if (line_buf[0] == 'M') {
					mode = MASK_MODE;
					read_again = YES;
				}
				else if (line_buf[0] == 'Q') {
					int     x_q, y_q, n_p, n_s, i_q =
						0, i_s = 0;

					if (sscanf (&line_buf[1], "%d", &n_p)
					    < 1) {
						fprintf (stderr,
							 "Chyba na riadku %d: Za Q musi nasledovat pocet q-casti.\n",
							 line);
						free_kriz (osm);
						fclose (fil);
						return 0;
					}
					if (n_p < 1) {
						fprintf (stderr,
							 "Chyba na riadku %d: Pocet q-casti musi byt kladny.\n",
							 line);
						free_kriz (osm);
						fclose (fil);
						return 0;
					}

/*	  osm->qpart=(struct q_part *)malloc(sizeof(struct q_part));

	  if (osm->qpart==NULL) {
	    fprintf(stderr,"Chyba: Nepodarilo sa alokovat pamat(q_part,create_kriz).\n");
	    free_kriz(osm);
	    fclose(fil);
	    return 0;
	  }

	  osm->qpart->elems=(struct q_sect **)malloc(sizeof(struct q_sect *)*np);

	  if (osm->qpart->elems==NULL) {
	    fprintf(stderr,"Chyba: Nepodarilo sa alokovat pamat(q_part.elems,create_kriz).\n");
	    free((void *)osm->qpart);
	    osm->qpart=NULL;
	    free_kriz(osm);
	    fclose(fil);
	    return 0;
	  }
  */
					if (!create_q_part (&osm->qpart, n_p)) {
						free_kriz (osm);
						fclose (fil);
						return 0;
					}

					while (!feof (fil) && i_q < n_p) {
						if (fgets
						    (line_buf,
						     LINE_BUFFER_SIZE, fil)) {
							line++;
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\n')
								line_buf[strlen (line_buf) - 1] = '\0';
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\r')
								line_buf[strlen (line_buf) - 1] = '\0';

							if (strlen (line_buf)
							    && line_buf[0] !=
							    '#') {
								if (line_buf
								    [0] !=
								    'q') {
									fprintf (stderr, "Chyba na riadku %d: Ocakava sa specifikacia q-sekcie.\n", line);
									free_kriz
										(osm);
									fclose (fil);
									return 0;
								}
								else {
									if (sscanf (&line_buf[1], "%d", &n_s) < 1) {
										fprintf (stderr, "Chyba na riadku %d: Za q sa ocakava pocet poloziek v q-sekcii.\n", line);
										free_kriz
											(osm);
										fclose (fil);
										return 0;
									}
									if (n_s < 1) {
										fprintf (stderr, "Chyba na riadku %d: Ocakava sa kladny pocet poloziek v q-sekcii.\n", line);
										free_kriz
											(osm);
										fclose (fil);
										return 0;
									}
									if (!create_q_sect (&(osm->qpart->elems[i_q]), n_s)) {

/*		    for (i_s=0;i_s<i_q;i_s++)
		      free_q_sect(osm->qpart->elems[i_s]);*/
										free_kriz
											(osm);
										fclose (fil);
										return 0;
									}
									else {
										i_s = 0;
										while (!feof (fil) && i_s < n_s) {
											if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
												line++;
												if (line_buf[strlen (line_buf) - 1] == '\n')
													line_buf[strlen (line_buf) - 1] = '\0';
												if (line_buf[strlen (line_buf) - 1] == '\r')
													line_buf[strlen (line_buf) - 1] = '\0';

												if (strlen (line_buf)
												    &&
												    line_buf
												    [0]
												    !=
												    '#')
												{
													if (sscanf (line_buf, "%d %d", &x_q, &y_q) < 2) {
														fprintf (stderr, "Chyba: Na riadku %d: Ocakavaju  sa `x y`.\n", line);

/*			    for (i_s=0;i_s<i_q;i_s++)
			      free_q_sect(osm->qpart->elems[i_s]);*/
														free_kriz
															(osm);
														fclose (fil);
														return 0;
													}
													else {
														store_val_q_sect
															(osm->
															 qpart->
															 elems
															 [i_q],
															 i_s,
															 x_q,
															 y_q);
														i_s++;
													}
												}
											}
										}
										if (i_s < n_s) {
											fprintf (stderr, "Chyba: Sekcia neobsahuje zadany pocet prvkov.\n");
											free_kriz
												(osm);
											fclose (fil);
											return 0;
										}
										else
											i_q++;
									}
								}
							}
						}
					}
					if (i_q < n_p) {
						fprintf (stderr,
							 "Chyba: Subor neobsahuje zadany pocet casti.\n");

/*	    for (i=0;i<i_q;i++)
	      free_q_sect(osm->qpart->elems[i]);*/
						free_kriz (osm);
						fclose (fil);
						return 0;
					}
				}
			}
		}
	}

	osm->ids = id;

	if (id) {
		id++;
		pom_count = id / 16;
		if (id % 16)
			pom_count++;
		osm->id =
			(unsigned int *) malloc (sizeof (unsigned int) *
						 pom_count);

		if (osm->id == NULL) {
			fprintf (stderr, "Chyba: Nedostatok pamate (id).\n");
			free_kriz (osm);
		}
		else
			for (i = 0; i < pom_count; i++) {
				osm->id[i] = 0;
			}
	}

	fclose (fil);
	return 1;
}

int
read_s_section (struct osemsm *osm, FILE * fil, char *buffer)
{
	int     i = 0, pocet, x, y;
	char    buf[2] = { 0, 0 };

	if (sscanf (&buffer[1], "%d", &pocet) < 1) {
		fprintf (stderr,
			 "Chyba: Za S musi nasledovat pocet spec. skupin.\n");
		return 0;
	}
	if (pocet == 0) {
		fprintf (stderr,
			 "Chyba: Pocet spec. skupin musi byt nenulovy.\n");
		return 0;
	}

	while (!feof (fil) && i < pocet) {
		if (fgets (buffer, LINE_BUFFER_SIZE, fil)) {
			if (buffer[strlen (buffer) - 1] == '\n')
				buffer[strlen (buffer) - 1] = '\0';
			if (strlen (buffer) && buffer[0] != '#') {
				if (sscanf (buffer, "%d %d %1s", &x, &y, buf)
				    < 3) {
					fprintf (stderr,
						 "Chyba v udajoch: Ocakava sa `x y char`.\n");
					return 0;
				}
				else if (!add_specchar_list
					 (&osm->spec, x, y, buf[0]))
					return 0;
				else
					i++;
			}
		}
	}
	if (i < pocet) {
		fprintf (stderr,
			 "Chyba: Subor neobsahuje zadany pocet spec. znakov.\n");
		return 0;
	}
	return 1;
}

int
read_m_section (struct osemsm *osm, FILE * fil, char *line_buf, int *line)
{
	int     x, y, msk;

	do {
		strip_nl (line_buf);

		if (strlen (line_buf)
		    && line_buf[0] != '#'
		    && line_buf[0] != 's' && line_buf[0] != 'q'
		    && line_buf[0] != 'Q') {
			if (sscanf (line_buf, "%d %d %d", &x, &y, &msk) < 3) {
				fprintf (stderr,
					 "Chyba na riadku %d - ocakavaju sa x y mask.\n",
					 *line);
				return 0;
			}

/*			osm->mask[x +
				  y * get_size_x_osm_size (&osm->osm_size)] =
				msk;
*/
#ifdef OSM_DEBUG
			printf("Storing mask: %d,%d - %d(%d)\n", x,y,msk,get_pos(osm,x,y));
#endif
			osm->mask[get_pos(osm,x,y)]=msk;

		}
		else if (line_buf[0] == 's' || line_buf[0] == 'q'
			 || line_buf[0] == 'Q')
			return 1;

		if (fgets (line_buf, LINE_BUFFER_SIZE, fil) != NULL)
			(*line)++;
	}
	while (!feof (fil));
	return 1;
}

int
read_i_section (struct osemsm *osm, FILE * fil, char *line_buf, int *line)
{

	int     x, y, r, s, i, j, id = 0;
	struct pismeno pis;

	while (!feof (fil)) {
		if (fgets (line_buf, LINE_BUFFER_SIZE, fil) != NULL) {

			(*line)++;
			if (line_buf[strlen (line_buf) - 1] == '\n')
				line_buf[strlen (line_buf) - 1] = '\0';
			if (line_buf[strlen (line_buf) - 1] == '\r')
				line_buf[strlen (line_buf) - 1] = '\0';

			if (strlen (line_buf) && line_buf[0] != '#'
			    && line_buf[0] != 's' && line_buf[0] != 'q') {
				if (sscanf
				    (line_buf, "%d %d %d %d", &x, &y, &s,
				     &r) < 4) {
					fprintf (stderr,
						 "Chyba na riadku %d - ocakavaju sa x y dx dy",
						 *line);
					return 0;
				}
				id++;
				for (i = 0; i < s; i++)
					for (j = 0; j < r; j++) {
						pis = get_pismeno (osm, x + i,
								   y + j);
						pis.id = id;
						set_pismeno (osm, x + i,
							     y + j, pis);
					}
			}
			else if (line_buf[0] == 's' || line_buf[0] == 'q'
				 || line_buf[0] == 'M')
				return 1;
		}
	}
	return 1;
}

int
test_osm_obr (char *obr_file)
{

	struct osemsm osm;

	if (!create_kriz (obr_file, &osm))
		return 0;
	else {
		/*      print_masks(&osm); */
		free_kriz (&osm);
		return 1;
	}
}

/*
 * Reads obr file <filename> and creates osemsm_syl structure <osm>
 */
int
create_kriz_syl (char *filename, OSEMSM_SYL * osm)
{

	FILE   *fil;
	int     siz_x, siz_y, count, pom_count, i;

	int     line = 0, id = 0;

	int     mode = ID_MODE, read_again = YES;
	char    line_buf[LINE_BUFFER_SIZE];

	/*struct pismeno pis, chk_pis; */
	osm->qpart = NULL;
	osm->id = NULL;

	fil = fopen (filename, "rt");
	if (fil == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa otvorit subor '%s'.\n",
			 filename);
		return 0;
	}

	if (read_size_osm (fil, &line, &siz_x, &siz_y) != OK_SIZE) {
		fclose (fil);
		return 0;
	}

	count = siz_x * siz_y;

	osm->mask = (unsigned char *) malloc (sizeof (unsigned char) * count);
	if (osm->mask == NULL) {
		fprintf (stderr, "Nepodarilo sa alokovat pamat pre masky.\n");
		fclose (fil);
		return 0;
	}

	for (i = 0; i < count; i++)
		osm->mask[i] = 255;

	pom_count = count / 16 + ((count % 16) ? 1 : 0);

	osm->pole = (Syllabe *) malloc (count * sizeof (Syllabe));

	if (osm->pole == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre pole.\n");
		free ((void *) osm->mask);
		fclose (fil);
		return 0;
	}

	if (create_bitmap2d_ulm (&osm->bitmap, siz_x, siz_y) < 0) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre bitmap.\n");
		free ((void *) osm->mask);
		free ((void *) osm->pole);
		fclose (fil);
		return 0;
	}

/*	if (create_bitmap(osm, pom_count + 1))
		init_bitmap(osm, pom_count + 1);
  	 else {
	 	fprintf(stderr, "Chyba: Nepodarilo sa alokovat pamat pre bitmap.\n");
	  	free((void *) osm->pole);
	  	fclose(fil);
	  	return 0;
   	}
*/

	set_size_osm_size (&osm->osm_size, siz_x, siz_y);
	osm->spec = NULL;

	if (read_syllabe_data_osm (fil, &line, filename, osm, ' ') != OK_DATA) {
		free_kriz_syl (osm);
		fclose (fil);
		return 0;
	}

	while (!feof (fil)) {

		if (!read_again
		    || fgets (line_buf, LINE_BUFFER_SIZE, fil) != NULL) {
			int     x, y, r, s /*, i, j */ ;

			read_again = YES;
			line++;
			if (line_buf[strlen (line_buf) - 1] == '\n')
				line_buf[strlen (line_buf) - 1] = '\0';
			if (line_buf[strlen (line_buf) - 1] == '\r')
				line_buf[strlen (line_buf) - 1] = '\0';
			if (strlen (line_buf) && line_buf[0] != '#'
			    && line_buf[0] != 's' && line_buf[0] != 'Q'
			    && line_buf[0] != 'M') {
				if (mode == ID_MODE) {
					if (sscanf
					    (line_buf, "%d %d %d %d", &x, &y,
					     &s, &r) < 4) {
						fprintf (stderr,
							 "Chyba na riadku %d - ocakavaju sa x y dx dy\n",
							 line);
						free_kriz_syl (osm);
						return 0;
					}

					id++;
					{
						int     i, j;

						for (i = 0; i < s; i++)
							for (j = 0; j < r;
							     j++) {
								set_syllabe_id
									(osm,
									 x +
									 i,
									 y +
									 j,
									 id);
							}
					}

/* 
				chk_pis = get_pismeno(osm, x, y);
				for (i = 0; i < s; i++)
			   	for (j = 0; j < r; j++) {
				  	pis = get_pismeno(osm, x + i, y + j);
				  	if (chk_pis.l != pis.l) {
					 	fprintf(stderr, "Chyba: Skupina (%d %d %d %d) na riadku %d neobsahuje rovnake pismena.\n", x, y, r, s, line);
					 	free_kriz_syl(osm);
					 	return 0;
				  	}
				 	pis.id = id;
				  	set_pismeno(osm, x + i, y + j, pis);
				}
 */
				}
				else {
					if (!read_m_section
					    ((OSEMSM *) osm, fil, line_buf,
					     &line)) {
						free_kriz_syl (osm);
						return 0;
					}
					read_again = NO;
				}
			}
			else {
				if (line_buf[0] == 's') {
					int     i = 0, pocet, x, y;
					char    buf[2] = { 0, 0 };

					if (sscanf
					    (&line_buf[1], "%d",
					     &pocet) < 1) {
						fprintf (stderr,
							 "Chyba: Za s musi nasledovat pocet spec. skupin");
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}
					if (pocet == 0) {
						fprintf (stderr,
							 "Chyba: Pocet spec. skupin musi byt nenulovy");
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}
					while (!feof (fil) && i < pocet) {
						if (fgets
						    (line_buf,
						     LINE_BUFFER_SIZE, fil)) {
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\n')
								line_buf[strlen (line_buf) - 1] = '\0';
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\r')
								line_buf[strlen (line_buf) - 1] = '\0';

							if (strlen (line_buf)
							    && line_buf[0] !=
							    '#') {
								if (sscanf
								    (line_buf,
								     "%d %d %1s",
								     &x, &y,
								     buf)
								    < 3) {
									fprintf (stderr, "Chyba v udajoch `x y char`.\n");
									free_kriz_syl
										(osm);
									fclose (fil);
									return 0;
								}
								else if (add_specchar_list (&osm->spec, x, y, buf[0]) < 0) {
									free_kriz_syl
										(osm);
									fclose (fil);
									return 0;
								}
								else
									i++;
							}

						}
					}
				}
				else if (line_buf[0] == 'M') {
					mode = MASK_MODE;
					read_again = YES;
				}
				else if (line_buf[0] == 'Q') {
					int     x_q, y_q, n_p, n_s, i_q =
						0, i_s = 0;

					if (sscanf (&line_buf[1], "%d", &n_p)
					    < 1) {
						fprintf (stderr,
							 "Chyba na riadku %d: Za Q musi nasledovat pocet q-casti.\n",
							 line);
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}
					if (n_p < 1) {
						fprintf (stderr,
							 "Chyba na riadku %d: Pocet q-casti musi byt kladny.\n",
							 line);
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}

/*	  osm->qpart=(struct q_part *)malloc(sizeof(struct q_part));

	  if (osm->qpart==NULL) {
	    fprintf(stderr,"Chyba: Nepodarilo sa alokovat pamat(q_part,create_kriz).\n");
	    free_kriz_syl(osm);
	    fclose(fil);
	    return 0;
	  }

	  osm->qpart->elems=(struct q_sect **)malloc(sizeof(struct q_sect *)*np);

	  if (osm->qpart->elems==NULL) {
	    fprintf(stderr,"Chyba: Nepodarilo sa alokovat pamat(q_part.elems,create_kriz).\n");
	    free((void *)osm->qpart);
	    osm->qpart=NULL;
	    free_kriz_syl(osm);
	    fclose(fil);
	    return 0;
	  }
  */
					if (!create_q_part (&osm->qpart, n_p)) {
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}

					while (!feof (fil) && i_q < n_p) {
						if (fgets
						    (line_buf,
						     LINE_BUFFER_SIZE, fil)) {
							line++;
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\n')
								line_buf[strlen (line_buf) - 1] = '\0';
							if (line_buf
							    [strlen (line_buf)
							     - 1] == '\r')
								line_buf[strlen (line_buf) - 1] = '\0';

							if (strlen (line_buf)
							    && line_buf[0] !=
							    '#') {
								if (line_buf
								    [0] !=
								    'q') {
									fprintf (stderr, "Chyba na riadku %d: Ocakava sa specifikacia q-sekcie.\n", line);
									free_kriz_syl
										(osm);
									fclose (fil);
									return 0;
								}
								else {
									if (sscanf (&line_buf[1], "%d", &n_s) < 1) {
										fprintf (stderr, "Chyba na riadku %d: Za q sa ocakava pocet poloziek v q-sekcii.\n", line);
										free_kriz_syl
											(osm);
										fclose (fil);
										return 0;
									}
									if (n_s < 1) {
										fprintf (stderr, "Chyba na riadku %d: Ocakava sa kladny pocet poloziek v q-sekcii.\n", line);
										free_kriz_syl
											(osm);
										fclose (fil);
										return 0;
									}
									if (!create_q_sect (&(osm->qpart->elems[i_q]), n_s)) {

/*		    for (i_s=0;i_s<i_q;i_s++)
		      free_q_sect(osm->qpart->elems[i_s]);*/
										free_kriz_syl
											(osm);
										fclose (fil);
										return 0;
									}
									else {
										i_s = 0;
										while (!feof (fil) && i_s < n_s) {
											if (fgets (line_buf, LINE_BUFFER_SIZE, fil)) {
												line++;
												if (line_buf[strlen (line_buf) - 1] == '\n')
													line_buf[strlen (line_buf) - 1] = '\0';
												if (line_buf[strlen (line_buf) - 1] == '\r')
													line_buf[strlen (line_buf) - 1] = '\0';

												if (strlen (line_buf)
												    &&
												    line_buf
												    [0]
												    !=
												    '#')
												{
													if (sscanf (line_buf, "%d %d", &x_q, &y_q) < 2) {
														fprintf (stderr, "Chyba: Na riadku %d: Ocakavaju  sa `x y`.\n", line);

/*			    for (i_s=0;i_s<i_q;i_s++)
			      free_q_sect(osm->qpart->elems[i_s]);*/
														free_kriz_syl
															(osm);
														fclose (fil);
														return 0;
													}
													else {
														store_val_q_sect
															(osm->
															 qpart->
															 elems
															 [i_q],
															 i_s,
															 x_q,
															 y_q);
														i_s++;
													}
												}
											}
										}
										if (i_s < n_s) {
											fprintf (stderr, "Chyba: Sekcia neobsahuje zadany pocet prvkov.\n");
											free_kriz_syl
												(osm);
											fclose (fil);
											return 0;
										}
										else
											i_q++;
									}
								}
							}
						}
					}
					if (i_q < n_p) {
						fprintf (stderr,
							 "Chyba: Subor neobsahuje zadany pocet casti.\n");

/*	    for (i=0;i<i_q;i++)
	      free_q_sect(osm->qpart->elems[i]);*/
						free_kriz_syl (osm);
						fclose (fil);
						return 0;
					}
				}
			}
		}
	}

	osm->ids = id;

	if (id) {
		id++;
		pom_count = id / 16;
		if (id % 16)
			pom_count++;
		osm->id =
			(unsigned int *) malloc (sizeof (unsigned int) *
						 pom_count);

		if (osm->id == NULL) {
			fprintf (stderr, "Chyba: Nedostatok pamate (id).\n");
			free_kriz_syl (osm);
		}
		else
			for (i = 0; i < pom_count; i++) {
				osm->id[i] = 0;
			}
	}

	fclose (fil);
#ifdef OSM_DEBUG
	printf ("KRZDUMP--->\n");
	dump_kriz_syl (osm);
	printf ("<----------\n");
#endif
	return 1;
}
