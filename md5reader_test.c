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
 
  printf ("Meshfile\n");
  pmd5meshfile meshfile;
  meshfile = md5meshfile_open (argv[1]);
  printf ("\tfilename \"%s\"\n", meshfile->filename);
 
  printf ("Meshfile info\n");
  pmd5info meshfileinfo;
  meshfileinfo = md5meshfile_loadinfo (meshfile);
  printf ("\tfile version %d\n", meshfileinfo->fileVersion);
  printf ("\tnum joints %d\n", meshfileinfo->numJoints);
  printf ("\tnum meshes %d\n", meshfileinfo->numMeshes);
  
  printf ("Loading Skeleton\n");
  pskeleton mySkeleton;
  mySkeleton = md5meshfile_loadSkeleton (meshfile);
  if (mySkeleton == NULL)
    {
      fprintf (stderr, "Generating skeleton failed\n");
      exit (EXIT_FAILURE);
    }

  printf ("Print Skeleton\n");
  printSkeleton (mySkeleton);
  /*
    ppmesh     myMeshes;
    md5mesh_loadfile ( argv[1], &mySkeleton, &myMeshes );
  */
  /*
    printSkeleton (*mySkeleton);
  */
  md5meshfile_close (meshfile);
  return 0;
}
