#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void meshCleanUp (pmesh rmmesh)
{
  return;
}

void printMesh (pmesh pm)
{
  int i;
  printf ("Mesh verts\n");
  for (i = 0; i < pm->numverts; ++i)
    {
      printf ("Vert #%d, s:%f, t:%f, start weight:%d, count weight:%d\n", 
	      i, pm->verts[i]->s, pm->verts[i]->t, 
	      pm->verts[i]->startweight, pm->verts[i]->countweight);
    }
  printf ("Mesh tris\n");
  for (i = 0; i < pm->numtris; ++i)
    {
      printf ("Tri #%d, verts #0:%d, #1:%d, #2:%d\n",
	      i, pm->tris[i]->vert0, pm->tris[i]->vert1, pm->tris[i]->vert2);
    }
  printf ("Mesh weights\n");
  for (i = 0; i < pm->numweights; ++i)
    {
      printf ("weight #%d, joint #%d, bias %f, posx %f, posy %f, posz %f\n",
	      i, pm->weights[i]->joint, pm->weights[i]->bias, pm->weights[i]->posx,
	      pm->weights[i]->posy, pm->weights[i]->posz);
    }
}
