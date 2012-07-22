/** md5reader.h
 * Reads in a md5mesh file returning a struct to a skeleton
 */

#ifndef __MD5__READER__
#define __MD5__READER__

#include "skeleton.h"
#include "mesh.h"
#include <stdio.h>


  
/** @@TODO
 *  Do we need to hold on to this, debug purposes??
 *  For now we discard
 */
typedef struct
{
  int   fileVersion;
  int   numJoints;
  int   numMeshes;
  joint *joints;
  mesh  *meshs;
} md5mesh;

typedef struct {
  char * filename;
  FILE * fp;
} _md5meshfile, md5meshfile, *pmd5meshfile;

typedef struct {
  char * name;
  int fileVersion;
  int numJoints;
  int numMeshes;
}md5info, *pmd5info;

/**
 * Loads file and returns a pointer to the md5mesh struct
 * @param fp [in] FILE* to file name
 * @return md5mesh struct
 */
void md5mesh_loadfile (char * fn, ppskeleton newSkeleton, ppmesh newMeshes);
/** 
 * Open the md5mesh file
 */
pmd5meshfile md5meshfile_open (char * filename);
void md5meshfile_close (pmd5meshfile meshfile);
/**
 * Load Skeleton from md5file
 */
pskeleton md5meshfile_loadSkeleton (pmd5meshfile meshfile);
/**
 * Load info from md5 file
 */
pmd5info md5meshfile_loadinfo (pmd5meshfile meshfile);
/**
 * Load mesh from file
 */
pmesh md5meshfile_loadmesh (pmd5meshfile meshfile, int meshnumber);
#endif
