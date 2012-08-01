/** model.h
 *  creates a model based on the requested asset
 */
#ifndef __MODEL_H__
#define __MODEL_H__

/* Base item of all models */
#include "skeleton.h"  
#include "mesh.h"
/* Formats recognized */
#include "model_formats.h" 

typedef struct
{
  pskeleton skel;
  pmesh baseMesh;
} model, *pmodel;

pmodel loadModel (char * resourceName);
void modelCleanUp (pmodel rmmodel);
#endif
