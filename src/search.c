#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "num_path.h"
#include "find.h"
#include "findsyl.h"
#include "smery.h"
#include "num_path.h"
#include "osmtop.h"
#include "str_path.h"

/*
int search(OSEMSM *krz,int size,struct pismeno *wrd) {
int i,j,k,st;
struct pismeno pis;

for (i=0;i<get_size_x_osm_size(&krz->osm_size);i++)
for (j=0;j<get_size_y_osm_size(&krz->osm_size);j++) {
pis=get_pismeno(krz,i,j);
if (pis.l==wrd[0].l) {
 for (k=0;k<7;k++) {
  st=find(krz,i,j,k,size,wrd);
  if (st==-2) {fprintf(stderr,"Chyba rozmerov\n");return -1;}
  if (st) printf("%d,%d\n",i,j);
 }
}
}
return 0;
}
*/

/*
int search_2(OSEMSM *krz,struct slovo *wrd) {
int i,j,k,st,find=0;
struct pismeno pis;

for (i=0;i<get_size_x_osm_size(&krz->osm_size);i++)
for (j=0;j<get_size_y_osm_size(&krz->osm_size);j++) {
pis=get_pismeno(krz,i,j);
if (pis.l==wrd->pism[0].l) {
 for (k=0;k<8;k++) {
  st=find_2(krz,i,j,k,wrd);
  if (st==-2) {fprintf(stderr,"Chyba rozmerov\n");return -1;}
  if (st) {
#ifdef OSM_DEBUG
    printf("%d,%d -> dir: %d == %d,%d\n",i,j,k,smery_get_dx(k),smery_get_dy(k));
#endif
    find++;
  }
 }
}
}
return (find);
}
*/

/*int search_3(OSEMSM *krz,struct slovo *wrd,int all,int select) {

 int i,j,k,st,find=0,kx,ky;
 struct pismeno pis;

  for (i=0;i<get_size_x_osm_size(&krz->osm_size);i++)
  for (j=0;j<get_size_y_osm_size(&krz->osm_size);j++) {
    pis=get_pismeno(krz,i,j);
    if (pis.l==wrd->pism[0].l) {
      for (k=0;k<8;k++) {
	st=find_3(krz,i,j,k,wrd,&kx,&ky);
	if (st==-2) {
	  fprintf(stderr,"Chyba rozmerov\n");
	 return -1;
	}
	if (st) {
#ifdef OSM_DEBUG
	  printf("%d,%d -> %d,%d | dir: %d  == %d,%d\n",i,j,kx,ky,k,smery_get_dx(k),smery_get_dy(k));
#endif
	  find++;
	  if (select) select_kriz(krz,i,j,kx,ky);
	  if (!all) return 1;
	}
      }
    }
  }
  return (find);
}
*/

/*
int search_4(OSEMSM *krz,struct slovo *wrd,int all,int select) {

 int i,j,k,st,find=0,kx,ky;
 struct pismeno pis;

  for (i=0;i<get_size_x_osm_size(&krz->osm_size);i++)
  for (j=0;j<get_size_y_osm_size(&krz->osm_size);j++) {
    pis=get_pismeno(krz,i,j);
    if (pis.l==wrd->pism[0].l) {
      for (k=0;k<8;k++) {
	st=find_4(krz,i,j,k,pis.id,wrd,&kx,&ky);
	if (st==-2) {
	  fprintf(stderr,"Chyba rozmerov\n");
	 return -1;
	}
	if (st) {
#ifdef OSM_DEBUG
	  printf("%d,%d -> %d,%d | dir: %d  == %d,%d\n",i,j,kx,ky,k,smery_get_dx(k),smery_get_dy(k));
#endif
	  find++;
	  if (select) select_kriz(krz,i,j,kx,ky);
	  if (!all) return 1;
	}
      }
    }
  }
  return (find);
}
*/

