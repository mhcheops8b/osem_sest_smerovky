#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "specchar.h"

void
init_specchar_list (struct spec_char *list)
{
	list->x = 0;
	list->y = 0;
	list->chr = 0;
	list->next = NULL;
}

int
create_specchar_list (struct spec_char **list, int x, int y, int chr)
{
	*list = (struct spec_char *) malloc (sizeof (struct spec_char));
	if (*list == NULL) {
		fprintf (stderr,
#ifdef LANG_EN
  				"Error: Could not allocate memory (`create_specchar_list`).\n"
#else
  				"Chyba: Nepodarilo sa alokovat pamat (`create_specchar_list`).\n"
#endif
     );
		return -1;
	}

	init_specchar_list (*list);
	(*list)->x = x;
	(*list)->y = y;
	(*list)->chr = chr;
	return 0;
}

int
add_specchar_list (struct spec_char **list, int x, int y, int chr)
{
	struct spec_char *pom;

	if (*list == NULL)
		return create_specchar_list (list, x, y, chr);
	if (is_in_specchar_list_pos (*list, x, y)) {
		fprintf (stderr,
#ifdef LANG_EN
				"Warning: Adding position [%2d,%2d](%2d) is already in list.\n"
#else
			 "Upozornenie: Pridavana pozicia [%2d,%2d](%2d) uz je v zozname.\n"
#endif
			 ,x, y, get_char_specchar_list_pos (*list, x, y));

		fprintf (stderr, 
#ifdef LANG_EN
				"Not added, ignoring it.\n"
#else				
				"Nie je pridana, ignoruje sa.\n"
#endif
			);
		return 0;
	}
	pom = NULL;
	if (create_specchar_list (&pom, x, y, chr) < 0)
		return -1;
	pom->next = *list;
	*list = pom;
	return 0;
}

int
is_in_specchar_list_pos (struct spec_char *list, int x, int y)
{
	struct spec_char *pom = list;

	while (pom) {
		if (pom->x == x && pom->y == y)
			return 1;
		pom = pom->next;
	}
	return 0;
}

int
get_char_specchar_list_pos (struct spec_char *list, int x, int y)
{
	struct spec_char *pom = list;

	while (pom) {
		if (pom->x == x && pom->y == y)
			return pom->chr;
		pom = pom->next;
	}
	return 0;
}

void
free_specchar_list (struct spec_char **list)
{
	struct spec_char *pom = *list;

	while (pom) {
		*list = pom->next;
		free ((void *) pom);
		pom = *list;
	}
}
void
debug_print_specchar_list_elem (struct spec_char *spc_list)
{
	printf ("[%2d,%2d] - %3d - %8p\n", spc_list->x, spc_list->y,
		spc_list->chr, (void *)spc_list->next);
}

void
debug_print_specchar_list (struct spec_char *spc_list)
{
	while (spc_list) {
		debug_print_specchar_list_elem (spc_list);
		spc_list = spc_list->next;
	}
}


/*
int main() {
    struct spec_char *spc,*pom;

    spc = NULL;

    add_specchar_list(&spc,1,1,44);
    add_specchar_list(&spc,1,4,7);
    add_specchar_list(&spc,13,1,8);
    add_specchar_list(&spc,5,1,11);    
    add_specchar_list(&spc,1,15,12);
    add_specchar_list(&spc,1,15,45);
    
    debug_print_specchar_list(spc);

    if (is_in_specchar_list_pos(spc,13,1)) 
        printf("Je.\n");
    else printf("Nie je.\n");

    
    free_specchar_list(&spc);    
    add_specchar_list(&spc,2,2,2);
    debug_print_specchar_list(spc);
    free_specchar_list(&spc);    
    return 0;  
}
*/
