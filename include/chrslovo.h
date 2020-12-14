#ifndef _OSM_CHRSLOVO_H_
#define _OSM_CHRSLOVO_H_

typedef struct chr_slovo_list
{
	char   *slovo;
	struct chr_slovo_list *next;
}
CHR_SLOVO_LIST;

/* Prototypes */
int     add_chr_slovo_list (struct chr_slovo_list **list, char *slovo);

void    free_chr_slovo_list (struct chr_slovo_list **list);

int     remove_chr_slovo_from_list (struct chr_slovo_list **list,
				    struct chr_slovo_list *rm);
#endif
