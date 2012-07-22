/** model.h
 *  creates a model based on the requested asset
 */

/* Base item of all models */
#include "skeleton.h"  
#include "mesh.h"
/* Formats recognized */
#include "model_formats.h" 

typedef struct
{
  pskeleton skel;
  pmesh mesh;
} model;

