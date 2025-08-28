#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define EMPTY_CHAR '\0'
#define DIR_DELIM '/'
#define OSM_DEBUG
#undef OSM_DEBUG
#define MY_UINT_BITSIZE	( (unsigned int)(8 * sizeof(unsigned int)) )
char   *pism[] =
	{ "", "A", ",A", ":A", "B", "C", "vC", "D", "vD", "`DZ", "`DvZ", "E",
	",E", "F", "G", "H", "`CH", "I", ",I", "J", "K", "L", ",L", "vL", "M",
	"N",
	"vN", "O",
	",O", "vO", "P", "Q", "R", ",R", "S", "vS", "T", "vT", "U", ",U", "V",
	"W", "X", "Y",
	",Y", "Z", "vZ"
};
typedef struct pismeno
{
	char    l;
	int     id;
}
PISMENO;
typedef struct slovo
{
	int     count;
	struct pismeno *pism;
}
SLOVO;
typedef struct sestsm
{
	int     siz_x, siz_y, id;
	struct pismeno *pole;
	unsigned int *bitmap;
}
SESTSM;
typedef struct elem
{
	char    key;
	int     value;
}
ELEM;
typedef struct table
{
	int     pocet;
	struct elem *hodn;
}
TABLE;

/*
			    1                        2                        3                       4
 1  2  3 4 5  6 7  8   9    0 1  2 3 4 5   6  7  8 9 0 1  2  3 4 5  6 7  8  9 0 1 2  3 4  5 6  7 8  9 0 1 2 3  4 5  6
 a ,a :a b c vc d vd `dz `dvz e ,e f g h `ch  i ,i j k l ,l vl m n vn o ,o vo p q r ,r s vs t vt u ,u v w x y ,y z vz
*/
int
is_in_table (struct table *tbl, char key)
{
	int     i;

	for (i = 0; i < tbl->pocet; i++) {
		if (tbl->hodn[i].key == key)
			return 1;
	}
	return 0;
}
struct pismeno
get_table (struct table *tbl, char key)
{
	int     i;
	struct pismeno pis;

	pis.l = 0;
	pis.id = 0;
	for (i = 0; i < tbl->pocet; i++) {
		if (tbl->hodn[i].key == key) {
			pis.l = (char) tbl->hodn[i].value;
			return pis;
		}
	}
	return pis;
}

ELEM    all_elem[] = {
	{
	 'A', 1}
	,
	{
	 'B', 4}
	,
	{
	 'C', 5}
	,
	{
	 'D', 7}
	,
	{
	 'E', 11}
	,
	{
	 'F', 13}
	,
	{
	 'G', 14}
	,
	{
	 'H', 15}
	,
	{
	 'I', 17}
	,
	{
	 'J', 19}
	,
	{
	 'K', 20}
	,
	{
	 'L', 21}
	,
	{
	 'M', 24}
	,
	{
	 'N', 25}
	,
	{
	 'O', 27}
	,
	{
	 'P', 30}
	,
	{
	 'Q', 31}
	,
	{
	 'R', 32}
	,
	{
	 'S', 34}
	,
	{
	 'T', 36}
	,
	{
	 'U', 38}
	,
	{
	 'V', 40}
	,
	{
	 'W', 41}
	,
	{
	 'X', 42}
	,
	{
	 'Y', 43}
	,
	{
	 'Z', 45}
};
struct table all_table = { 26, all_elem };
ELEM    dlz_elem[] = {

	{
	 'A', 2}
	,
	{
	 'E', 12}
	,
	{
	 'I', 18}
	,
	{
	 'L', 22}
	,
	{
	 'O', 28}
	,
	{
	 'R', 33}
	,
	{
	 'U', 39}
	,
	{
	 'Y', 44}
};
struct table dlz_table = { 9, dlz_elem };
ELEM    mak_elem[] = {

	{
	 'C', 6}
	,
	{
	 'D', 8}
	,
	{
	 'L', 23}
	,
	{
	 'N', 26}
	,
	{
	 'O', 29}
	,
	{
	 'S', 35}
	,
	{
	 'T', 37}
	,
	{
	 'Z', 46}
};
struct table mak_table = { 8, mak_elem };
ELEM    bod_elem[] = {
	{
	 'A', 3}
};
struct table bod_table = { 1, bod_elem };
int     smery[][2] =
	{ {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1,
								       -1}
};
int     get_pos (SESTSM * krz, int pos_x, int pos_y);
struct pismeno get_pismeno (SESTSM * krz, int pos_x, int pos_y);
int     is_in_kriz (SESTSM * krz, int pos_x, int pos_y);
int     is_non_void (SESTSM * krz, int pos_x, int pos_y);
void    set_pismeno (SESTSM * krz, int pos_x, int pos_y, struct pismeno chr);
//int     find (SESTSM * krz, int pos_x, int pos_y, int dir, int count,
//	      struct pismeno *wrd);
void    select_kriz_2 (struct sestsm *ssm, int sx, int sy, int dir,
		       int count);

