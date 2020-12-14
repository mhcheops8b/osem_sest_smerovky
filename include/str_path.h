#ifndef _OSM_STR_PATH_H_
#define _OSM_STR_PATH_H_

#define BAD_ARG         -2
#define NOT_SAME        -1
#define ARE_SAME         0

/* Prototypes  */
int     rol_path_str_ver2 (char *path, char *buffer);

int     cmp_o_path_str (char *o_path1, char *o_path2);

int     rol_path_str (char *path, char *buffer);

int     rol_miss_dir (char *path);
#endif
