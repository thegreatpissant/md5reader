/**
 * Reads in a md5mesh file returning a struct to the model
 */

#define _GNU_SOURCE
#include "model.h"
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

static char * md5basename (char * name);

static int parseline (FILE *fp, char **line, size_t *length)
{
  size_t ret = 0;
  int blank = 1;
  *line = NULL;
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
      return -1;
    }
#ifdef DEBUG_FILE
  printf ("DBG-PARSE_LINE: Line length: %d\n", *length);
  printf ("DBG-PARSE_LINE: LINE: %s", *line);
#endif
}

static int checkToken (char *ptoken, char *token, int len)
{
#ifdef DEBUG_FILE
  printf ("DBG-CHECKTOKEN: ptoken: %s, token %s\n",ptoken, token);
#endif
  if (strncmp (ptoken, token, len) != 0)
    {
      fprintf (stderr, "Expected \"%s\" token; got \"%s\" token\n", token, ptoken);
      fprintf (stderr, "Check this file is formated correctly\n");
      return -1;
    }
}

pmd5info md5meshfile_loadinfo (pmd5meshfile meshfile)
{
  FILE * fp = meshfile->fp;
  /*  Parser token always used */
  char token[25];
  char commandline[2458];
  char *line = NULL;
  size_t length = 0;
  int fileVersion;
  int numJoints;
  int numMeshes;
  pmd5info md5MeshInfo; 
  /* to the begining of the file */
  rewind (fp);
  /*
   *  Parse version 
   */
  do 
    {  
      if (parseline (fp, &line, &length) == -1)
	{
	  fprintf (stderr, "Error reading Version, bad file\n");
	  return NULL;
	}
    } while (sscanf (line, "MD5Version %d", &fileVersion) != 1);

  free (line);
  line = NULL;

  /*
   *  Parse "commandline %s" line out 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "commandline %s",&commandline) != 1) 
    {
      fprintf (stderr, "Error reading commandline\n");
      free (line);
      return NULL;
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: commandline Line: \"%s\"\n", commandline);
#endif
  free (line);
  line = NULL;

  /*
   *  Parse Number of Joints 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "numJoints %d", &numJoints) != 1)
    {
      fprintf (stderr, "Error reading numJoints\n");
      printf ("line: %s\n", line);
      return NULL;
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numJoints: %d\n", numJoints);
#endif
  free (line);
  line = NULL;

  /*
   *  Parse Number of meshes 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "numMeshes %d", &numMeshes) != 1)
    {
      fprintf (stderr, "Error parsing number of meshes\n");
      fprintf (stderr, "line: %s", line);
      free (line);
      return NULL;
    }
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numMeshes: %d\n", numMeshes);
#endif
  free (line);
  line = NULL;

  md5MeshInfo = (pmd5info) malloc (sizeof (md5info));
  md5MeshInfo->fileVersion = fileVersion;
  md5MeshInfo->numJoints = numJoints;
  md5MeshInfo->numMeshes = numMeshes;
  return md5MeshInfo;
}
pskeleton md5meshfile_loadSkeleton (pmd5meshfile meshfile)
{
  FILE *fp = meshfile->fp;
  char token[25];
  char tmpLine[255];
  char *line = NULL;
  size_t length = 0;
  /* to the begining of the file */
  rewind (fp);
  pskeleton newSkeleton = getNewSkeleton (0);
  newSkeleton->name = md5basename (meshfile->filename);
  /*
   *  Parse joints 
   */
  do 
    {  
      if (line != NULL) 
	{
	  free (line);
	  line = NULL;
	}
    if (parseline (fp, &line, &length) == -1)
	{
	  fprintf (stderr, "Error parsing beginig of joints section.  Check file format.\n");
	  free (line);
	  line = NULL;
	  return NULL;
	}
    } while (sscanf (line, "joints %s", &token, &tmpLine) != 1);
  free (line);
  line = NULL;

  /*  
   * Assuming the number of joints is correct parse accordingly 
   */
  int jointsParsed = 0;
  char  jN[50]; /*  Joint Name   */
  int   jP;     /*  Joint Parent */
  float px;     /*  Position X   */
  float py;     /*  Position Y   */
  float pz;     /*  Position Z   */
  float qx;     /*  Orientation X  */
  float qy;     /*  Orientation Y  */
  float qz;     /*  Orientation Z  */
  float qw;     /*  W component im told to calculate  */
  int ret;
  do
    {

      parseline (fp, &line, &length);
      printf ("Parsing line: %s\n", line);
      memset (jN, '\n', 50);
      ret = sscanf (line, "%s %d ( %f %f %f ) ( %f %f %f )",
		    &jN,  &jP, &px, &py, &pz, &qx, &qy, &qz);
      if (ret == 8)
	{
	  skeletonAddJoint (newSkeleton, jN, jP, px, py, pz, qz, qy, qz);	  
	}
      else if ( sscanf (line, "%s", &token) == 1)
	{
	  if (checkToken (token, "}", 1) == -1)
	    {
	      free (line);
	      line = NULL;
	      return NULL;
	    }
	  else
	    {
	      free (line);
	      line = NULL;
	      break;
	    }
	}
      else
	{
	  fprintf (stderr, "Error Parsing joint #%d\n sscanf ret: %d\n",jointsParsed, ret); 
	  free (line);
	  line = NULL;
	  return NULL;
	}
#ifdef DEBUG_FILE 
      printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
      printf ("DGB-MD5MESH_LOADFILE: Joint: %s %d ( %.10f %.10f %.10f ) ( %.10f %.10f %.10f )\n",
	      jN, jP, px, py, pz, qx, qy, qz);
#endif	      
      free (line);
      line = NULL;
    } while (1);
  return newSkeleton;
}
pmesh md5meshfile_loadMesh (pmd5meshfile meshfile, int meshnumber)
{
}
pmd5meshfile md5meshfile_open (char *filename)
{
  FILE * fp;
  pmd5meshfile meshfile;
  size_t fnameLen;
  fp = fopen (filename, "r");
  if (fp == NULL)
    {
      fprintf (stderr, "Error opening file \"%s\"\n", filename);
      return NULL;
    }
  fnameLen = strlen(filename);
  meshfile = (pmd5meshfile) malloc (sizeof (md5meshfile));
  meshfile->filename = (char *) malloc (fnameLen+1);
  meshfile->filename[fnameLen] = '\0';
  meshfile->fp = fp;
  strncpy (meshfile->filename, filename,fnameLen);
  return meshfile;
}
void md5meshfile_close (pmd5meshfile meshfile)
{
  fclose (meshfile->fp);
  free (meshfile->filename);
  free (meshfile);
}

