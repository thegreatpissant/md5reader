#ifndef __SKELETON_H_
#define __SKELETON_H_

struct joint
{
  char *name;
  struct joint *parent;
  struct joint **children;
  float posX;
  float posY;
  float posZ;
  float orX;
  float orY;
  float orZ;
  float w;
};

struct skeleton
{
  struct joint **joints;
};

struct skeleton * getNewSkeleton ();

void skeletonAddJoint (struct skeleton * skel, int num, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ);
#endif
