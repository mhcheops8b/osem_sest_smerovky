#ifndef _OSM_TABLE_H_
#define _OSM_TABLE_H_

#define EMPTY_LETTER     0
#define QUEST_LETTER    47

#define CH_LETTER   16
#define DZ_LETTER    9
#define DZZ_LETTER  10
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

/*ELEM all_elem[], dlz_elem[], mak_elem[], bod_elem[];*/
struct table all_table, dlz_table, mak_table, bod_table;
int     is_in_table (struct table *tbl, char key);
int     check_char (char **poc, struct table *tbl, char *msg_1, char *msg_2);
int     check_char_2 (char **poc);
int     parse_letter (char *input, int *int_code, int *let_len);

#endif /*  */