#ifdef NOREADER
/* This is for reference only Transitioning to functions above*/
void md5mesh_loadfile (char * fn, ppskeleton retSkeleton, ppmesh retMeshes )
{
  /*
   * Parse Mesh sections
   */
  int mdx;
  for (mdx = 0; mdx < numMeshes; mdx++)
    {
      /*
       * Parse Decleration Mesh token "mesh {"
       */
      parseline (fp, &line, &length);
      if (sscanf (line, "%s %s", &token, &tmpLine) == 2) 
	{
	  checkToken (token, "mesh", 4);
	  checkToken (tmpLine, "{", 1);
	}
      else
	{
	  fprintf (stderr, "Error parsing Opening mesh decleration.\n");
	  exit (EXIT_FAILURE);
	}
#ifdef DEBUG_FILE 
      printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
#endif
      free (line);
      line = NULL;
      /*
       *  Parse shader name
       */
      parseline (fp, &line, &length);
      if (sscanf (line, "%s %s", &token, &tmpLine) == 2) 
	{
	  checkToken (token, "shader", 6);
	}
      else
	{
	  fprintf (stderr, "Error parsing Opening mesh decleration.\n");
	  exit (EXIT_FAILURE);
	}
#ifdef DEBUG_FILE 
      printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
#endif
      free (line);
      line = NULL;      
      
      /*
       * Done Parsing Mesh decleration
       */
      parseline (fp, &line, &length);
      if (sscanf (line, "%s", &token) == 1) 
	{
	  checkToken (token, "{", 1);
	}
      else
	{
	  fprintf (stderr, "Error parsing Closing mesh decleration.\n");
	  exit (EXIT_FAILURE);
	}
#ifdef DEBUG_FILE 
      printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
#endif
      free (line);
      line = NULL;
            
    }

  newSkeleton->name = md5basename (fn);
}
#endif
static char * md5basename (char * name)
{
  char * basen;
  char * bname;
  size_t nlen;
    
  if ( (basen = basename (name)) != NULL)
    {
      nlen = strlen (basen);
      bname = (char *) malloc (nlen+1);
      bname[nlen] = '\0';
      strncpy (bname, basen, nlen);
      return bname;
    }

  return NULL;
}
