#ifndef _OSM_CESTY_H_
#define _OSM_CESTY_H_

typedef struct cesty
{
	int     count;
	char  **elems;
}
CESTY;

#define PRG_BUFFER_SIZE 128

extern char *default_cesta_elems[];
extern char *vlnka_cesta_elems[];
extern char *obdlznik_cesta_elems[];

extern struct cesty default_cesta;
extern struct cesty vlnka_cesta;
extern struct cesty obdlznik_cesta;

int     cesta_is_allowed (char *allowed, char c);

int     test_cesta (char *start, char *end);

int     count_cesta (char *elem);

int     create_cesty (struct cesty *cst, char *elems);

void    free_cesty (struct cesty *cst);
#endif
