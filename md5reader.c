/**
 * Reads in a md5mesh file returning a struct to the model
 */

#define _GNU_SOURCE
#include "model.h"
#include "md5reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

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
  char *comment = NULL;
  char found;
  char *j, *k;
  int i = 0;
  *line = NULL;
  while (blank == 1) 
    {
      ret = getline (line, length, fp);
      if (ret == -1) 
	break;
      //  Check for a comment and fake remove it with a '\0'
      if ((comment = strstr (*line, "//")) != NULL)
	{
	  *comment = '\0';
	  ret = strlen (*line);
	}
      j = *line;
      //  Look for blank spaces in the front
      while (isspace(*j) && (*j != '\0'))
	{
	  j++;
	}
      //  left justify the line, remove blanks in the front
      //  Also inform the loop it is not a blank line and return it
      if (*j != '\0')
	{
	  blank = 0;
	  k = *line;
	  while (*j != '\0')
	    {
	      if ((*k = *j) != '\0') {
		k++;
		j++;
	      }
	    } 

	  ret = strlen (*line);
	}
    }
  //  It should always be this anyway
  *length = ret;
  return ret;
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

pmd5info md5meshfile_loadInfo (pmd5meshfile meshfile)
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
  length = 0;

  /*
   *  Parse "commandline %s" line out 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "commandline %s",&commandline) != 1) 
    {
      fprintf (stderr, "Error reading commandline\n");
      free (line);
      length = 0;
      return NULL;
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: commandline Line: \"%s\"\n", commandline);
#endif
  free (line);
  line = NULL;
  length = 0;

  /*
   *  Parse Number of Joints 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "numJoints %d", &numJoints) != 1)
    {
      fprintf (stderr, "Error reading numJoints\n");
      return NULL;
    }
#ifdef DEBUG_FILE
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numJoints: %d\n", numJoints);
#endif
  free (line);
  line = NULL;
  length = 0;

  /*
   *  Parse Number of meshes 
   */
  parseline (fp, &line, &length);
  if (sscanf (line, "numMeshes %d", &numMeshes) != 1)
    {
      fprintf (stderr, "Error parsing number of meshes\n");
      fprintf (stderr, "line: %s", line);
      free (line);
      length = 0;
      return NULL;
    }
#ifdef DEBUG_FILE 
  printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
  printf ("DGB-MD5MESH_LOADFILE: numMeshes: %d\n", numMeshes);
#endif
  free (line);
  length = 0;
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
	  length = 0;
	}
      if (parseline (fp, &line, &length) == -1)
	{
	  fprintf (stderr, "Error parsing beginig of joints section.  Check file format.\n");
	  free (line);
	  line = NULL;
	  length = 0;
	  return NULL;
	}
    } while (sscanf (line, "joints %s", &token, &tmpLine) != 1);
  free (line);
  line = NULL;
  length = 0;

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
#ifdef DEBUG_FILE 
      printf ("Parsing line: %s\n", line);
#endif
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
	      length = 0;
	      return NULL;
	    }
	  else
	    {
	      free (line);
	      line = NULL;
	      length = 0;
	      break;
	    }
	}
      else
	{
	  fprintf (stderr, "Error Parsing joint #%d\n sscanf ret: %d\n",jointsParsed, ret); 
	  free (line);
	  line = NULL;
	  length = 0;
	  return NULL;
	}
#ifdef DEBUG_FILE 
      printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
      printf ("DGB-MD5MESH_LOADFILE: Joint: %s %d ( %.10f %.10f %.10f ) ( %.10f %.10f %.10f )\n",
	      jN, jP, px, py, pz, qx, qy, qz);
#endif	      
      free (line);
      line = NULL;
      length = 0;
    } while (1);
  return newSkeleton;
}

