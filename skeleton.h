#ifndef __SKELETON_H_
#define __SKELETON_H_

typedef struct _joint
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
} joint, *pjoint, **ppjoint;

typedef struct _skeleton
{
  int numJoints;
  int MAXJOINTS;
  ppjoint joints;
} skeleton, *pskeleton, **ppskeleton;

pskeleton getNewSkeleton ();

void skeletonAddJoint (pskeleton skel, int num, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ);
#endif
