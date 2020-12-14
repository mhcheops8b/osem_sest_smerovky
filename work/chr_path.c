#include <stdio.h>
#include "smery.h"

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

#define PATH_OK      0
#define PATH_ER     -1

int
parse_path_dir_str (char *path, int zac_dir, int cur_dir, int i_allowed,
		    int *dir_len, int *dir_val)
{

  char *pom_path = path;
  int pom_dir_val = cur_dir, pom_val;

  switch (*pom_path)
    {
    case '0':
      *dir_len = 1;
      *dir_val = cur_dir;
      return PATH_OK;
    case 'I':
      if (i_allowed)
	{
	  *dir_len = 1;
	  *dir_val = zac_dir;
	  return PATH_OK;
	}

      fprintf (stderr, "`I` could not be used in path specification.\n");
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

      if (*pom_path < '0' || *pom_path > '7')
	{
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

      if (*pom_path < '0' || *pom_path > '7')
	{
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
    }				/* end switch */
}

int
get_dir_val (char **prg, int zac_dir, int old_val, int *new_val)
{

  char *pom = *prg;
  int dir_val = old_val;

  switch (*pom)
    {
    case 'P':
      pom++;
      if (*pom < '0' || *pom > '9')
	{
	  fprintf (stderr,
		   "Chyba: V specifikacii cesty. Za P(M) sa ocakava cislo od 0-9.\n");
	  return 0;
	}
      adjust_pos_dirno (&dir_val, *pom - '0');
      break;
    case 'M':
      pom++;
      if (*pom < '0' || *pom > '9')
	{
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
      fprintf (stderr, "Chyba: retazec obsahuje nepovoleny znak.\n");
      return 0;
      break;
    }

  *prg = pom;
  *new_val = dir_val;
  return 1;
}

int
test_program_sections (char *prg)
{
  int c_count, f_count, o_count;
  char *pom = prg;

  c_count = f_count = o_count = 0;

  while (*pom)
    {
      if (*pom == 'C')
	c_count++;
      if (*pom == 'F')
	f_count++;
      if (*pom == 'O')
	o_count++;
      pom++;
    }
  if (c_count > 1 || f_count > 1 || o_count > 1)
    {
      fprintf (stderr,
	       "Chyba: C, F , O sa mozu v programe nachadzat nanajvys raz.\n");
      return 0;
    }
  if (o_count && (c_count || f_count))
    {
      fprintf (stderr,
	       "Chyba: Pri specifikatore O sa nemozu nachadzat dalsie ine.\n");
      return 0;
    }
  return 1;
}

int
get_length_path (char *path)
{

  char *pom = path;
  int pocet = 0;

  if (pom == NULL)
    return 0;

  if (*pom == 'C' || *pom == 'F' || *pom == 'O')
    pom++;
  else
    return 0;

  while (*pom && *pom != 'C' && *pom != 'F' && *pom != 'O')
    {

      switch (*pom)
	{
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
