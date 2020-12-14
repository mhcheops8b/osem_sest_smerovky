#ifndef _OSEM_SPECCHAR_H_
#define _OSEM_SPECCHAR_H_
typedef struct spec_char
{
	int     x, y, 	/* position */
       		chr;	/* internal code of character */
	struct spec_char *next;	/* pointer to next record - linked list */
}
SPEC_CHAR;

void    init_specchar_list (struct spec_char *list);
int     create_specchar_list (struct spec_char **list, int x, int y, int chr);
int     add_specchar_list (struct spec_char **list, int x, int y, int chr);
int     is_in_specchar_list_pos (struct spec_char *list, int x, int y);
int     get_char_specchar_list_pos (struct spec_char *list, int x, int y);
void    free_specchar_list (struct spec_char **list);
void    debug_print_specchar_list_elem (struct spec_char *spc_list);
void    debug_print_specchar_list (struct spec_char *spc_list);

#endif /*  */
