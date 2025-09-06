#include "pismeno.h"
const char *pism[] =
	{ "", "A", ",A", ":A", "B", "C", "vC", "D", "vD", "`DZ", "`DvZ", "E",
	",E", "F", "G", "H", "`CH", "I", ",I", "J", "K", "L", ",L", "vL", "M",
	"N",
	"vN", "O", ",O",
	"vO", "P", "Q", "R", ",R", "S", "vS", "T", "vT", "U", ",U", "V", "W",
	"X",
	"Y", ",Y", "Z",
	"vZ", "?", "-", ":U", "`?", "vR", "vE", "!", "`.", ":E"
};
const char *
get_pism (int num)
{
	return pism[num];
}

const char*
get_pism_vis(int num)
{
	if (num)
		return pism[num];
	else
		return ".";
}


struct pismeno
get_table (struct table *tbl, char key)
{
	int     i;
	struct pismeno pis = { 0, 0 };

	for (i = 0; i < tbl->pocet; i++) {
		if (tbl->hodn[i].key == key) {
			pis.l = (char) tbl->hodn[i].value;
			return pis;
		}
	}
	return pis;
}

int
compare_pism_to_pism (Pismeno * pism1, Pismeno * pism2)
{
	return (pism1->l == pism2->l);
}

int
compare_pism_to_pism_with_quest (struct pismeno *p1, struct pismeno *p2)
{
	return (p1->l == p2->l) || (p1->l == QUEST_CHAR)
		|| (p2->l == QUEST_CHAR);
}
