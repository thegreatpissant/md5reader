#include <stdlib.h>
#include <stdio.h>

int main (int argc, char **argv)
{

  typedef struct _node 
  {
    int x;
    char * y;
  } node, *pnode;

  printf ("sizeof node %ld\n", sizeof (node));
  printf ("sizeof node %ld\n", sizeof ( pnode));

  typedef struct _node2
  {
    float x;
    float y;
    float z;
    float w;
  } node2, *pnode2;

  printf ("sizeof node2 %ld\n", sizeof (node2));
  printf ("sizeof node2 %ld\n", sizeof (pnode2));


  pnode2 tnode;
  tnode = (pnode2) malloc (sizeof (node2));
  tnode->x = 0;
  tnode->y = 20;
  tnode->z = 30;
  tnode->w = 40;
  (tnode->x)++;
  tnode->y++;
  printf ("(tnode->x)++ : %f\n", tnode->x);
  printf ("tnode->y++ : %f \n",  tnode->y);
  return 0;
}    
