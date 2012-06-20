/**
 * Reads in a md5mesh file returning a struct to the model
 */

#include "md5reader.h"
#include <stdio.h>
#include <stdlib.h>

//  Recognized formats
#define FORMAT10 10

//  Tokens in the file format
enum md510tokens_enum
{
  MD5VERSION,
  COMMANDLINE,
  NUMJOINTS,
  NUMMESHES,
  JOINTS,
  MESH,
  MD510TOKENCOUNT
};
char * md510tokens[] = {
 "MD5Version",
 "commandline",
 "numJoints",
 "numMeshes",
 "joints",
 "mesh"
};

static void parseline (FILE *fp, char **line, size_t *length)
{

  if (getline (line, length, fp) == -1)
    {
      fprintf (stderr, "error reading from file\n");
      exit (EXIT_FAILURE);
    }
  #ifdef DEBUG_FILE
  printf ("Line length: %d\n", *length);
  printf ("LINE: %s", *line);
  #endif
}

md5mesh * md5mesh_loadfile (FILE * fp)
{
  md5mesh * md5meshanimal = (md5mesh *)malloc (sizeof (md5mesh));

  //  Parse version
  char *line = NULL;
  size_t length = 0;
  parseline (fp, &line, &length);

  char token[25];
  int version;
  sscanf (line, "%s %d",&token, &version);
  printf ("Version: %d\n", version);

  //  printf ("Token %s, version %d", token, version);
  
  //  Parse Number of Joints

  //  Parse joints
  
  //  Parse Number of meshes

  //  Parse Meshes

  return md5meshanimal;
}
