#ifndef _OSM_NUM_PATH_H_
#define _OSM_NUM_PATH_H_

#define PATH_OK      0
#define PATH_ER     -1

#define IS_O_PATH 1
#define NOT_O_PATH 0

typedef struct num_path
{
	int     for_elems, cyc_elems, is_o_path;
	int    *for_smery_nums;
	int    *cyc_smery_nums;
}
NUM_PATH;

void    init_path_numpath (struct num_path *pth);
void    free_path_numpath (struct num_path *pth);
int     create_path_numpath (struct num_path *pth, int for_elems,
			     int cyc_elems);
int     create_path2_numpath (struct num_path *pth, int for_elems,
			      int cyc_elems, int is_o_path);
int     add_to_path_for_numpath (struct num_path *pth, int for_pos,
				 int dir_num);
int     add_to_path_cyc_numpath (struct num_path *pth, int cyc_pos,
				 int dir_num);
int     create_path_str_numpath (struct num_path *pth, int zac_dir,
				 char *prg);
int     create_path_str_ver2_numpath (struct num_path *pth, int zac_dir,
				      char *prg);
int     create_path_str2_numpath (struct num_path *pth, int zac_dir,
				  char *prg);
int     get_dir_num_numpath (struct num_path *pth, int dir_pos,
			     int *new_dir_num);

int		test_path(char* path);

/* to str_path */
int     parse_path_dir_str (char *path, int zac_dir, int cur_dir,
			    int i_allowed, int *dir_len, int *dir_val);
int     test_program_sections (char *prg);
int     get_length_path (char *path);
int     get_dir_val (char **prg, int zac_dir, int old_val, int *new_val);
void    print_path_numpath (struct num_path *pth);

int     rol_path (struct num_path *pth);
#endif /*  */