/*void select_kriz(struct sestsm *ssm,int sx,int sy,int ex,int ey);*/
int     check_slovo_0 (char **poc);
int
get_pos (SESTSM * krz, int pos_x, int pos_y)
{
	int     pom;

	pom = pos_y * krz->siz_x;
	pom -= (pos_y / 2) - (pos_y % 2 == 0 ? krz->id:0);
	pom -= krz->id;
	pom += pos_x;
	return pom;
}
struct pismeno
get_pismeno (SESTSM * krz, int pos_x, int pos_y)
{
	return krz->pole[get_pos (krz, pos_x, pos_y)];
}

int
is_in_kriz (SESTSM * krz, int pos_x, int pos_y)
{
	if (pos_x < 0 || pos_x >= krz->siz_x || pos_y < 0
	    || pos_y >= krz->siz_y)
		return 0;
	if ( (pos_y + krz->id ) % 2 ) {
		if (pos_x >= krz->siz_x - 1)
			return 0;
	}
	return 1;
}

int
is_non_void (SESTSM * krz, int pos_x, int pos_y)
{
	struct pismeno pom;

	pom = get_pismeno (krz, pos_x, pos_y);
	if (pom.l == EMPTY_CHAR)
		return 0;
	else
		return 1;
}

void
set_pismeno (SESTSM * krz, int pos_x, int pos_y, struct pismeno chr)
{
	if (is_in_kriz (krz, pos_x, pos_y))
		krz->pole[get_pos (krz, pos_x, pos_y)] = chr;
}


/*int find(SESTSM *krz,int pos_x,int pos_y,int dir,int count,struct pismeno* wrd) {

 int pos=0,match_count=1,match=1,new_pos_x,new_pos_y;
 struct pismeno pis;

 if (pos_x>krz->siz_x || pos_x<0 || pos_y<0 || pos_y>krz->siz_y) {fprintf(stderr,"Chyba nemozno vyhladavat mimo krizovky.\n");return -2;}
 while (match && match_count<count) {
  pos++;
  new_pos_x=smery[dir][0]+pos_x;
  new_pos_y=smery[dir][1]+pos_y;

  if (!is_in_kriz,krz,new_pos_x,new_pos_y) {
   match=0;
  }
  else {
   pis=get_pismeno(krz,new_pos_x,new_pos_y);
   if (pis.l==wrd[pos].l) {match_count++; pos_x=new_pos_x;pos_y=new_pos_y;}
   else match=0;
  };
 }
 if (match) return 1;
 else return 0;
}
*/

