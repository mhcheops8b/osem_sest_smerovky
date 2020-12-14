#include <stdio.h>
#include <string.h>
#include "str_path.h"

int
rol_path_str_ver2 (char *path, char *buffer)
{
	int     miss_dir = rol_miss_dir (path);
	char   *pom = path, *pom2 = buffer;

	if (*pom != 'O') {
		fprintf (stderr,
#ifdef LANG_EN
			 "Error: Could not rotate path, which is not `O` path.\n"
#else
			 "Chyba: Nemozno rolovat cestu, ktora nie je `O` cesta.\n"
#endif
			);

		return 0;
	}
	*pom2++ = *pom++;

	if (*pom != '0') {
		fprintf (stderr,
#ifdef LANG_EN
			 "Error: `O` path should start with 0.\n"
#else
			 "Chyba: O cesta musi zacinat hodnotou 0.\n"
#endif
			);
		return 0;
	}
	*pom2++ = *pom++;

	/*
	 * preskoc specifikaciu smeru - prvy bude vzdy 0
	 */

	if (*pom == 'M' || *pom == 'P')
		pom += 2;
	else
		pom++;

	while (*pom)
		*pom2++ = *pom++;	/* kopiruj zvysok retazca */

	if (miss_dir > 4) {
		if (miss_dir == 6)
			*pom2++ = '+';
		else {
			*pom2++ = 'P';
			*pom2++ = '0' + (8 - miss_dir);
		}
	}
	else {
		if (miss_dir == 0)
			*pom2++ = '0';
		else if (miss_dir == 2)
			*pom2++ = '-';
		else {
			*pom2++ = 'M';
			*pom2++ = '0' + miss_dir;
		}
	}

	*pom2 = '\0';
	return 1;
}

#define BAD_ARG         -2
#define NOT_SAME        -1
#define ARE_SAME         0

int
cmp_o_path_str (char *o_path1, char *o_path2)
{
	char   *pom1 = o_path1, *pom2 = o_path2;
	int     same, diff, val;

	if (*pom1 != 'O') {
		fprintf (stderr,
#ifdef LANG_EN
			 "Error: The first argument in function `cmp_o_path_str` is not `O` path.\n"
#else
			 "Chyba: prvy argument vo funkcii `cmp_o_path_str` nie je O cesta\n"
#endif
			);
		return BAD_ARG;
	}

	if (*pom2 != 'O') {
		fprintf (stderr,
#ifdef LANG_EN
			 "Error: The second argument in function `cmp_o_path_str` is not `O` path.\n"
#else
			 "Chyba: druhy argument vo funkcii `cmp_o_path_str` nie je O cesta\n"
#endif
			);
		return BAD_ARG;
	}

	pom1++;
	pom2++;

	while (*pom1 && *pom2) {

		switch (*pom1) {
		case '0':
			same = 0;
			diff = 1;

			if (*pom2 == '0')
				same = 1;
			else if (*pom2 == 'P' && *(pom2 + 1) == '0') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'M' && *(pom2 + 1) == '0') {
				diff = 2;
				same = 1;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;
		case '+':
			same = 0;
			diff = 1;

			if (*pom2 == '+')
				same = 1;
			else if (*pom2 == 'P' && *(pom2 + 1) == '2') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'M' && *(pom2 + 1) == '6') {
				same = 1;
				diff = 2;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;
		case '-':
			same = 0;
			diff = 1;

			if (*pom2 == '-')
				same = 1;
			else if (*pom2 == 'M' && *(pom2 + 1) == '2') {
				diff = 2;
				same = 1;
			}
			else if (*pom2 == 'P' && *(pom2 + 1) == '6') {
				same = 1;
				diff = 2;
			}

			if (!same)
				return NOT_SAME;

			pom1++;
			pom2 += diff;

			break;

		case 'P':
			same = 0;
			diff = 1;
			val = *(pom1 + 1);
			switch (val) {
			case '0':
				if (*pom2 == '0')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '0')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '0')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;
				break;
			case '2':
				if (*pom2 == '+')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '2')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			case '6':
				if (*pom2 == '-')
					same = 1;
				else if ((*pom2 == 'M' && *(pom2 + 1) == '2')
					 || (*pom2 == 'P'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			default:
				if ((*pom2 == 'P' && *(pom2 + 1) == val) ||
				    (*pom2 == 'M'
				     && *(pom2 + 1) ==
				     '0' + (8 - (val - '0')))) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;
			}
			break;
		case 'M':
			same = 0;
			diff = 1;
			val = *(pom1 + 1);
			switch (val) {
			case '0':
				if (*pom2 == '0')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '0')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '0')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;
				break;
			case '2':
				if (*pom2 == '-')
					same = 1;
				else if ((*pom2 == 'M' && *(pom2 + 1) == '2')
					 || (*pom2 == 'P'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			case '6':
				if (*pom2 == '+')
					same = 1;
				else if ((*pom2 == 'P' && *(pom2 + 1) == '2')
					 || (*pom2 == 'M'
					     && *(pom2 + 1) == '6')) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;

			default:
				if ((*pom2 == 'M' && *(pom2 + 1) == val) ||
				    (*pom2 == 'P'
				     && *(pom2 + 1) ==
				     '0' + (8 - (val - '0')))) {
					same = 1;
					diff = 2;
				}

				if (!same)
					return NOT_SAME;

				pom1 += 2;
				pom2 += diff;

				break;
			}
			break;

		}		/* end switch */

	}			/* end while */


	if (*pom1 == *pom2)
		return ARE_SAME;
	else
		return NOT_SAME;
}				/* end cmp_o_path_str */

int
rol_path_str (char *path, char *buffer)
{
	char    buf[3] = { '\0', '\0', '\0' }, *pom = path, *pom2 = buffer;
	int     i = 0;

	if (*pom != 'O') {
		fprintf (stderr,
#ifdef LANG_EN
			 "Error: Could not rotate path, which is not `O` path.\n"
#else
			 "Chyba: Nemozno rolovat cestu, ktora nie je `O` cesta.\n"
#endif
			);

		return 0;
	}
	buffer[0] = *pom;
	pom2 = &buffer[1];
	pom++;

	if (*pom == 'P' || *pom == 'M') {
		buf[0] = *pom;
		buf[1] = *(pom + 1);
		buf[2] = '\0';
		pom += 2;
	}
	else {
		buf[0] = *pom;
		buf[1] = '\0';
		pom++;
	}

	while (*pom) {
		*pom2 = *pom;
		pom++;
		pom2++;
		i++;
	}
	if (strlen (buf) < 2) {
		pom2[0] = buf[0];
		pom2[1] = '\0';
	}
	else {
		pom2[0] = buf[0];
		pom2[1] = buf[1];
		pom2[2] = '\0';
	}
	return 1;
}

int
rol_miss_dir (char *path)
{
	char   *pom = path;
	int     miss_dir = 0;

	pom++;			/* preskoc 'O' na zaciatku cesty */
	while (*pom) {
		switch (*pom) {
		case 'P':
			pom++;
			miss_dir += *pom - '0';
			break;
		case 'M':
			pom++;
			miss_dir -= *pom - '0';
			break;
		case '+':
			miss_dir += 2;
			break;
		case '-':
			miss_dir -= 2;
			break;
		}
		pom++;

	}

	while (miss_dir >= 8)
		miss_dir -= 8;
	while (miss_dir < 0)
		miss_dir += 8;

	return miss_dir;
}