pmesh md5meshfile_loadMesh (pmd5meshfile meshfile, int meshnumber)
{
  FILE * fp = meshfile->fp;
  char token[25];
  char tmpLine[2555];
  char *line = NULL;
  size_t length = 0;
  int cur_mesh = -1;
  /* To begining of file */
  rewind (fp);
  
  /*
   * Parse for mesh number
   */
  while (cur_mesh++ != meshnumber)
    {
      do
	{
	  if (line != NULL)
	    {
	      free (line);
	      line = NULL;
	      length = 0;
	    }
	  if (parseline (fp, &line, &length) == -1)
	    {
	      fprintf (stderr, "Error Finding mesh number %d\n", meshnumber);
	      free (line);
	      line = NULL;
	      length = 0;
	      return NULL;
	    }
	} while (sscanf (line, "mesh %s", &token, &tmpLine) != 1);
    }
  free (line);
  line = NULL;
  length = 0;
  /* We should have hit the correct mesh number */
  pmesh newmesh;
  int numverts = 0, numtris = 0, numweights = 0;
  int verti = 0, vertstartweight, vertcountweight;
  float verts = 0, vertt = 0;
  int trii = 0, triidx0 = 0, triidx1 = 0, triidx2 = 0;
  int weighti = 0, weightjoint = 0;
  float weightbias = 0, weightposx = 0, weightposy = 0, weightposz = 0;
  newmesh = (pmesh) malloc (sizeof (mesh));
  /* Parse for the associated shader */
  do {
    parseline (fp, &line, &length);
    if (sscanf (line, "shader %s", &tmpLine) == 1)
      {
	int shader_name_len = strlen(tmpLine);
	newmesh->shadername = (char *)malloc ( shader_name_len+1);
	strncpy (newmesh->shadername, tmpLine, shader_name_len);
	newmesh->shadername[shader_name_len] = '\0';
	
      }
    else if (sscanf (line, "numverts %d", &numverts) == 1)
      {
	newmesh->numverts = numverts;
	newmesh->verts = (ppvert) malloc (sizeof (pvert)*numverts);
      }
    else if (sscanf (line, "numtris %d", &numtris) == 1)
      {
	newmesh->numtris = numtris;
	newmesh->tris = (pptri) malloc (sizeof (ptri)*numtris);
      }
    else if (sscanf (line, "numweights %d", &numweights) == 1)
      {
	newmesh->numweights = numweights;
	newmesh->weights = (ppweight) malloc (sizeof (pweight)*numweights);
      }
    else if (sscanf (line, "vert %d ( %f %f ) %d %d",
		     &verti, &verts, &vertt, &vertstartweight, &vertcountweight) == 5)
      {
	if (verti < 0 || verti >= numverts)
	  return NULL;
	newmesh->verts[verti] = (pvert) malloc (sizeof (vert));
	newmesh->verts[verti]->s = verts;
	newmesh->verts[verti]->t = vertt;
	newmesh->verts[verti]->startweight = vertstartweight;
	newmesh->verts[verti]->countweight = vertcountweight;
      }
    else if (sscanf (line, "tri %d %d %d %d",
		     &trii, &triidx0, &triidx1, &triidx2) == 4 )
      {
	if (trii < 0 || trii >= numtris)
	  return NULL;
	newmesh->tris[trii] = (ptri) malloc (sizeof (tri));
	newmesh->tris[trii]->vert0 = triidx0;
	newmesh->tris[trii]->vert1 = triidx1;
	newmesh->tris[trii]->vert2 = triidx2;
      }
    else if (sscanf (line, "weight %d %d %f ( %f %f %f )",
		     &weighti, &weightjoint, &weightbias, 
		     &weightposx, &weightposy, &weightposz) == 6)
      {
	if (weighti < 0 || weighti >= numweights)
	  return NULL;

	newmesh->weights[weighti] = (pweight) malloc (sizeof (weight));
	newmesh->weights[weighti]->joint = weightjoint;
	newmesh->weights[weighti]->bias = weightbias;
	newmesh->weights[weighti]->posx = weightposx;
	newmesh->weights[weighti]->posy = weightposy;
	newmesh->weights[weighti]->posz = weightposz;
      }
    else if ( sscanf (line, "%s", &token) == 1)
      {
	if (checkToken (token, "}", 1) == -1)
	  {
	    free (line);
	    line = NULL;
	    length = 0;
	    return NULL;
	  }
	else 
	  {
	    free (line);
	    line = NULL;
	    length = 0;
	    break;
	  }
      }
    else 
      {
	fprintf (stderr, "Error Parsing mesh #%d\n sscanf ret: %s\n", cur_mesh-1, line);
	free (line);
	line = NULL;
	length = 0;
	return NULL;
      }
#ifdef DEBUG_FILE
    printf ("DGB-MD5MESH_LOADFILE: line: %s", line);
#endif
    free (line);
    line = NULL;
    length = 0;
  } while (1);
  return newmesh;
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
      length = 0;
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
      length = 0;
      
      /*
       * Done Parsing Mesh decleration
       */
      parseline (fp, &line, &length);
      if (sscanf (line, "%s", &token) == 1) 
	{
	  checkToken (token, "}", 1);
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
      length = 0;
            
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