/*int find_2(OSEMSM *krz,int pos_x,int pos_y,int dir,struct slovo *wrd) {

 int pos=0,match_count=1,match=1,new_pos_x,new_pos_y;
 struct pismeno pis;

 if (!is_in_kriz(krz,pos_x,pos_y)) {
   fprintf(stderr,"Chyba nemozno vyhladavat mimo krizovky.\n");
   return -2;
 }

 while (match && match_count<wrd->count) {
   pos++;
   new_pos_x=smery[dir][0]+pos_x;
   new_pos_y=smery[dir][1]+pos_y;

   if (!is_in_kriz(krz,new_pos_x,new_pos_y) || !is_non_void(krz,new_pos_x,new_pos_y)) {
     match=0;
   }
   else {
     pis=get_pismeno(krz,new_pos_x,new_pos_y);
     if (pis.l==wrd->pism[pos].l) {
       match_count++;
       pos_x=new_pos_x;
       pos_y=new_pos_y;
     }
     else
       match=0;
   }
 }
 if (match) return 1;
 else return 0;
}
*/
int
find_3 (SESTSM * krz, int pos_x, int pos_y, int dir, struct slovo *wrd,
	int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match = 1, new_pos_x =
		pos_x, new_pos_y = pos_y;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		new_pos_x = smery[dir][0] + pos_x;
		new_pos_y = smery[dir][1] + pos_y;
		if (!is_in_kriz (krz, new_pos_x, new_pos_y)
		    || !is_non_void (krz, new_pos_x, new_pos_y)) {
			match = 0;
		}

		else {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}

	else
		return 0;
}


/*int find_4(OSEMSM *krz,int pos_x,int pos_y,int dir,int id,struct slovo *wrd,int *kon_pos_x,int *kon_pos_y) {

 int pos=0,match_count=1,match=1,new_pos_x,new_pos_y,new_id;
 struct pismeno pis;

 if (!is_in_kriz(krz,pos_x,pos_y)) {
   fprintf(stderr,"Chyba nemozno vyhladavat mimo krizovky.\n");
   return -2;
 }

 while (match && match_count<wrd->count) {

   pos++;
   new_pos_x=smery[dir][0]+pos_x;
   new_pos_y=smery[dir][1]+pos_y;

   if (id) {
     new_id=(get_pismeno(krz,new_pos_x,new_pos_y)).id;
     while (id==new_id) {
       pos_x=new_pos_x;
       pos_y=new_pos_y;
       new_pos_x=smery[dir][0]+pos_x;
       new_pos_y=smery[dir][1]+pos_y;
       new_id=(get_pismeno(krz,new_pos_x,new_pos_y)).id;
     }
   }

   if (!is_in_kriz(krz,new_pos_x,new_pos_y) || !is_non_void(krz,new_pos_x,new_pos_y)) {
     match=0;
   }
   else {
     pis=get_pismeno(krz,new_pos_x,new_pos_y);
     if (pis.l==wrd->pism[pos].l) {
       match_count++;
       pos_x=new_pos_x;
       pos_y=new_pos_y;
       id=pis.id;
     }
     else
       match=0;
   }
  }
 if (match) {
   *kon_pos_x=new_pos_x;
   *kon_pos_y=new_pos_y;
   return 1;
 }
 else return 0;
}
*/
int     get_next_field (struct sestsm *krz, int pos_x, int pos_y, int dir,
			int *new_x, int *new_y);
int
get_next_field (struct sestsm *krz, int pos_x, int pos_y, int dir, int *new_x,
		int *new_y)
{
	int     pom = pos_y % 2;

	switch (dir) {
	case 0:
		*new_x = pos_x + ((pom == krz->id) ? 0 : 1);
		*new_y = pos_y - 1;
		break;
	case 1:
		*new_x = pos_x + 1;
		*new_y = pos_y;
		break;
	case 2:
		*new_x = pos_x + ((pom == krz->id) ? 0 : 1);
		*new_y = pos_y + 1;
		break;
	case 3:
		*new_x = pos_x - ((pom == krz->id) ? 1 : 0);
		*new_y = pos_y + 1;
		break;
	case 4:
		*new_x = pos_x - 1;
		*new_y = pos_y;
		break;
	case 5:
		*new_x = pos_x - ((pom == krz->id) ? 1 : 0);
		*new_y = pos_y - 1;
		break;
	}
	return 1;
}

