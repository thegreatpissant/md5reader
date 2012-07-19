#ifndef __SKELETON_H_
#define __SKELETON_H_

typedef struct _joint joint, *pjoint, **ppjoint;
struct _joint
{
  char *name;
  pjoint parent;
  ppjoint children;
  int numChildren;
  int MAXCHILDREN;
  float posX;
  float posY;
  float posZ;
  float orX;
  float orY;
  float orZ;
  float w;
} ;

typedef struct _skeleton skeleton, *pskeleton, **ppskeleton;
struct _skeleton
{
  char * name;
  int numJoints;
  int MAXJOINTS;
  ppjoint joints;
} ;

pskeleton getNewSkeleton ();

void skeletonAddJoint (pskeleton skel, char *name, int parent, 
		       float posX, float posY, float posZ,
		       float orX,  float orY,  float orZ);

void printSkeleton (pskeleton skel);
void skeletonCleanUp (pskeleton skel);
#endif
