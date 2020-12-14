#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _alpha_parse_element
{
  char selector_character;
  int length_of_selected_groups;
  char **selected_group;
};

struct _alpha_onelength_element
{
  char selector_character;
  struct character_list *element_list;
};

struct character_list
{
  char character;
  struct internal_code *internal_code;
  struct character_list *next_character;
};

int
create_character_list (struct character_list **p_p_character_list, char ch,
		       int int_code, int letter_type)
{
  /* *p_p_character_list allocate memory */

}




/* -------------------------- Internal code structure -------------------- */
struct internal_code
{
  int internal_code;
  int letter_type;
};


void
init_internal_code (struct internal_code *int_cod_struct, int int_code,
		    int letter_type)
{
  int_cod_struct->internal_code = int_code;
  int_cod_struct->letter_type = letter_type;
}

void
default_internal_code (struct internal_code *int_cod_struct)
{
  init_internal_code (int_cod_struct, 0, 0);
}

int
create_internal_code (struct internal_code **p_p_int_code_struct, int code,
		      int letter_type)
{
  if (*p_p_int_code_struct == NULL)
    {
      *p_p_int_code_struct =
	(struct internal_code *) malloc (sizeof (struct internal_code));
      if (*p_p_int_code_struct == NULL)
	{
	  print_err_msg (-1);
	  return -1;
	}
    }
  init_internal_code (*p_p_int_code_struct, int_code, int_letter_type);
  return 0;
}

void
free_internal_code (struct internal_code **p_p_int_code_struct)
{
  if (*p_p_int_code_struct != NULL)
    {
      free ((void *) *p_p_int_code_struct);
      *p_p_int_code_struct = NULL;
    }
}

int
create_character_list (struct character_list *ch_list, char ch, int int_code,
		       int letter_type)
{

}

/* --------------------------- -End of internal code struct ---------------- */

int
main ()
{
  return 0;
}
