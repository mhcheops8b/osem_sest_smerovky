#ifndef _OSM_OBRFILE_H_
#define _OSM_OBRFILE_H_

#include "flosm.h"
#include "flosmsyl.h"

#define LINE_BUFFER_SIZE 256
#define BAD_SIZE_FMT 	-1
#define BAD_SIZE_ARG 	-2
#define OK_SIZE 	0
#define BAD_DATA_CHRS		-1
#define BAD_DATA_LINE_CNT 	-2
#define BAD_DATA		-3
#define BAD_DATA_COUNT 		-4
#define OK_DATA			 0
#define MASK_MODE 	1
#define ID_MODE  	0
#define YES 	1
#define NO	0

/* prototypes */
void    strip_nl (char *line);
char   *skip_ws (char *line);

int     read_size_osm (FILE * fil, int *line, int *size_x, int *size_y);

/* The grammatic shapes of SK word */

/* letter*/
extern const char *pismeno_skl[];

/* expected */
extern const char *ocakavany_skl[];

const char *sklonuj (char *zaklad, int count);

int     check_line_size (int size_x, char *filename, int cnt, int tot_cnt,
			 int tot_interval, int line, int tot_line_beg,
			 int tot_line_end);

int     read_data_osm (FILE * fil, int *line, char *filename,
		       struct osemsm *osm);

int     read_syllabe_data_osm (FILE * fil, int *line, char *filename,
			       struct osemsm_syl *osm,
			       char syllabe_delimiter);

int     create_kriz (char *filename, struct osemsm *osm);

int     read_s_section (struct osemsm *osm, FILE * fil, char *buffer);

int     read_m_section (struct osemsm *osm, FILE * fil, char *line_buf,
			int *line);

int     read_i_section (struct osemsm *osm, FILE * fil, char *line_buf,
			int *line);

int     test_osm_obr (char *obr_file);

int     create_kriz_syl (char *filename, OSEMSM_SYL * osm);
#endif
