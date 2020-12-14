#ifndef _OSM_SEARCH_H_
#define _OSM_SEARCH_H_

#include "flosm.h"
#include "flosmsyl.h"
#include "charpath.h"
#include "cesty.h"
#include "osm_ries.h"

/* int search(OSEMSM *krz,int size,struct pismeno *wrd)*/

/* int search_2(OSEMSM *krz,struct slovo *wrd) */

/* int search_3(OSEMSM *krz,struct slovo *wrd,int all,int select) */

/* int search_4(OSEMSM *krz,struct slovo *wrd,int all,int select) */

/* int search_5(OSEMSM *krz,struct slovo *wrd,int all,int select) */

int     search2 (OSEMSM * krz, struct slovo *wrd, char *prg, int all,
		 int select);

int     search2_1 (OSEMSM * krz, struct slovo *wrd, struct char_path *prg,
		   int all, int select);

int     search2_2 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		   int all, int select);

int     search3 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		 struct osm_ries **ries);

int     search4 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		 struct osm_ries **ries);

int     search5 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		 struct osm_ries **ries);

int     search5_1 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		   struct osm_ries **ries);

int     search5_2 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		   struct osm_ries **ries);

int     search6_syl (OSEMSM_SYL * krz, struct slovo *wrd, struct cesty *prg,
		     struct osm_ries **ries, int opt_print_ries,
		     int opt_print_sused);

int     search6 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		 struct osm_ries **ries, int opt_print_ries,
		 int opt_print_sused);

int     search7 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
		 struct osm_ries **ries, int opt_print_ries,
		 int opt_print_sused, int forb_limit);

int     search7_syl (OSEMSM_SYL * krz, struct slovo *wrd, struct cesty *prg,
		     struct osm_ries **ries, int opt_print_ries,
		     int opt_print_sused, int forb_limit);

int     is_same_sol_id (struct osemsm *krz, struct osm_ries *o_ries,
			int start_x, int start_y, int dir);

int     is_same_sol_palindrom (struct osm_ries *o_ries, struct slovo *slv,
			       int end_x, int end_y, int dir_num);

int     is_same_sol_id_syl (OSEMSM_SYL * krz, struct osm_ries *o_ries,
			    int start_x, int start_y, int dir);
void    print_qword_solution (OSEMSM * krz, int st_x, int st_y,
			      struct num_path *pth, SLOVO * wrd);
#endif
