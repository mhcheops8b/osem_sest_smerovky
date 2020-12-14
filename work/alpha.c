#include <stdio.h>
#include <string.h>

#include "alpha.h"
const char *alphabet_ascii[] =
  { "", "A", ",A", ":A", "B", "C", "vC", "D", "vD", "`DZ", "`DvZ", "E",
  ",E", "F", "G", "H", "`CH", "I", ",I", "J", "K", "L", ",L", "vL", "M", "N",
  "vN", "O", ",O", "vO", "P",
  "Q", "R", ",R", "S", "vS", "T", "vT", "U", ",U", "V", "W", "X", "Y", ",Y",
  "Z", "vZ", "?"
};
const char *alphabet_dos852[] =
  { "", "A", "\265", "\216", "B", "C", "\254", "D", "\322", "DZ", "D\246",
  "E", "\220", "F", "G", "H", "CH", "I", "\326", "J", "K", "L", "\221",
  "\225", "M", "N", "\325", "O",
  "\340", "\342", "P", "Q", "R", "\350", "S", "\346", "T", "\233", "U",
  "\351", "V", "W", "X", "Y",
  "\355", "Z", "\246", "?"
};
const char *alphabet_w1250[] =
  { "", "A", "\301", "\304", "B", "C", "\310", "D", "\317", "DZ", "D\216",
  "E", "\311", "F", "G", "H", "CH", "I", "\315", "J", "K", "L", "\305",
  "\274", "M", "N", "\322", "O",
  "\323", "\324", "P", "Q", "R", "\300", "S", "\212", "T", "\215", "U",
  "\332", "V", "W", "X", "Y",
  "\335", "Z", "\216", "?"
};
const char *alphabet_i8859_2[] =
  { "", "A", "\301", "\304", "B", "C", "\310", "D", "\317", "DZ", "D\256",
  "E", "\311", "F", "G", "H", "CH", "I", "\315", "J", "K", "L", "\305",
  "\245", "M", "N", "\322", "O",
  "\323", "\324", "P", "Q", "R", "\300", "S", "\251", "T", "\253", "U",
  "\332", "V", "W", "X", "Y", "\335",
  "Z", "\256", "?"
};
ELEM all_elem[] = {

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
ELEM dlz_elem[] = {

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
ELEM mak_elem[] = {

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
ELEM bod_elem[] = {
  {
   'A', 3}
};
struct table bod_table = { 1, bod_elem };

/* Checks whether <key> is in table <tbl> */
int
is_in_table (TABLE * tbl, char key)
{
  int i;

  for (i = 0; i < tbl->pocet; i++)
    if (tbl->hodn[i].key == key)
      return 1;
  return 0;
}

int
get_code_from_table (TABLE * tbl, char key, int *code)
{
  int i;

  for (i = 0; i < tbl->pocet; i++)
    if (tbl->hodn[i].key == key)
      {
	*code = tbl->hodn[i].value;
	return 1;
      }
  return 0;
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
  char *pom = input;

  switch (*input)
    {
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
      if (!get_code_from_table (&dlz_table, *pom, int_code))
	{
	  fprintf (stderr,
		   "Chyba: Za `,` musi nasledovat pismeno, ktore ma dlzen.\n");
	  return -1;
	}
      *let_len = 2;
      break;
    case 'v':
      pom++;
      if (!get_code_from_table (&mak_table, *pom, int_code))
	{
	  fprintf (stderr,
		   "Chyba: Za `v` musi nasledovat pismeno, ktore ma makcen.\n");
	  return -1;
	}
      *let_len = 2;
      break;
    case '`':
      pom++;
      if (!strncmp (pom, "CH", 2))
	{
	  *int_code = CH_LETTER;
	  *let_len = 3;
	}

      else if (!strncmp (pom, "DZ", 2))
	{
	  *int_code = DZ_LETTER;
	  *let_len = 3;
	}

      else if (!strncmp (pom, "DvZ", 3))
	{
	  *int_code = DZZ_LETTER;
	  *let_len = 4;
	}

      else
	{
	  fprintf (stderr, "Chyba: Za ` musi nasledovat CH, DZ alebo DvZ.\n");
	  return -1;
	}
      break;
    case ':':
      pom++;
      if (!get_code_from_table (&bod_table, *pom, int_code))
	{
	  fprintf (stderr, "Chyba: Za `:` musi nasledovat pismeno A.\n");
	  return -1;
	}
      *let_len = 2;
      break;
    default:
      if (*input >= 'A' && *input <= 'Z')
	{			/* ASCII */
	  get_code_from_table (&all_table, *input, int_code);
	  *let_len = 1;
	}

      else
	{
	  if (*pom > 32)
	    fprintf (stderr, "Chyba: Neznamy znak (%c) na vstupe.\n", *pom);

	  else
	    fprintf (stderr, "Chyba: Neznamy znak (%02xh) na vstupe.\n",
		     *pom);
	  return -1;
	}
    }
  return 0;
}
const char *
get_str_letter (int int_code, int alphabet)
{
  switch (alphabet)
    {
    case ALPHA_ASCII:
      return alphabet_ascii[int_code];
    case ALPHA_W1250:
      return alphabet_w1250[int_code];
    case ALPHA_I88592:
      return alphabet_i8859_2[int_code];
    case ALPHA_DOS852:
      return alphabet_dos852[int_code];
    default:
      fprintf (stderr, "Neznamy kod abecedy.\n" "   Pouzijem 1 (ASCII).\n");
      return alphabet_ascii[int_code];
    }
}
