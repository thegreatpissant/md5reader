/**
 * Reads in a md5mesh file returning a struct to the model
 */

#define _GNU_SOURCE
#include "md5reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Recognized formats */
#define FORMAT10 10

/*  Tokens in the file format */
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
  size_t ret = 0;
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

pskeleton md5mesh_loadfile (FILE * fp)
{
  /*  A blank mesh object to populate */
  md5mesh * md5meshanimal = (md5mesh *)malloc (sizeof (md5mesh));
  pskeleton newSkeleton = getNewSkeleton();

  /*  Parser token always used */
  char token[25];
  char commandline[2458];
  char *line = NULL;
  size_t length = 0;

  /*  Parse version */
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %d",&token, &(md5meshanimal->fileVersion)) == 2)
    checkToken (token, "MD5Version",10);
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: Version: %d\n", md5meshanimal->fileVersion);
#endif
  free (line);
  line = NULL;

  /*  Parse "commandline %s" line out */
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

  /*  Parse Number of Joints */
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

  /*  Parse Number of meshes */
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %d", &token, &(md5meshanimal->numMeshes)) == 2)
    checkToken (token, "numMeshes", 9);
  else
    {
      fprintf (stderr, "Error parsing number of meshes\n");
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numMeshes: %d\n", md5meshanimal->numMeshes);
#endif
  free (line);
  line = NULL;
  
  /*  Parse joints */
  char tmpLine[255];
  parseline (fp, &line, &length);
  if (sscanf (line, "%s %s", &token, &tmpLine) == 2)
    checkToken (token, "joints", 6);
  else
    {
      fprintf (stderr, "Error parsing beginig of joints section\n");
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: tmpLine: %s\n", tmpLine);
#endif
  free (line);
  line = NULL;

  /*  Assuming the number of joints is correct parse accordingly */
  int jointsParsed = 0;
  char  jN[50]; /*  Joint Name   */
  int  jP;      /*  Joint Parent */
  float px;     /*  Position X   */
  float py;     /*  Position Y   */
  float pz;     /*  Position Z   */
  float qx;     /*  Orientation X  */
  float qy;     /*  Orientation Y  */
  float qz;     /*  Orientation Z  */
  float qw;     /*  W component im told to calculate  */
  int ret;
  for ( ; jointsParsed < md5meshanimal->numJoints; ++jointsParsed)
    {
      parseline (fp, &line, &length);
      memset (jN, '\n', 50);
      ret = sscanf (line, "%s %d ( %f %f %f ) ( %f %f %f )",
		    &jN,  &jP, &px, &py, &pz, &qx, &qy, &qz);
#ifdef DEBUG_FILE 
	printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
	printf ("DGB-MD5MESH_LOADFILE: Joint: %s %d ( %.10f %.10f %.10f ) ( %.10f %.10f %.10f )\n",
		jN, jP, px, py, pz, qx, qy, qz);
#endif
	if (ret != 8)
	{
	  fprintf (stderr, "Error Parsing joint #%d\n sscanf ret: %d\n",jointsParsed, ret); 
	  exit (EXIT_FAILURE);
	}
	
	/*
	  printf ("Joint: %s %hd ( %.10f %.10f %.10f ) ( %.10f %.10f %.10f )\n",
		jN,  jP, px, py, pz, qx, qy, qz);
	*/
	
	skeletonAddJoint (newSkeleton, jN, jP, px, py, pz, qz, qy, qz);
    }

  parseline (fp, &line, &length);
  if (sscanf (line, "%s", &token) == 1)
    checkToken (token, "}", 1);
  else
    {
      fprintf (stderr, "Error parsing clossing joint bracket\n");
      exit (EXIT_FAILURE);
    }
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
#endif
  free (line);
  line = NULL;
  
  free (md5meshanimal);
  return newSkeleton;
}
