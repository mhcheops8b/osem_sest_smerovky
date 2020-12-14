#include <stdio.h>
#include <stdlib.h>

#define     O_PATH 1
#define NOT_O_PATH 0


typedef struct num_path
{
  int for_elems, cyc_elems, is_o_path;
  int *for_smery_nums;
  int *cyc_smery_nums;
}
NUM_PATH;

void
init_numpath (struct num_path *pth)
{
  pth->for_elems = 0;
  pth->cyc_elems = 0;
  pth->is_o_path = NOT_O_PATH;
  pth->for_smery_nums = NULL;
  pth->cyc_smery_nums = NULL;
}

void
free_numpath (struct num_path *pth)
{

  pth->is_o_path = NOT_O_PATH;
  pth->for_elems = 0;
  pth->cyc_elems = 0;

  if (pth->for_smery_nums)
    {
      free ((void *) pth->for_smery_nums);
      pth->for_smery_nums = NULL;
    }

  if (pth->cyc_smery_nums)
    {
      free ((void *) pth->cyc_smery_nums);
      pth->cyc_smery_nums = NULL;
    }
}

int
create_numpath_FC (struct num_path *pth, int for_elems, int cyc_elems)
{

  if (for_elems)
    {
      pth->for_smery_nums = (int *) malloc (for_elems * sizeof (int));

      if (pth->for_smery_nums == NULL)
	{
	  fprintf (stderr,
		   "Chyba: Nedostatok pamate pri vytvarani pola for_smerov.\n");
	  init_numpath (pth);
	  return 0;
	}
    }
  else
    pth->for_smery_nums = NULL;

  if (cyc_elems)
    {
      pth->cyc_smery_nums = (int *) malloc (cyc_elems * sizeof (int));

      if (pth->cyc_smery_nums == NULL)
	{

	  fprintf (stderr,
		   "Chyba: Nedostatok pamate pri vytvarani pola cyc_smerov.\n");

	  if (for_elems)
	    free_numpath (pth);
	  else
	    init_numpath (pth);

	  return 0;
	}
    }
  else
    pth->cyc_smery_nums = NULL;

  if (for_elems || cyc_elems)
    {
      pth->for_elems = for_elems;
      pth->cyc_elems = cyc_elems;
      return 1;
    }
  else
    {
      fprintf (stderr, "Chyba: Vytvara sa cesta nulovej dlzky.\n");
      init_numpath (pth);
      return 0;
    }

  return 1;
}

int
create_numpath_FCO (struct num_path *pth, int for_elems, int cyc_elems,
		    int is_o_path)
{

  if (is_o_path == O_PATH)
    {
      if (cyc_elems != 0)
	{
	  fprintf (stderr, "O cesta nesmie obsahovat opakovane casti.\n");
	  init_numpath (pth);
	  return 0;
	}

    }

  if (!create_numpath_FC (pth, for_elems, cyc_elems))
    return 0;

  pth->is_o_path = is_o_path;
  return 1;
}

int
add_to_numpath_for (struct num_path *pth, int for_pos, int dir_num)
{

  if (pth->for_elems <= for_pos)
    {
      fprintf (stderr,
	       "Chyba: Nemozno pridat na poziciu %d, pretoze cesta ma %d prvkov.\n",
	       for_pos, pth->for_elems);
      return 0;
    }

  pth->for_smery_nums[for_pos] = dir_num;
  return 1;
}

int
add_to_numpath_cyc (struct num_path *pth, int cyc_pos, int dir_num)
{

  if (pth->cyc_elems <= cyc_pos)
    {
      fprintf (stderr,
	       "Chyba: Nemozno pridat na poziciu %d, pretoze cesta ma %d prvkov.\n",
	       cyc_pos, pth->cyc_elems);
      return 0;
    }

  pth->cyc_smery_nums[cyc_pos] = dir_num;
  return 1;
}
