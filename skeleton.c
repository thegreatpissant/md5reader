#include "skeleton.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JOINTALLOCATION 5

pskeleton getNewSkeleton ()
{
  pskeleton newSkeleton = (pskeleton) malloc (sizeof (skeleton));
  newSkeleton->joints = NULL;
  newSkeleton->numJoints = 0;
  newSkeleton->MAXJOINTS = 0;
  return newSkeleton;
}

pskeleton addJoint(pskeleton skel, pjoint newJoint)
{
  if (skel->numJoints == skel->MAXJOINTS)
    {
      skel->MAXJOINTS += JOINTALLOCATION;
      skel->joints = (ppjoint) realloc (skel->joints, sizeof (pjoint) * skel->MAXJOINTS);
    }
  skel->joints[skel->numJoints++] = newJoint;
  return skel;
}

void printSkeleton (pskeleton skel)
{
  printf ("Printing skeleton\n");
  int idx;
  for (idx = 0; idx < skel->numJoints; idx++)
    {
      printf ("Name: %s\n", skel->joints[idx]->name);
      printf ("posx: %f\n", skel->joints[idx]->posX);
    }
}

void skeletonAddJoint (pskeleton skel, int num, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ)
{
  pjoint newJoint = (pjoint) malloc (sizeof (joint));
  newJoint->parent = NULL;
  newJoint->name = (char*) malloc (sizeof (strlen(name)+1));
  strcpy (newJoint->name, name);
  /*
  newJoint->children = (ppjoint) malloc (sizeof ());
  newJoint->children[0] = NULL;
  if (parent != -1)
    {
      newJoint->parent = skel->joints[parent];
      addJoint(newJoint->parent->children, newJoint);
    }
  */
  newJoint->posX = posX;
  newJoint->posY = posY;
  newJoint->posZ = posZ;
  newJoint->orX  = orX;
  newJoint->orY  = orY;
  newJoint->orZ  = orZ;
  newJoint->w    =  0;

  addJoint (skel, newJoint);

}

#define NUM_JOINTS 3
int main ()
{
  pskeleton tom = getNewSkeleton ();

  char *jointName1 = "arm";
  char *jointName2 = "hand";
  char *jointName3 = "finger";
  char *jointName4 = "nail";

  skeletonAddJoint (tom, 0, jointName1, -1,
		    1,2,3,
		    2,3,4);

  skeletonAddJoint (tom, 1, jointName2, 0,
		    10,20,30,
		    20,30,40);
  

  skeletonAddJoint (tom, 2, jointName3, 0,
		    1.4,2.4,3.4,
		    2.4,3.4,4.4);
  
  skeletonAddJoint (tom, 3, jointName4, 2,
		    101,202,303,
		    2.01,3.01,4.01);

  
  printSkeleton (tom);
  return 0;
}