int
find_5 (SESTSM * krz, int pos_x, int pos_y, int dir, struct slovo *wrd,
	int *kon_pos_x, int *kon_pos_y)
{
	int     pos = 0, match_count = 1, match = 1, new_pos_x = 0, new_pos_y = 0;
	struct pismeno pis;

	if (!is_in_kriz (krz, pos_x, pos_y)) {
		fprintf (stderr, "Chyba nemozno vyhladavat mimo krizovky.\n");
		return -2;
	}
	while (match && match_count < wrd->count) {
		pos++;
		get_next_field (krz, pos_x, pos_y, dir, &new_pos_x,
				&new_pos_y);
		if (!is_in_kriz (krz, new_pos_x, new_pos_y)
		    || !is_non_void (krz, new_pos_x, new_pos_y)) {
			match = 0;
		}

		else {
			pis = get_pismeno (krz, new_pos_x, new_pos_y);
			if (pis.l == wrd->pism[pos].l) {
				match_count++;
				pos_x = new_pos_x;
				pos_y = new_pos_y;
			}

			else
				match = 0;
		}
	}
	if (match) {
		*kon_pos_x = new_pos_x;
		*kon_pos_y = new_pos_y;
		return 1;
	}

	else
		return 0;
}


/*
int search(OSEMSM *krz,int size,struct pismeno *wrd) {
int i,j,k,st;
struct pismeno pis;

for (i=0;i<krz->siz_x;i++)
for (j=0;j<krz->siz_y;j++) {
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

/*int search_2(OSEMSM *krz,struct slovo *wrd) {
int i,j,k,st,find=0;
struct pismeno pis;

for (i=0;i<krz->siz_x;i++)
for (j=0;j<krz->siz_y;j++) {
pis=get_pismeno(krz,i,j);
if (pis.l==wrd->pism[0].l) {
 for (k=0;k<8;k++) {
  st=find_2(krz,i,j,k,wrd);
  if (st==-2) {fprintf(stderr,"Chyba rozmerov\n");return -1;}
  if (st) {
#ifdef OSM_DEBUG
    printf("%d,%d -> dir: %d == %d,%d\n",i,j,k,smery[k][0],smery[k][1]);
#endif
    find++;
  }
 }
}
}
return (find);
}
*/
int
search_3 (SESTSM * krz, struct slovo *wrd, int all, int select)
{
	int     i, j, k, st, find = 0, kx, ky;
	struct pismeno pis;

	for (j = 0; j < krz->siz_y; j++)
		for (i = 0; i < (krz->siz_x - ((krz->id == (j % 2)) ? 0 : 1));
		     i++) {
			pis = get_pismeno (krz, i, j);
			if (pis.l == wrd->pism[0].l) {
				for (k = 0; k < 6; k++) {
					st = find_5 (krz, i, j, k, wrd, &kx,
						     &ky);
					if (st == -2) {
						fprintf (stderr,
							 "Chyba rozmerov\n");
						return -1;
					}
					if (st) {

#ifdef OSM_DEBUG
						printf ("%d,%d -> %d,%d | dir: %d  == %d,%d\n", i, j, kx, ky, k, smery[k][0], smery[k][1]);	/* pis poziciu, oznac, ... */

#endif /*  */
						find++;
						if (select)
							select_kriz_2 (krz, i, j, k, wrd->count);	/* select kriz */
						if (!all)
							return 1;
					}
				}
			}
		}
	return (find);
}


