/** mesh.h
 * our meshes for the models
 */

#ifndef __MESH__
#define __MESH__
/**
 *  As skeleton becomes more complete, these will disapear.
 */
typedef struct
{
  float x, y, z;
} vert;

typedef struct
{
  int vert1, vert2, vert3;
} tri;

typedef struct
{
  int joint;
  float bias;
  float posx, posy, posz;
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
} mesh, *pmesh, **ppmesh;

void meshCleanUp (pmesh rmmesh);

#endif
