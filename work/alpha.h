#ifndef _OSEMSEST_ALPHA_H_
#define _OSEMSEST_ALPHA_H_

#define EMPTY_LETTER     0
#define QUEST_LETTER    47

#define CH_LETTER   16
#define DZ_LETTER    9
#define DZZ_LETTER  10

#define ALPHA_ASCII     1
#define ALPHA_W1250     2
#define ALPHA_I88592    3
#define ALPHA_DOS852    4

/*
extern const char *alphabet_ascii[];
extern const char *alphabet_dos852[];
extern const char *alphabet_w1250[];
extern const char *alphabet_i8859_2[];
*/
typedef struct elem
{
  char key;
  int value;
}
ELEM;
typedef struct table
{
  int pocet;
  struct elem *hodn;
}
TABLE;

/*
extern TABLE    all_table;
extern TABLE    dlz_table;
extern TABLE    mak_table;
extern TABLE    bod_table;
*/
int is_in_table (TABLE * tbl, char key);
int get_code_from_table (TABLE * tbl, char key, int *code);
int parse_letter (char *input, int *int_code, int *let_len);
const char *get_str_letter (int int_code, int alphabet);

#endif
