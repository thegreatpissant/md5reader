/**
 * Reads in a md5mesh file returning a struct to a skeleton
 */

#ifndef __MD5__READER__
#define __MD5__READER__

#include "skeleton.h"
#include <stdio.h>

/**
 *  As skeleton becomes more complete, these will disapear.
 */

typedef struct
{
} vert;

typedef struct
{
} tri;

typedef struct
{
} weight;

  
typedef struct 
{
  char *name;
  int numverts;
  int numtris;
  int numweights;  
  tri *tris;
  vert *verts;
  weight *weights;
} mesh;

  
/** @@TODO
 *  Do we need to hold on to this, debug purposes??
 *  For now we discard
 */
typedef struct
{
  int   fileVersion;
  int   numJoints;
  int   numMeshes;
  joint *joints;
  mesh  *meshs;
} md5mesh;



/**
 * Loads file and returns a pointer to the md5mesh struct
 * @param fp [in] FILE* to file name
 * @return md5mesh struct
 */

pskeleton md5mesh_loadfile (char * fn);

#endif