/*int search_5(OSEMSM *krz,struct slovo *wrd,int all,int select) {

 int i,j,k,st,find=0,kx,ky;
 struct pismeno pis;

  for (i=0;i<get_size_x_osm_size(&krz->osm_size);i++)
  for (j=0;j<get_size_y_osm_size(&krz->osm_size);j++) {
    pis=get_pismeno(krz,i,j);
    if (pis.l==wrd->pism[0].l) {
      for (k=0;k<8;k++) {
	st=find_5(krz,i,j,k,wrd,&kx,&ky);
	if (st==-2) {
	  fprintf(stderr,"Chyba rozmerov\n");
	 return -1;
	}
	if (st) {
#ifdef OSM_DEBUG
	  printf("%d,%d -> %d,%d | dir: %d  == %d,%d\n",i,j,kx,ky,k,smery_get_dx(k),smery_get_dy(k));
#endif
	  find++;
	  if (select) select_kriz(krz,i,j,kx,ky);
	  if (!all) return 1;
	}
      }
    }
  }
  return (find);
}
*/

int
search2 (OSEMSM * krz, struct slovo *wrd, char *prg, int all, int select)
{

	int     i, j, k, st, find = 0, kx, ky;
	struct pismeno pis;
	struct num_path pth;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l) {
				for (k = 0; k < 8; k++) {
					if (!create_path_str_numpath
					    (&pth, k, prg)) {
						fprintf (stderr,
							 "Chyba: Pocas vytvarania cesty.\n");
						return 0;
					}
					else {
						st = find2 (krz, i, j, &pth,
							    wrd, &kx, &ky);
						if (st == -2) {
							fprintf (stderr,
								 "Chyba rozmerov\n");
							free_path_numpath
								(&pth);
							return 0;
						}
						if (st) {
#ifdef OSM_DEBUG
							printf (">>> search2: <<<\n");
							printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
							printf ("Program: %s\n", prg);
#endif
							find++;
							if (select)
								select_kriz2 (krz, &pth, i, j, wrd->count);	/*
														   * select
														   * kriz
														 */
							free_path_numpath
								(&pth);
							if (!all)
								return 1;
						}
						free_path_numpath (&pth);
					}
				}
			}
		}
	return (find);
}

int
search2_1 (OSEMSM * krz, struct slovo *wrd,
	   struct char_path *prg, int all, int select)
{

	int     i, j, k, st, find = 0, kx, ky;
	struct pismeno pis;
	struct num_path pth;
	struct char_path *prg_pom = prg;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l) {
				prg_pom = prg;
				while (prg_pom) {
					for (k = 0; k < 8; k++) {
						if (!create_path_str_numpath
						    (&pth, k,
						     prg_pom->elem)) {
							fprintf (stderr,
								 "Chyba: Pocas vytvarania cesty.\n");
							return 0;
						}
						else {
							st = find2 (krz, i, j,
								    &pth, wrd,
								    &kx, &ky);
							if (st == -2) {
								fprintf (stderr, "Chyba rozmerov\n");
								free_path_numpath
									(&pth);
								return 0;
							}
							if (st) {
#ifdef OSM_DEBUG
								printf (" >>> search2_1 <<<\n");
								printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
								printf ("Program: %s\n", prg_pom->elem);
#endif
								find++;
								if (select)
									select_kriz2 (krz, &pth, i, j, wrd->count);	/*
															   * select
															   * kriz
															 */
								free_path_numpath
									(&pth);
								if (!all)
									return 1;
							}
							free_path_numpath
								(&pth);
						}
					}
					prg_pom = prg_pom->next;
				}
			}
		}
	return (find);
}

