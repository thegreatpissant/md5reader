#include "skeleton.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct skeleton *getNewSkeleton ()
{
  struct skeleton *newSkeleton = (struct skeleton*) malloc (sizeof (struct skeleton));
  newSkeleton->joints = (struct joint **) malloc (sizeof (struct joint*));
  newSkeleton->joints[0] = NULL;
  return newSkeleton;
}

struct joint ** addJoint(struct joint **joints, struct joint *newJoint)
{
  struct joint **pos;
  int i;
  for (i = 1,pos = joints; *pos != NULL; ++pos,++i)
    ;
  *pos = newJoint;
  joints = (struct joint **) realloc (joints, sizeof (struct joint*) * (i+1));
  joints[i] = NULL;
  return joints;
}

void printSkeleton (struct skeleton * skel)
{
  printf ("Printing skeleton\n");
  struct joint ** joints = skel->joints;
  for (; *joints != NULL; joints++)
    {
      printf ("Name: %s\n", (*joints)->name);
      printf ("posx: %f\n", (*joints)->posX);
    }
}

void skeletonAddJoint (struct skeleton * skel, int num, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ)
{
  struct joint * newJoint = (struct joint*) malloc (sizeof (struct joint));
  newJoint->name = (char*) malloc (sizeof (strlen(name)+1));
  strcpy (newJoint->name, name);
  newJoint->children = (struct joint**) malloc (sizeof (struct joint *));
  newJoint->children[0] = NULL;

  if (parent != -1)
    {
      newJoint->parent = skel->joints[parent];
      addJoint(newJoint->parent->children, newJoint);
    }
  else
    {
      newJoint->parent = NULL;
    }
  newJoint->posX = posX;
  newJoint->posY = posY;
  newJoint->posZ = posZ;
  newJoint->orX  = orX;
  newJoint->orY  = orY;
  newJoint->orZ  = orZ;
  newJoint->w    =  0;

  //  skel->joints = addJoint (skel->joints, newJoint);
  addJoint (skel->joints, newJoint);

}

#define NUM_JOINTS 3
int main ()
{
  struct skeleton *tom = getNewSkeleton ();

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

}
