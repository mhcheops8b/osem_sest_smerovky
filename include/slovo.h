#ifndef _OSM_SLOVO_H_
#define _OSM_SLOVO_H_
typedef struct slovo
{
	int     count;
	struct pismeno *pism;
}
SLOVO;

/* Prototypes */
int     is_palindrom (struct slovo *slv);
int     create_slovo (char *poc, struct slovo *slv);
int     create_slovo_range (char *from, char *to, struct slovo *slv);
int 	copy_slovo(SLOVO *new_sl, SLOVO *old_sl); 
void    free_slovo (struct slovo *slv);
int     check_slovo (char *poc);
int     count_slovo (char *poc);
int     count_slovo_range (char *from, char *to);
void    print_slovo (SLOVO * slv);
void    fprint_slovo (SLOVO * slv, FILE * fil);
int     compare_wrd_to_wrd (SLOVO * slv1, SLOVO * slv2);
int     is_qword (SLOVO * wrd);

/* */
#endif /*  */
