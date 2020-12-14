#ifndef _OSM_RIES_H_
#define _OSM_RIES_H_

typedef struct osm_ries
{
	int     pos_x, pos_y, dir_num, cnt_question_mark;
	char   *chr_path;
	struct osm_ries *next;
}
OSM_RIES;

#define OSM_RIES_OK 1
#define OSM_RIES_ERR 0

/* Prototypes */

int     create_osm_ries_list (struct osm_ries *ries,
			      int pos_x, int pos_y, int dir_num,
			      int cnt_quest, char *path);

int     add_osm_ries_list (struct osm_ries **ries,
			   int pos_x, int pos_y, int dir_num, int cnt_quest,
			   char *path);

void    free_osm_ries (struct osm_ries *list);

int     has_quest_ries (struct osm_ries *ries);

void    count_quests (struct osm_ries *ries, int *has_q, int *non_q);

int     get_size_of_osm_ries (struct osm_ries *ries);

int     get_min_ries_count (struct osm_ries *ries);

int     test_riesenie (struct osm_ries *ries, int test_count);

/* End prototypes */

#endif
