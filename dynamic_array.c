#include <stdlib.h>
#include <stdio.h>
#define MAX_JOINTS 10
#define JOINT_ALLOCATION 10
/* skeletal dynamic array */
typedef struct _array_node 
{
  int num;
} darray_node, * pdarray_node, **ppdarray_node;

typedef struct _skeleton_array 
{
  int MAX_ITEMS;
  int length;
  ppdarray_node array;
} skeleton, *pskeleton, **ppskeleton;

void initSkeleton (ppskeleton skel) 
{
  if (*skel == NULL)
    *skel = (pskeleton) malloc (sizeof (skeleton));
  (*skel)->length = 0;
  if ((*skel)->array != NULL)
    free ((*skel)->array);
  (*skel)->array = NULL;  
  (*skel)->MAX_ITEMS = 0;
}

void addSkeletonJoint (pskeleton skel, pdarray_node joint)
{
  if (skel == NULL)
    initSkeleton (&skel);
  /*  if (joint == NULL) */
  /*    newJoint (0, 0, 0, 0); */
  if (skel->length == skel->MAX_ITEMS)
    {
      skel->MAX_ITEMS += JOINT_ALLOCATION;
      skel->array = (ppdarray_node) realloc (skel->array, (skel->MAX_ITEMS) * sizeof (pdarray_node));
    }
  skel->array[skel->length++] = joint;
}

void printSkeleton (skeleton *skel)
{
  int dxi;

  for (dxi = 0; dxi < skel->length; dxi++)
    {
      printf ("array[%d]-> %i\n", dxi, skel->array[dxi]->num);
    }
}
pdarray_node getNewJoint (int x)
{
  pdarray_node tmp;
  tmp = (pdarray_node) malloc (sizeof (darray_node));
  tmp->num = x;
  return tmp;
}
int main (int argc, char **argv)
{
  int dxi = 0;
  pskeleton tom = NULL;
  
  initSkeleton (&tom);
  for (dxi = 1; dxi < MAX_JOINTS; ++dxi)
    {
      addSkeletonJoint (tom, getNewJoint(dxi));
    }
  printSkeleton (tom);
  return 0;
}

