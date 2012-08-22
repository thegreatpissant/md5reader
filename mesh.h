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
  float s, t;
  int startweight, countweight;
} vert, *pvert,**ppvert;

typedef struct
{
  int vert0, vert1, vert2;
} tri, *ptri, **pptri;

typedef struct
{
  int joint;
  float bias;
  float posx, posy, posz;
} weight, *pweight,**ppweight;

typedef struct 
{
  char *shadername;
  int numverts;
  int numtris;
  int numweights;  
  pptri tris;
  ppvert verts;
  ppweight weights;
} mesh, *pmesh, **ppmesh;

void meshCleanUp (pmesh rmmesh);
void printMesh (pmesh printingMesh);

#endif
