#include "skeleton.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JOINTALLOCATION 5
#define CHILDALLOCATION 2

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

void addChild (pskeleton skel, int pos, pjoint childJoint)
{
  if (skel->joints[pos]->numChildren == skel->joints[pos]->MAXCHILDREN)
    {
      skel->joints[pos]->MAXCHILDREN += CHILDALLOCATION;
      skel->joints[pos]->children = (ppjoint) realloc(skel->joints[pos]->children, skel->joints[pos]->MAXCHILDREN * sizeof (pjoint));
    }
  skel->joints[pos]->children[skel->joints[pos]->numChildren++] = childJoint;
}

void printSkeleton (pskeleton skel)
{
  printf ("Printing skeleton\n");
  int idx, idy;
  printf ("Number of joints: %d\n", skel->numJoints);
  for (idx = 0; idx < skel->numJoints; idx++)
    {
      printf ("Name: %s\n", skel->joints[idx]->name);
      printf ("posx: %f\n", skel->joints[idx]->posX);
      printf ("posy: %f\n", skel->joints[idx]->posY);
      printf ("posz: %f\n", skel->joints[idx]->posZ);
      printf ("orx: %f\n", skel->joints[idx]->orX);
      printf ("ory: %f\n", skel->joints[idx]->orY);
      printf ("orz: %f\n", skel->joints[idx]->orZ);
      printf ("w: %f\n", skel->joints[idx]->w);
      for (idy = 0; idy < skel->joints[idx]->numChildren; idy++)
	printf ("Child name: %s\n", skel->joints[idx]->children[idy]->name);
    }
}

void skeletonAddJoint (pskeleton skel, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ)
{
  pjoint newJoint = (pjoint) malloc (sizeof (joint));
  newJoint->parent = NULL;
  newJoint->name = (char*) malloc (sizeof (strlen(name)+1));
  newJoint->name[strlen(name)] = '\0';
  strcpy (newJoint->name, name);
  newJoint->children = NULL;
  newJoint->numChildren = 0;
  newJoint->MAXCHILDREN = 0;
  if (parent != -1)
    {
      newJoint->parent = skel->joints[parent];
      addChild(skel, parent, newJoint);
    }

  newJoint->posX = posX;
  newJoint->posY = posY;
  newJoint->posZ = posZ;
  newJoint->orX  = orX;
  newJoint->orY  = orY;
  newJoint->orZ  = orZ;
  newJoint->w    =  0;

  addJoint (skel, newJoint);

}