int
search2_2 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	   int all, int select)
{

	int     i, j, k, l, st, find = 0, kx, ky;
	struct pismeno pis;
	struct num_path pth;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l) {
				for (l = 0; l < prg->count; l++) {
					for (k = 0; k < 8; k++) {
						if (!create_path_str2_numpath
						    (&pth, k,
						     prg->elems[l])) {
							fprintf (stderr,
								 "Chyba: Pocas vytvarania cesty.\n");
							return 0;
						}
						else {
							st = find2_1 (krz, i,
								      j, &pth,
								      wrd,
								      &kx,
								      &ky);
							if (st == -2) {
								fprintf (stderr, "Chyba rozmerov\n");
								free_path_numpath
									(&pth);
								return 0;
							}
							if (st) {
#ifdef OSM_DEBUG
								printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
								printf ("Program: %s\n", prg->elems[l]);
#endif
								find++;
								if (select)
									select_kriz2 (krz, &pth, i, j, wrd->count);	/*
															   * select
															   * kriz
															 */
								free_path_numpath
									(&pth);
								if (!all)
									return 1;
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search3 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	 struct osm_ries **ries)
{

	int     i, j, k, l, st, find = 0, kx, ky;
	struct pismeno pis;
	struct num_path pth;

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l) {
				for (l = 0; l < prg->count; l++) {
					for (k = 0; k < 8; k++) {
						if (!create_path_str_numpath
						    (&pth, k,
						     prg->elems[l])) {
							fprintf (stderr,
								 "Chyba: Pocas vytvarania cesty.\n");
							return 0;
						}
						else {
							st = find2_1 (krz, i,
								      j, &pth,
								      wrd,
								      &kx,
								      &ky);
							if (st == -2) {
								fprintf (stderr, "Chyba rozmerov\n");
								free_path_numpath
									(&pth);
								return 0;
							}
							if (st) {
#ifdef OSM_DEBUG
								printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
								printf ("Program: %s\n", prg->elems[l]);
#endif
								find++;
								if (!add_osm_ries_list (ries, i, j, k, 0, prg->elems[l])) {
									fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
									free (*ries);
									return 0;
								}
								free_path_numpath
									(&pth);
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search4 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	 struct osm_ries **ries)
{

	int     i, j, k, l, st, find = 0, kx, ky, qest2 = 0;
	struct pismeno pis;
	struct num_path pth;

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {
					for (k = 0; k < 8; k++) {
						if (!create_path_str2_numpath
						    (&pth, k,
						     prg->elems[l])) {
							fprintf (stderr,
								 "Chyba: Pocas vytvarania cesty.\n");
							return 0;
						}
						else {
							qest2 = 0;
							st = find2_2 (krz, i,
								      j, &pth,
								      wrd,
								      &kx,
								      &ky,
								      &qest2);
							if (pis.l ==
							    QUEST_CHAR)
								qest2++;
							if (st == -2) {
								fprintf (stderr, "Chyba rozmerov\n");
								free_path_numpath
									(&pth);
								return 0;
							}
							if (st) {
#ifdef OSM_DEBUG
								printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
								printf ("Program: %s\n", prg);
#endif
								find++;
								if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
									fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
									free (*ries);
									return 0;
								}
								free_path_numpath
									(&pth);
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search5 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	 struct osm_ries **ries)
{

	int     i, j, k, l, m, st, find = 0, kx, ky, qest2 = 0, pth_len;
	struct pismeno pis;
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {
					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							else {
								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);

									/*
									   * pis
									   * poziciu,
									   * oznac,
									   * ...
									 */
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
									free_path_numpath
										(&pth);
								}
								free_path_numpath
									(&pth);
							}
						}
					}
					else {	/* O cesta */

						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, buf1)) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							for (m = 0;
							     m < pth_len;
							     m++) {
								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);

/*
																			 * pis
																			 * poziciu,
																			 * oznac,
																			 * ...
																			 */
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)) {

										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								rol_path (&pth);
								rol_path_str
									(buf1,
									 buf2);
								strcpy (buf1,
									buf2);
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search5_1 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	   struct osm_ries **ries)
{

	int     i, j, k, l, st, find = 0, kx, ky, qest2 = 0, pth_len;
	struct pismeno pis;
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {
					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							else {
								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("Slovo: ");
									print_slovo
										(wrd);
									printf ("\n");
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));	/* pis poziciu, oznac, ... */
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
									free_path_numpath
										(&pth);
								}
								free_path_numpath
									(&pth);
							}
						}

					}
					else {	/* O cesta */

						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);


						do {
							for (k = 0; k < 8;
							     k++) {
								if (!create_path_str2_numpath (&pth, k, buf1)) {
									fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
									return 0;
								}

								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);

								if (pis.l ==
								    QUEST_CHAR)
									qest2++;

								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}

								if (st) {	/* naslo sa nejake riesenie */
#ifdef OSM_DEBUG
									printf ("Slovo: ");
									print_slovo
										(wrd);
									printf ("\n");
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", buf1);
#endif

									if (!is_same_sol_id (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{

											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
							}	/* end for */

							rol_path_str_ver2
								(buf1, buf2);
							strcpy (buf1, buf2);

						}
						while (cmp_o_path_str
						       (buf1,
							prg->elems[l]) !=
						       ARE_SAME);
					}	/* end else */
				}	/* end prg_count for */
			}
		}
	return find;
}

