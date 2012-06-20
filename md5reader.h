/**
 * Reads in a md5mesh file returning a struct to the model
 */

#ifndef __MD5__READER__
#define __MD5__READER__

#include <stdio.h>

/**
 * Holds all md5mesh model data from file.
 */
typedef struct
{
  char * name;
  int parent;
  int posx;
  int posy;
  int posz;
  int orientx;
  int orienty;
  int orientz;
} joint;

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
md5mesh * md5mesh_loadfile (FILE * fp);

#endif