/*int search_4(OSEMSM *krz,struct slovo *wrd,int all,int select) {

 int i,j,k,st,find=0,kx,ky;
 struct pismeno pis;

  for (i=0;i<krz->siz_x;i++)
  for (j=0;j<krz->siz_y;j++) {
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
	  printf("%d,%d -> %d,%d | dir: %d  == %d,%d\n",i,j,kx,ky,k,smery[k][0],smery[k][1]);
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
check_char (char **poc, struct table *tbl, char *msg_1, char *msg_2)
{
	char   *pom = *poc;

	pom++;
	if (*pom) {
		if (!is_in_table (tbl, *pom)) {
			fprintf (stderr, "Chyba: %s\n", msg_1);

			/*Za , musi byt pismeno ktore ma dlzen.\n"); */
			return 0;
		}

		else {
			*poc = pom;
			return 1;
		}
	}

	else {
		fprintf (stderr, "Chyba: %s\n", msg_2);

		/*Za , musi nasledovat pismeno.\n"); */
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


/*int check_char_r(char **poc,int *r) {

   char *pom=*poc;

   if (!*pom) {
     fprintf(stderr,"Chyba: Za r musi nasledovat znak.\n");
     return 0;
   }
   if (*pom<'0' || *pom>'9') {
     fprintf(stderr,"Chyba: Za r musi nasledovat cislica.\n");
     return 0;
   }
   if (*pom=='0') {
     fprintf(stderr,"Chyba: Za r musi nasledovat nenulova cislica.\n");
     return 0;
   }
   *poc=pom;
   *r=(int) *pom-'0';
   return 1;
} */

/*int check_char_s(char **poc,int *s) {

   char *pom=*poc;

   if (!*pom) {
     fprintf(stderr,"Chyba: Za s musi nasledovat znak.\n");
     return 0;
   }
   if (*pom<'0' || *pom>'9') {
     fprintf(stderr,"Chyba: Za s musi nasledovat cislica.\n");
     return 0;
   }
   if (*pom=='0') {
     fprintf(stderr,"Chyba: Za s musi nasledovat nenulova cislica.\n");
     return 0;
   }
   *poc=pom;
   *s=(int) *pom-'0';
   return 1;
} */

/*int check_char_3(char **poc,int *r,int *s) {
 char *pom=*poc;
 int r1=0,s1=0;

 pom++;
 if (!*pom) {
   fprintf(stderr,"Chyba: Za b musi nasledovat znak.\n");
   return 0;
 }
 if (*pom!='s' && *pom!='r') {
   fprintf(stderr,"Chyba: Za b musi nasledovat r alebo s.\n");
   return 0;
 }
 if (*pom=='s') {
   pom++;
   if (!check_char_s(&pom,s)) return 0;
   s1=1;
 }
 if (*pom=='r') {
   pom++;
   if (!check_char_r(&pom,r)) return 0;
   r1=1;
 }
 pom++;
 if (!*pom) {
   fprintf(stderr,"Za b(r|s) musi byt znak.\n");
   return 0;
 }
 if (*pom=='s' || *pom=='r') {
   if (*pom=='s') {
     pom++;
     if (!check_char_s(&pom,s)) return 0;
     if (s1) {
       fprintf(stderr,"Chyba: Dvakrat sa nastavuje hodnota s.\n");
       return 0;
     }
   }
   if (*pom=='r') {
     pom++;
     if (!check_char_r(&pom,r)) return 0;
     if (r1) {
       fprintf(stderr,"Chyba: Dvakrat sa nastavuje hodnota r.\n");
       return 0;
     }
   }
   pom++;
 }
 if (!check_slovo_0(&pom)) return 0;
 *poc=pom;
 return 1;
} */
int
check_slovo_0 (char **poc)
{
	char   *pom = *poc;

	while (*pom) {
		switch (*pom) {
		case '.':
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
		}		/* end switch */
		pom++;
	}			/* end while */
	*poc = pom;
	return 1;
}

int
check_slovo (char *poc)
{
	char   *pom = poc;

/*  int r, s;*/
	while (*pom) {
		switch (*pom) {

/*      case 'b':
	if (!check_char_3(&pom,&r,&s)) return 0;
      break;*/
		case 'm':
			break;
		case '.':
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
		}		/* end switch */
		pom++;
	}			/* end while */
	return 1;
}

int
count_slovo (char *poc)
{
	int     count = 0;

	/*int r, s; */
	char   *pom = poc;

	if (!check_slovo (pom))
		return 0;
	pom = poc;
	while (*pom) {
		switch (*pom) {

/*      case 'b':
	check_char_3(&pom,&r,&s);
	count++;
      break;*/

/*      case 'm':
       count++;*/
		case '.':
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
		case '.':
			slv->pism[i].l = EMPTY_CHAR;
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
void
free_kriz (struct sestsm *ssm)
{
	if (ssm->bitmap) {
		free ((void *) ssm->bitmap);
		ssm->bitmap = NULL;
	}
	if (ssm->pole) {
		free ((void *) ssm->pole);
		ssm->pole = NULL;
	}
	ssm->siz_x = 0;
	ssm->siz_y = 0;
	ssm->id = 0;
}

int
create_kriz (char *filename, struct sestsm *ssm)
{
	FILE   *fil;
	int /*c, */ siz_x, siz_y, id, /*eoln, */ count, ix = 0, iy =
	0,      pom_count, i, cnt;

/*  char buf[2]={'\0','\0'};*/
	int     line = 0;
	char    line_buf[256];
	struct slovo pom;

/*  struct pismeno pis;*/
	fil = fopen (filename, "rt");
	if (fil == NULL) {
		fprintf (stderr, "Chyba: Nepodarilo sa otvorit subor '%s'.\n",
			 filename);
		return 0;
	}
	if (fgets (line_buf, 256, fil) == NULL) {
		fprintf (stderr, "Chyba: Subor je prazdny.\n");
		fclose (fil);
		return 0;
	}
	if (sscanf (line_buf, "%d %d %d", &siz_x, &siz_y, &id) < 3) {
		fprintf (stderr,
			 "Chyba: Na prvom riadku suboru musia byt rozmery krizovky.\n");
		fclose (fil);
		return 0;
	}
	if (siz_x < 1 || siz_y < 1) {
		fprintf (stderr,
			 "Chyba: Rozmery krizovky musia byt kladne.\n");
		fclose (fil);
		return 0;
	}
	if (id != 0 && id != 1) {
		fprintf (stderr, "Chyba: Id musi byt 0 alebo 1.\n");
		fclose (fil);
		return 0;
	}

#ifdef OSM_DEBUG
	printf ("%d,%d:%d\n", siz_x, siz_y, id);

#endif /*  */
	count = siz_x * siz_y;
	pom_count = count / MY_UINT_BITSIZE + ((count % MY_UINT_BITSIZE) ? 1 : 0);
	count -= ((siz_y / 2) + id);
	ssm->pole =
		(struct pismeno *) malloc (count * sizeof (struct pismeno));
	if (ssm->pole == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre pole.\n");
		fclose (fil);
		return 0;
	}
	ssm->bitmap =
		(unsigned int *) malloc ((pom_count + 1) *
					 sizeof (unsigned int));
	if (ssm->bitmap == NULL) {
		fprintf (stderr,
			 "Chyba: Nepodarilo sa alokovat pamat pre bitmap.\n");
		free ((void *) ssm->pole);
		fclose (fil);
		return 0;
	}
	ssm->siz_x = siz_x;
	ssm->siz_y = siz_y;
	ssm->id = id;
	for (i = 0; i <= pom_count; i++)
		ssm->bitmap[i] = 0;
	while (!feof (fil) && line < siz_y) {
		if (fgets (line_buf, 256, fil)) {
			line++;
			if (line_buf[strlen (line_buf) - 1] == '\n')
				line_buf[strlen (line_buf) - 1] = '\0';	/* odstran \n z konca riadka */
			if (line_buf[strlen (line_buf) - 1] == '\r')
				line_buf[strlen (line_buf) - 1] = '\0';	/* -'\r' pre unix cit. dos subory. */
			if (!check_slovo (line_buf)) {
				fprintf (stderr,
					 "Chyba na riadku %d v subore %s - chybne znaky.\n",
					 line + 1, filename);
				free_kriz (ssm);
				fclose (fil);
				return 0;
			}
			cnt = count_slovo (line_buf);
			if (cnt == 0)
				line--;	/* ignore empty lines */

			else {
				int     pom_cnt =
					siz_x - ((id + line + 1) % 2);

				if (cnt != pom_cnt) {
					if (cnt < pom_cnt)
						fprintf (stderr,
							 "Chyba na riadku %d v subore %s - obsahuje menej ako %d pismen.\n",
							 line + 1, filename,
							 pom_cnt);

					else
						fprintf (stderr,
							 "Chyba na riadku %d v subore %s - obsahuje viacej ako %d pismen.\n",
							 line + 1, filename,
							 pom_cnt);
					free_kriz (ssm);
					fclose (fil);
					return 0;
				}

				else if (create_slovo (line_buf, &pom)) {
					for (ix = 0;
					     ix <
					     siz_x - ((id + line - 1) % 2);
					     ix++) {
						set_pismeno (ssm, ix, iy,
							     pom.pism[ix]);
					}
					iy++;
					count -= siz_x -
						((id + line + 1) % 2);
					free_slovo (&pom);
				}

				else {
					fprintf (stderr,
						 "Chyba: Pocas vytvarania slova.\n");
					free_kriz (ssm);
					fclose (fil);
					return 0;
				}
			}
		}
	}			/* end while */
	if (count != 0) {
		fprintf (stderr, "Chyba: Pocas nacitavania obrazca.\n");
		free_kriz (ssm);
		return 0;
	}
	fclose (fil);
	return 1;
}

void
select_kriz_2 (struct sestsm *ssm, int sx, int sy, int dir, int count)
{
	int     px = sx, py = sy, i, co, co_i, co_b;

	for (i = 0; i < count; i++) {
		co = get_pos (ssm, px, py);
		co_i = co / MY_UINT_BITSIZE;
		co_b = co % MY_UINT_BITSIZE;
		ssm->bitmap[co_i] |= ((unsigned int)1 << co_b);
		get_next_field (ssm, px, py, dir, &px, &py);
	}
}


/*void select_kriz(struct sestsm *ssm,int sx,int sy,int ex,int ey) {

 int px=ex-sx,py=ey-sy,ax,ay,d,i,co,co_i,co_b;

 if (px<0) ax=-px; else ax=px;
 if (py<0) ay=-py; else ay=py;
 if (ax==0) d=ay; else d=ax;
 d++;
 if (ax>0) px=px/ax;
 if (ay>0) py=py/ay;
 for (i=0;i<d;i++) {
   co=sx+sy*osm->siz_x;
   co_i=co / MY_UINT_SIZE;
   co_b=co % MY_UINT_SIZE;
   osm->bitmap[co_i]|=(1<<co_b);
   sx+=px;
   sy+=py;
 }
}
*/
void
print_kriz (struct sestsm *ssm)
{
	int     i, j, co, co_i, co_b, size = 0;

	for (j = 0; j < ssm->siz_y; j++)
		for (i = 0; i < ssm->siz_x - ((j + ssm->id) % 2); i++) {
			co = get_pos (ssm, i, j);
			co_i = co / MY_UINT_BITSIZE;
			co_b = co % MY_UINT_BITSIZE;
			if (!(ssm->bitmap[co_i] & (1 << co_b))
			    && (get_pismeno (ssm, i, j)).l != EMPTY_CHAR) {
				printf ("%s",
					pism[(int) (get_pismeno (ssm, i, j)).
					     l]);
				size++;
			}
		}
	printf ("\n%d\n", size);
}


/*void print_kriz_2(struct osemsm *osm) {
int i,j,co,co_i,co_b;
for (j=0;j<osm->siz_y;j++)
for (i=0;i<osm->siz_x;i++) {
  co=i+j*osm->siz_x;
  co_i=co / MY_UINT_SIZE;
  co_b=co % MY_UINT_SIZE;
 // if (co_b) co_i++;
  if (!(osm->bitmap[co_i] & (1 << co_b)) && !(get_pismeno(osm,i,j).id)) {
    printf("%s",pism[(get_pismeno(osm,i,j)).l],(get_pismeno(osm,i,j)).l);
  }
}
printf("\n");
}
*/
int
lusti_kriz (char *file_1, char *file_2)
{
	FILE   *fil;
	char    line_buf[256];
	int     line = 0;
	struct sestsm ssm;
	struct slovo slv;

	fil = fopen (file_2, "rt");
	if (fil == NULL) {
		fprintf (stderr,
			 "Chyba: nepodarilo sa otvorit subor so slovami.\n");
		return 0;
	}
	if (create_kriz (file_1, &ssm)) {
	//	for (int j = 0; j < 15;j++) {
	//	for (int i = 0; i <=27; i++)
	//		printf("%s ", pism[get_pismeno(&ssm, i,j).l]);
	//	printf("\n");
	//	}
	//	return;

		while (!feof (fil)) {
			if (fgets (line_buf, 256, fil)) {
				line++;
				if (line_buf[strlen (line_buf) - 1] == '\n')
					line_buf[strlen (line_buf) - 1] = '\0';	/* odstran \n z konca riadka */
				if (line_buf[strlen (line_buf) - 1] == '\r')
					line_buf[strlen (line_buf) - 1] = '\0';	/* odstran \r z konca riadka */
				if (create_slovo (line_buf, &slv)) {
					if (!search_3 (&ssm, &slv, 0, 1))
						printf ("Nenaslo sa slovo na riadku %d.\n", line);
					free_slovo (&slv);
				}
			}
		}
		print_kriz (&ssm);
		free_kriz (&ssm);
		fclose (fil);
		return 1;
	}

	else {
		fprintf (stderr, "nepodarilo sa vytvorit krizovku.\n");
		return 0;
	}
}
char   *
strip_path (char *path)
{
	char   *pom;

	pom = strrchr (path, DIR_DELIM);
	if (!pom)
		return path;

	else
		return pom + 1;
}

void test_get_pos() 
{
	printf("id: 0\n");
	SESTSM testobr;
	
	testobr.siz_x = 4;
	testobr.siz_y = 4;
	testobr.id = 0;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4-(j%2)*(1-testobr.id); i++) {
			if (is_in_kriz(&testobr, i, j))
				printf("%d ", get_pos(&testobr, i, j));
		}
		printf("\n");
	}
	printf("id: 1\n");


	testobr.siz_x = 4;
	testobr.siz_y = 4;
	testobr.id = 1;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4 - ((j+1) % 2) * testobr.id; i++) {
			if (is_in_kriz(&testobr, i, j))
				printf("%d ", get_pos(&testobr, i, j));
		}
		printf("\n");
	}

}