int
search5_2 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	   struct osm_ries **ries)
{

	int     i, j, k, l, st, find = 0, kx, ky, qest2 = 0, pth_len;
	struct pismeno pis;
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR
			    || wrd->pism[0].l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {
					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							else {
								qest2 = 0;
								st = find2_3
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);

								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {

									if (is_qword (wrd)) {
										print_qword_solution
											(krz,
											 i,
											 j,
											 &pth,
											 wrd);
										printf ("\n");
									}
#ifdef OSM_DEBUG
									printf ("Slovo: ");
									print_slovo
										(wrd);
									printf ("\n");
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));	/* pis poziciu, oznac, ... */
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
									free_path_numpath
										(&pth);
								}
								free_path_numpath
									(&pth);
							}
						}

					}
					else {	/* O cesta */

						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);


						do {
							for (k = 0; k < 8;
							     k++) {
								if (!create_path_str2_numpath (&pth, k, buf1)) {
									fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
									return 0;
								}

								qest2 = 0;
								st = find2_3
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);


								if (pis.l ==
								    QUEST_CHAR)
									qest2++;

								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}

								if (st) {	/* naslo sa nejake riesenie */
									if (is_qword (wrd)) {
										print_qword_solution
											(krz,
											 i,
											 j,
											 &pth,
											 wrd);
										printf ("\n");
									}

#ifdef OSM_DEBUG
									printf ("Slovo: ");
									print_slovo
										(wrd);
									printf ("\n");
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", buf1);
#endif

									if (!is_same_sol_id (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{

											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
							}	/* end for */

							rol_path_str_ver2
								(buf1, buf2);
							strcpy (buf1, buf2);

						}
						while (cmp_o_path_str
						       (buf1,
							prg->elems[l]) !=
						       ARE_SAME);
					}	/* end else */
				}	/* end prg_count for */
			}
		}
	return find;
}

int
search6_syl (OSEMSM_SYL * krz, struct slovo *wrd, struct cesty *prg,
	     struct osm_ries **ries, int opt_print_ries, int opt_print_sused)
{

	int     i, j, k, l, m, st, find = 0, kx, ky, qest2 = 0, pth_len;
	Syllabe *syl;
	struct pismeno pis = { 'a', 1 };
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			syl = get_syllabe (krz, i, j);
/*              pis = get_pismeno(krz, i, j);*/
			if (compare_word_to_syllabe (wrd, 0, syl)) {

/*		if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {*/
				for (l = 0; l < prg->count; l++) {
					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;

							}
							else {
								qest2 = 0;
								st = find2_3_syl (krz, i, j, &pth, wrd, &kx, &ky, &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id_syl (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (opt_print_ries)
											printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
										if (opt_print_sused) {
											write_sused
												((OSEMSM *) krz, i, j, kx, ky, k);
										}

										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
									free_path_numpath
										(&pth);
								}
								free_path_numpath
									(&pth);
							}
						}
					}
					else {
						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, buf1)) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							for (m = 0;
							     m < pth_len;
							     m++) {
								qest2 = 0;
								st = find2_3_syl (krz, i, j, &pth, wrd, &kx, &ky, &qest2);
								if (syl->slv.
								    pism[0].
								    l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}

								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif

									if (!is_same_sol_id_syl (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								rol_path (&pth);
								rol_path_str
									(buf1,
									 buf2);
								strcpy (buf1,
									buf2);
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search6 (OSEMSM * krz, struct slovo *wrd, struct cesty *prg,
	 struct osm_ries **ries, int opt_print_ries, int opt_print_sused)
{

	int     i, j, k, l, m, st, find = 0, kx, ky, qest2 = 0, pth_len;
	struct pismeno pis;
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {
					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;

							}
							else {
								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (opt_print_ries)
											printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
										if (opt_print_sused) {
											write_sused
												((OSEMSM *) krz, i, j, kx, ky, k);
										}

										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
									free_path_numpath
										(&pth);
								}
								free_path_numpath
									(&pth);
							}
						}
					}
					else {
						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, buf1)) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;
							}
							for (m = 0;
							     m < pth_len;
							     m++) {
								qest2 = 0;
								st = find2_2
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}

								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif

									if (!is_same_sol_id (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								rol_path (&pth);
								rol_path_str
									(buf1,
									 buf2);
								strcpy (buf1,
									buf2);
							}
							free_path_numpath
								(&pth);
						}
					}
				}
			}
		}
	return (find);
}

