#include "skeleton.h"
/*
 *  Our Test Case scenario
 */
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