//#include <direct.h>



int
main (int argc, char **argv)
{
	//char buffer[1024];
	//printf("%s\n", _getcwd(buffer, 1024));
	//printf("%s\n", argv[1]);
	//return 0;
	//test_get_pos();
	//return 0;
	//printf("%u\n", (int)sizeof(unsigned int));
	//return 0;
	//unsigned int test;
	//for (int i = 0; i < 64; i++) {
	//	test = (unsigned int)1 << i;
	//	printf("%llu\n", test);
	//}
	//printf("HH: %llu", 1 << 33);
	//return 0;

	//SESTSM testobr;
	//testobr.siz_x = 28;
	//testobr.siz_y = 15;
	//testobr.id = 1;

	//int nx, ny;
	//get_next_field(&testobr, 26, 7, 1, &nx, &ny);

	//printf("%d %d\n", nx, ny);

	//printf("%d\n", is_in_kriz(&testobr, 27, 7));
	//printf("%d\n", get_pos(&testobr, 27, 7));

	//return 0;
	if (argc < 3) {
		printf ("\nProgram lusti sestsmerovky v stvorcovej tabulke.\n");
		printf ("\nUsage: %s <sest_file> <slova_file>\n",
			strip_path (argv[0]));
		printf ("   <sest_file> - subor so sestsmerovkou\n");
		printf ("  <slova_file> - subor so slovami\n\n");
		return -1;
	}

	else
		lusti_kriz (argv[1], argv[2]);
	return 1;
}
