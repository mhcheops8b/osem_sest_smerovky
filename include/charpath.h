#ifndef _OSM_CHAR_PATH_H_
#define _OSM_CHAR_PATH_H_
typedef struct char_path
{
	char   *elem;
	struct char_path *next;
}
CHAR_PATHS;
/*extern struct char_path *default_char_path;
extern struct char_path *vlnka_char_path;*/
int     create_char_path (struct char_path **pth, char *elem);
int     add_char_path (struct char_path **pth, char *elem);
void    free_char_path (struct char_path *pth);
int     create_char_path_chrs (struct char_path **pth, int count, char **ps);
int     init_char_path_defaults (void);

#endif /*  */
