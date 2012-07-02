#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char **argv)
{
  if (argc <= 1)
    {
      fprintf (stderr, "file name not specified\n");
      exit (EXIT_FAILURE);
    }
  FILE * fp;
  fp = fopen (argv[1], "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Unable to open file\n");
      exit (EXIT_FAILURE);
    }

  int done = 0;
  char * line = NULL;
  size_t  length;
  int ret;
  int blank = 1;
  while (done == 0)
    {
      ret = getline (&line, &length, fp);
      if ( ret == -1 )
	{
	  fprintf (stderr, "getline failed\n");
	  exit (EXIT_FAILURE);
	}
      int i = 0;
      for (; i < ret; ++i)
	{
	  if (line[i] == ' ' || line[i] == '\r' || line[i] == '\n')
	    {
	      continue;
	    }
	  else
	    {
	      blank = 0;
	      break;
	    }
	}
      if (blank == 1) {
	printf ("Blank line\n");
      }
      blank = 1;
    }
  exit (EXIT_SUCCESS);
}
