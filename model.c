#include <stdlib.h>
#include "model.h"

pmodel loadModel (char * resourceName)
{
  /* generate model resource */
  pmodel retModel = (pmodel) malloc (sizeof (model));

  /* Detect resource type */
    
  /* Open the resource's file */
   pmd5meshfile meshfile;
  meshfile = md5meshfile_open (resourceName);
  
  /* Load resource's info */
  pmd5info md5fileinfo;
  md5fileinfo = md5meshfile_loadInfo (meshfile);

  /* Load resource's skeleton */
  retModel->skel = md5meshfile_loadSkeleton (meshfile);
  if (retModel->skel == NULL)
    {
      fprintf (stderr, "Generating skeleton failed\n");
      exit (EXIT_FAILURE);
    }
  
  md5meshfile_close (meshfile);
  //  free (md5fileinfo->name);
  free (md5fileinfo);
  /*  printSkeleton (retModel->skel); */
  /* Load resource's mesh */  

  /* return the model */
  return retModel;
}

void modelCleanUp (pmodel rmmodel)
{
  skeletonCleanUp (rmmodel->skel);
  meshCleanUp (rmmodel->baseMesh);
  free (rmmodel);
}
