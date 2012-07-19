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
  pskeleton mySkeleton;
  mySkeleton = md5mesh_loadfile ( argv[1] );

  printSkeleton (mySkeleton);
  return 0;
}
