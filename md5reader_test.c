/**
   test file for md5 class
*/

#include "md5reader.h"
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
  md5mesh *mymesh;
  mymesh = md5mesh_loadfile (md5mesh_file);
  printf ("md5mesh.fileVersion: %d\n", mymesh->fileVersion);
  printf ("md5mesh.numJoints:   %d\n", mymesh->numJoints);
  printf ("md5mesh.numMeshes:   %d\n", mymesh->numMeshes);

  return 0;
}
