/**
 * Reads in a md5mesh file returning a struct to the model
 */

#include "md5reader.h"
#include <stdio.h>
#include <stdlib.h>

//  Debug define
#define DEBUG_FILE
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

};

static void parseline (FILE *fp, char **line, size_t *length)
{
  int ret = 0;
  int done = 0;
  int blank = 1;
  while (blank == 1) 
    {
      ret = getline (line, length, fp);
      if (ret == -1) 
	break;
      int i = 0;
      for (; i < ret; ++i)
	{
	  if ( (*line)[i] != ' ' && (*line)[i] != '\r' && (*line)[i] != '\n')
	    {
	      blank = 0;
	      break;
	    }
	}
    }
  if (ret == -1)
    {
      fprintf (stderr, "error reading from file\n");
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE
  printf ("DBG-PARSE_LINE: Line length: %d\n", *length);
  printf ("DBG-PARSE_LINE: LINE: %s", *line);
#endif
}

static void checkToken (char *ptoken, char *token, int len)
{
#ifdef DEBUG_FILE
  printf ("DBG-CHECKTOKEN: ptoken: %s, token %s\n",ptoken, token);
#endif
  if (strncmp (ptoken, token, len) != 0)
    {
      fprintf (stderr, "Expected \"%s\" token; got \"%s\" token\n", token, ptoken);
      fprintf (stderr, "Check this file is formated correctly\n");
      exit (EXIT_FAILURE);
    }
}

md5mesh * md5mesh_loadfile (FILE * fp)
{
  //  A blank mesh object to populate
  md5mesh * md5meshanimal = (md5mesh *)malloc (sizeof (md5mesh));

  //  Parser token always used
  char token[25];
  char commandline[2458];
  char *line = NULL;
  size_t length = 0;

  //  Parse version
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %d",&token, &(md5meshanimal->fileVersion)) == 2)
    checkToken (token, "MD5Version",10);
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: Version: %d\n", md5meshanimal->fileVersion);
#endif
  free (line);
  line = NULL;

  //  Parse "commandline %s" line out
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %s", &token, &commandline) == 2) 
    checkToken (token, "commandline", 11);
  else
    {
      fprintf (stderr, "Error reading commandline\n");
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: commandline Line: \"%s\"\n", commandline);
#endif
  free (line);
  line = NULL;

  //  Parse Number of Joints
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %d", &token, &(md5meshanimal->numJoints)) == 2)
    checkToken (token, "numJoints", 9);
  else
    {
      fprintf (stderr, "Error reading numJoints\n");
      printf ("line: %s\n", line);
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numJoints: %d\n", md5meshanimal->numJoints);
#endif
  free (line);
  line = NULL;

  //  Parse Number of meshes
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %d", &token, &(md5meshanimal->numMeshes)) == 2)
    checkToken (token, "numMeshes", 9);
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numMeshes: %d\n", md5meshanimal->numMeshes);
#endif
  free (line);
  line = NULL;
  
  //  Parse joints
  char tmpLine[255];
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %s", &token, &tmpLine) == 2)
    checkToken (token, "joints", 6);
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: tmpLine: %s\n", tmpLine);
#endif

  //  Parse Meshes

  return md5meshanimal;
}
