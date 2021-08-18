#include <stdio.h>
#include <stdlib.h>
#include "slovo.h"

typedef struct {
	int pos_x, pos_y, init_dir;
	char *osm_path;
} Forced_solution;

typedef struct {
	int ulx,uly, brx, bry;
} Range;

typedef struct _range_list {
	Range *rng;
	struct _range_list *next;
} RangeList;

typedef struct _wordlist {
	SLOVO *slv;

	unsigned int is_link:1;
	int is_forced_solution:1;
	int is_range_list:1;
	Forced_solution *fsolution;
	int local_forb_limit;
	int number_of_to_solve;
	RangeList *rnglist;
	struct _wordlist *next_slv;
} Wrdlist;

int create_Wrdlist_copy(Wrdlist **wrdlist, SLOVO *slv) {
	*wrdlist=(Wrdlist *)malloc(sizeof(Wrdlist));

	if (*wrdlist==NULL) {
		fprintf(stderr, "Error: Cannot allocate memory(create_Wrdlist.wrdlist).\n");
		return -1;
	}

	(*wrdlist)->slv = (SLOVO *)malloc(sizeof(SLOVO));
	if ((*wrdlist)->slv==NULL) {
		fprintf(stderr, "Error: Could not allocate memory(create_Wrdlist.wrdlist.slv).\n");
		free((void *)*wrdlist);
		return -1;
	}
	if (copy_slovo((*wrdlist)->slv, slv)<0) {
		fprintf(stderr, "Error: Word `");
		print_slovo(slv);
		fprintf(stderr, "` cannot be stored.\n");
		free((void *)(*wrdlist)->slv);
		free((void *)*wrdlist);
		return -1;
	}

	(*wrdlist)->next_slv=NULL;
	(*wrdlist)->is_link=0;
	return 0;
}

int create_Wrdlist_link(Wrdlist **wrdlist, SLOVO *slv) {
	*wrdlist=(Wrdlist *)malloc(sizeof(Wrdlist));

	if (*wrdlist==NULL) {
		fprintf(stderr, "Error: Cannot allocate memory(create_Wrdlist.wrdlist).\n");
		return -1;
	}

	(*wrdlist)->slv = slv;
	(*wrdlist)->next_slv=NULL;
	(*wrdlist)->is_link=1;
	return 0;
}

int add_word_Wrdlist_copy(Wrdlist **wrdlist, SLOVO *slv) {
	Wrdlist *pom_wrdlist;

	if (*wrdlist==NULL) {
		if (create_Wrdlist_copy(wrdlist, slv)<0) {
			fprintf(stderr, "Cannot add word to the list(copy).\n");
			return -1;
		}
		return 0;
	}
	pom_wrdlist=*wrdlist;

	while (pom_wrdlist->next_slv) 
		pom_wrdlist=pom_wrdlist->next_slv;

	if (create_Wrdlist_copy(&pom_wrdlist->next_slv, slv)<0) {
			fprintf(stderr, "Cannot add word to the list(copy).\n");
			return -1;
	}

	return 0;
}

int add_word_Wrdlist_link(Wrdlist **wrdlist, SLOVO *slv) {
	Wrdlist *pom_wrdlist;

	if (*wrdlist==NULL) {
		if (create_Wrdlist_link(wrdlist, slv)<0) {
			fprintf(stderr, "Cannot add word to the list(link).\n");
			return -1;
		}
		return 0;
	}
	pom_wrdlist=*wrdlist;

	while (pom_wrdlist->next_slv) 
		pom_wrdlist=pom_wrdlist->next_slv;

	if (create_Wrdlist_link(&pom_wrdlist->next_slv, slv)<0) {
			fprintf(stderr, "Cannot add word to the list(link).\n");
			return -1;
	}
	return 0;
}


Wrdlist *find_word_Wrdlist(Wrdlist *wrdlist, SLOVO *slv) {
	Wrdlist *pom_wrdlist=wrdlist;

	while (pom_wrdlist) {
		if (compare_wrd_to_wrd(pom_wrdlist->slv, slv))
			return pom_wrdlist;
		pom_wrdlist=pom_wrdlist->next_slv;
	}
	return NULL;
}

void clear_slovo_Wrdlist(Wrdlist *wrdlist) {
	if (wrdlist) {
		if (!wrdlist->is_link) {
			if (wrdlist->slv) {
				free_slovo(wrdlist->slv);
				free((void *) wrdlist->slv);
				wrdlist->slv=NULL;
			}
		}
		else wrdlist->is_link=0;
	}
}

void del_word_Wrdlist(Wrdlist **wrdlist, SLOVO *slv) {
	Wrdlist *pom_this_wrdlist, *pom_wrdlist;

	pom_this_wrdlist=find_word_Wrdlist(*wrdlist, slv);
	if (pom_this_wrdlist==NULL) {
		fprintf(stderr, "Warning: Trying to remove non-existing word from list.\n");
		return;
	}

	if (pom_this_wrdlist==*wrdlist) { /* Head of the list */
		*wrdlist=pom_this_wrdlist->next_slv;
		clear_slovo_Wrdlist(pom_this_wrdlist);
		free((void *)pom_this_wrdlist);	
		return;
	}
	pom_wrdlist=*wrdlist;

	while (pom_wrdlist->next_slv!=pom_this_wrdlist) 
		pom_wrdlist=pom_wrdlist->next_slv;
	
	pom_wrdlist->next_slv=pom_this_wrdlist->next_slv;
	
	clear_slovo_Wrdlist(pom_this_wrdlist);
	free((void *)pom_this_wrdlist);	
}

void del_item_Wrdlist(Wrdlist **wrdlist, Wrdlist *item_wrdlist) {
	Wrdlist *pom_wrdlist;

	if (item_wrdlist==*wrdlist) { /* Head of the list */
		*wrdlist=item_wrdlist->next_slv;
	}
	else {
		pom_wrdlist=*wrdlist;

		while (pom_wrdlist->next_slv!=item_wrdlist) 
			pom_wrdlist=pom_wrdlist->next_slv;
	
		pom_wrdlist->next_slv=item_wrdlist->next_slv;
	
	}
	clear_slovo_Wrdlist(item_wrdlist);
	free((void *)item_wrdlist);	
	return;
}

void free_Wrdlist(Wrdlist **wrdlist) {
	while (*wrdlist) {
		del_item_Wrdlist(wrdlist, *wrdlist);
	}
}