int
search7 (OSEMSM * krz,
	 struct slovo *wrd,
	 struct cesty *prg,
	 struct osm_ries **ries,
	 int opt_print_ries, int opt_print_sused, int forb_limit)
{

	int     i, j, k, l, m, st, find = 0, kx, ky, qest2 = 0, pth_len;
	struct pismeno pis;
	struct num_path pth;
/*	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];*/
	char *buf1,*buf2;

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l || pis.l == QUEST_CHAR) {
				for (l = 0; l < prg->count; l++) {

					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;

							}
							else {
								qest2 = 0;
								st = find3
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2,
									 forb_limit);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;
										if (opt_print_ries)
											printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
										if (opt_print_sused) {
											write_sused
												(krz,
												 i,
												 j,
												 kx,
												 ky,
												 k);
										}

										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
							}
						}
					}
					else {	/* O path */
			pth_len = get_length_path (prg-> elems [l]);
				
			buf1 = (char *)malloc(sizeof(char)*(strlen(prg->elems[l])+1));
			if (buf1 == NULL) {
				fprintf(stderr, "Error: Not enough memory.\n");
				return 0;
			}
			strcpy (buf1, prg->elems[l]);
						for (k = 0; k < 8; k++) {
							for (m = 0;
							     m < pth_len;
							     m++) {
								if (!create_path_str2_numpath (&pth, k, buf1)) {
									fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
									free((void *)buf1);
									return 0;
								}
								qest2 = 0;
								st = find3
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2,
									 forb_limit);
								if (pis.l ==
								    QUEST_CHAR)
									qest2++;
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									free((void *)buf1);
									return 0;
								}

								if (st) {

#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", buf1);

									printf ("HERE?\n");
#endif
									if (!is_same_sol_id (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											free((void *)buf1);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
								buf2 = (char *)malloc(sizeof(char)*(strlen(buf1)+2));
								if (buf2 == NULL) {
									fprintf(stderr, "Error: Memory allocation error.\n");
									return 0;
								}
								rol_path_str_ver2
									(buf1,
									 buf2);
								strcpy (buf1,
									buf2);
								free((void *)buf2);
							}
						}
						free((void *)buf1);
					}
				}
			}
		}
	return (find);
}

int
search7_syl (OSEMSM_SYL * krz,
	     struct slovo *wrd,
	     struct cesty *prg,
	     struct osm_ries **ries,
	     int opt_print_ries, int opt_print_sused, int forb_limit)
{

	int     i, j, k, l, m, st, find = 0, kx, ky, qest2 = 0, pth_len;

/*struct pismeno  pis;*/
	Syllabe *syl;
	struct num_path pth;
	char    buf1[PRG_BUFFER_SIZE], buf2[PRG_BUFFER_SIZE];

	*ries = NULL;

	for (i = 0; i < get_size_x_osm_size (&krz->osm_size); i++)
		for (j = 0; j < get_size_y_osm_size (&krz->osm_size); j++) {
			syl = get_syllabe (krz, i, j);
			if (compare_word_to_syllabe (wrd, 0, syl)) {
				for (l = 0; l < prg->count; l++) {

					if (prg->elems[l][0] != 'O') {
						for (k = 0; k < 8; k++) {
							if (!create_path_str2_numpath (&pth, k, prg->elems[l])) {
								fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
								return 0;

							}
							else {
								qest2 = 0;
								st = find3_syl
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2,
									 forb_limit);

/*							if (pis.l == QUEST_CHAR)
						   		qest2++;*/
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}
								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif
									if (!is_same_sol_id_syl (krz, *ries, i, j, k)
									    &&
									    !is_same_sol_palindrom
									    (*ries,
									     wrd,
									     kx,
									     ky,
									     k))
									{
										find++;

										if (opt_print_ries)
											printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
										if (opt_print_sused) {
											write_sused_syl
												(krz,
												 i,
												 j,
												 kx,
												 ky,
												 k);
										}

										if (!add_osm_ries_list (ries, i, j, k, qest2, prg->elems[l])) {
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
							}
						}
					}
					else {	/* O path */
						pth_len =
							get_length_path (prg->
									 elems
									 [l]);
						strcpy (buf1, prg->elems[l]);


						for (k = 0; k < 8; k++) {
							for (m = 0;
							     m < pth_len;
							     m++) {
								if (!create_path_str2_numpath (&pth, k, buf1)) {
									fprintf (stderr, "Chyba: Pocas vytvarania cesty.\n");
									return 0;
								}
								qest2 = 0;
								st = find3_syl
									(krz,
									 i, j,
									 &pth,
									 wrd,
									 &kx,
									 &ky,
									 &qest2,
									 forb_limit);
								/*if (pis.l == QUEST_CHAR)
								   qest2++; */
								if (st == -2) {
									fprintf (stderr, "Chyba rozmerov\n");
									free_path_numpath
										(&pth);
									return 0;
								}

								if (st) {
#ifdef OSM_DEBUG
									printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery_get_dx (k), smery_get_dy (k));
									printf ("Program: %s\n", prg);
#endif

									if (!is_same_sol_id_syl (krz, *ries, i, j, k)) {
										find++;
										if (!add_osm_ries_list (ries, i, j, k, qest2, buf1)
										    &&
										    !is_same_sol_palindrom
										    (*ries,
										     wrd,
										     kx,
										     ky,
										     k))
										{
											fprintf (stderr, "Chyba pocas vytvarania zoznamov.\n");
											free (*ries);
											return 0;
										}
									}
								}
								free_path_numpath
									(&pth);
								rol_path_str_ver2
									(buf1,
									 buf2);
								strcpy (buf1,
									buf2);
							}
						}
					}
				}
			}
		}
	return (find);
}

