#include "skeleton.h"
/*
 *  Our Test Case scenario
 */
#define NUM_JOINTS 3
int main ()
{
  pskeleton tom = getNewSkeleton ();

  tom->name = "tom";
  char *jointName1 = "arm";
  char *jointName2 = "hand";
  char *jointName3 = "finger";
  char *jointName4 = "nail_onmyheadfortheeyes";


  skeletonAddJoint (tom, jointName1, -1,
		    1,2,3,
		    2,3,4);
  int dxi;
  for (dxi = 0; dxi < 10000; dxi++)
    {

      skeletonAddJoint (tom, jointName2, 0,
			10,20,30,
			20,30,40);
  

      skeletonAddJoint (tom, jointName3, 0,
			1.4,2.4,3.4,
			2.4,3.4,4.4);
  
      skeletonAddJoint (tom, jointName4, 2,
			101,202,303,
			2.01,3.01,4.01);

    }  
  printSkeleton (tom);
  skeletonCleanUp (tom);
  return 0;
}
