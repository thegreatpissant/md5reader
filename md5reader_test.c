/**
   test file for md5 class
*/

#include "md5reader.h"
#include "skeleton.h"
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char **argv)
{

  if (argc <= 1)
  {
      fprintf (stderr, "File name not specified\n");
      exit (EXIT_FAILURE);
  }
  FILE * md5mesh_file;
  md5mesh_file = fopen (argv[1], "r");
  if (md5mesh_file == NULL)
    {
      fprintf (stderr, "Error opening file\n");
      exit (EXIT_FAILURE);
    }
  pskeleton mySkeleton;
  mySkeleton = md5mesh_loadfile (md5mesh_file);
  printSkeleton (mySkeleton);
  return 0;
}