int
is_same_sol_id (struct osemsm *krz, struct osm_ries *o_ries, int start_x,
		int start_y, int dir)
{

	struct pismeno pis;
	struct osm_ries *ries = o_ries;
	int     id;

	pis = get_pismeno (krz, start_x, start_y);
	id = pis.id;
	if (!o_ries)
		return 0;
	if (!pis.id)
		return 0;

	while (ries) {
		pis = get_pismeno (krz, ries->pos_x, ries->pos_y);
		if (pis.id == id && ries->dir_num == dir)
			return 1;	/*
					   * je
					   * rovnake
					   * riesenie
					   * ako
					   * zname
					 */
		ries = ries->next;
	}
	return 0;
}

int
is_same_sol_palindrom (struct osm_ries *o_ries, struct slovo *slv, int end_x,
		       int end_y, int dir_num)
{

	struct osm_ries *ries = o_ries;
	int     dif;

	if (!is_palindrom (slv))
		return 0;

	while (ries) {
		dif = ries->dir_num - dir_num;
		if (dif < 0)
			dif = -dif;
		if (ries->pos_x == end_x && ries->pos_y == end_y && dif == 4)
			return 1;	/*
					   * je
					   * rovnake
					   * palindromicke
					   * riesenie
					 */
		ries = ries->next;
	}
	return 0;
}

int
is_same_sol_id_syl (OSEMSM_SYL * krz, struct osm_ries *o_ries, int start_x,
		    int start_y, int dir)
{

	Syllabe *syl;
	struct osm_ries *ries = o_ries;
	int     id;

	syl = get_syllabe (krz, start_x, start_y);

	id = syl->id;

	if (!o_ries)		/* no solution -  ignoring */
		return 0;
	if (!id)		/* no set id - ignoring */
		return 0;

	while (ries) {
		syl = get_syllabe (krz, ries->pos_x, ries->pos_y);

		if (syl->id == id && ries->dir_num == dir)
			return 1;	/* solution is already known */

		ries = ries->next;
	}

	return 0;
}

void
print_qword_solution (OSEMSM * krz, int st_x, int st_y, struct num_path *pth,
		      SLOVO * wrd)
{
	int     len = 0, pos_x = st_x, pos_y =
		st_y, new_pos_x, new_pos_y, act_dir;

	while (len < wrd->count) {
		if (wrd->pism[len].l == QUEST_CHAR) {
			printf ("%s", get_pism ((int)
						get_pismeno (krz, pos_x,
							     pos_y).l));
			printf ("(%2d,%2d)", pos_x, pos_y);
		}
		else
			printf ("%s", get_pism ((int) wrd->pism[len].l));

		get_dir_num_numpath (pth, len, &act_dir);
		get_next_word_in_dir2 (krz, pos_x, pos_y, act_dir, &new_pos_x,
				       &new_pos_y);

		pos_x = new_pos_x;
		pos_y = new_pos_y;
		len++;
	}			/* end while */

}
