#include "skeleton.h"
#include <stdlib.h>
#include <string.h>

struct skeleton *getNewSkeleton (int joints)
{
  struct skeleton *newSkeleton = (struct skeleton*) malloc (sizeof (struct skeleton));
  newSkeleton->joints = (struct joint**) malloc (sizeof(struct joint*)*joints);
  return newSkeleton;
}

void addChild(struct joint *parent, struct joint *child)
{
  if (parent->children == NULL)
    {
      *(parent->children) = (struct joint*) malloc (sizeof (struct joint *) * (++parent->numChildren));
      return;
    }
  parent->children = (struct joint**) realloc (&(parent->children), sizeof (struct joint *) * (++parent->numChildren));
  parent->children[parent->numChildren] = child;
}

void skeletonAddJoint (struct skeleton * skel, int num, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ)
{
  struct joint * newJoint = (struct joint*) malloc (sizeof (struct joint));
  newJoint->name = (char*) malloc (sizeof (strlen(name)+1));
  strcpy (newJoint->name, name);
  newJoint->children = NULL;
  newJoint->numChildren = 0;
  if (parent != -1)
    {
      newJoint->parent = skel->joints[parent];
      addChild(newJoint->parent, newJoint);
    }
  else
    newJoint->parent = NULL;
  newJoint->posX = posX;
  newJoint->posY = posY;
  newJoint->posZ = posZ;
  newJoint->orX  = orX;
  newJoint->orY  = orY;
  newJoint->orZ  = orZ;
  newJoint->w    =  0;
  skel->joints[num] = newJoint;

}

#define NUM_JOINTS 3
int main ()
{
  struct skeleton *tom = getNewSkeleton (NUM_JOINTS);

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
  
  /*  
  skeletonAddJoint (&tom, 0, jointName3, 0,
		    1.4,2.4,3.4,
		    2.4,3.4,4.4);
  skeletonAddJoint (&tom, 0, jointName4, 2,
		    101,202,303,
		    2.01,3.01,4.01);
  */
}
