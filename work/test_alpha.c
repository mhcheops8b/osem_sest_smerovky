#include <stdio.h>
#include "alpha.h"
int
main ()
{
  char *uu = "POK,U`CH,AvO`DZ`DvZDDZ", *pom;
  int code, len;

  pom = uu;
  printf ("%s\n", uu);
  while (*pom)
    {
      if (parse_letter (pom, &code, &len) < 0)
	{
	  return 1;
	}
      printf ("Code: %2d, Len: %d, Str_Letter: %s\n", code, len,
	      get_str_letter (code, ALPHA_DOS852));
      pom += len;
    }
  for (len = 0; len < 48; len++)
    {
      printf ("%s,", get_str_letter (len, ALPHA_DOS852));
    }
  return 0;
}
