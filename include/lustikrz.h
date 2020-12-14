#ifndef _OSM_LUSTIKRZ_H_
#define _OSM_LUSTIKRZ_H_

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

extern const char *prg_default;

/* Prototypes */
int     special_solution (OSEMSM * osm, char *line_buf, struct slovo *slv,
			  struct osm_ries *riesenie, int ignore_multiple,
			  int lusti_pocet);

int     special_solution_syl (OSEMSM_SYL * osm, char *line_buf,
			      struct slovo *slv, struct osm_ries *riesenie,
			      int ignore_multiple, int lusti_pocet);

int     normal_solution (OSEMSM * osm, char *line_buf, struct slovo *slv,
			 struct osm_ries *riesenie, int lusti_pocet);

int     normal_solution_syl (OSEMSM_SYL * osm, char *line_buf,
			     struct slovo *slv, struct osm_ries *riesenie,
			     int lusti_pocet);

void    select_forb_limit (char *pom, int *forb_limit);

int     lusti_kriz_word_syl (OSEMSM_SYL * osm, char *line_buf, char *prg_buf,
			     int ignore_multiple, int special_sol, int line,
			     struct cesty *cst, int *def_cesta,
			     int *lusti_pocet, int *forb_lim, int *opt1,
			     int *opt2);

int     lusti_kriz_word (OSEMSM * osm, char *line_buf, char *prg_buf,
			 int ignore_multiple, int special_sol, int line,
			 struct cesty *cst, int *def_cesta, int *lusti_pocet,
			 int *forb_lim, int *opt1, int *opt2);

int     lusti_kriz3_6_syl (char *file_1, char *file_2, int ignore_multiple,
			   int special_sol, int opt1, int opt2);

int     lusti_kriz3_6 (char *file_1, char *file_2, int ignore_multiple,
		       int special_sol, int opt1, int opt2);

int     lusti_kriz3_6_prompt (char *file_1, int ignore_multiple,
			      int special_sol, int opt1, int opt2);

int     lusti_kriz3_6_log (char *file_1, char *file_2, char *file_3,
			   int ignore_multiple, int special_sol, int opt1,
			   int opt2);

int     select_type (char *line_buf, char *prg_buf, int line, int *def_cesta,
		     struct cesty *cst);

void    strip_nl_char (char *line);
int     dopln_quest (struct osemsm *osm, struct osm_ries *ries,
		     struct slovo *slv);
int     dopln_quest_syl (struct osemsm_syl *osm, struct osm_ries *ries,
			 struct slovo *slv);

/* */
#endif
