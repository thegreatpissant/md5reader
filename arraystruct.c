#include <stdio.h>
#include <stdlib.h>
int main ()
{

  struct number {
    int n;
  };

  struct number numbers[10];

  struct number * i = numbers;
  
  struct number * is = NULL;
  printf ("Address of is pointer is : 0x%08x\n", &is);
  printf ("Address in is pointer is : 0x%08x\n", is);
  printf ("Sizeof is pointer is : %d\n", sizeof (is));
  printf ("Sizeof is pointer address is : %d\n", sizeof (&is));
  is = (struct number *) malloc (sizeof (struct number) *2);
  is[0].n = 10;
  is[1].n = 11;
  printf ("malloc of is\n");
  printf ("Sizeof is pointer is : %d\n", sizeof (is));
  printf ("Sizeof is pointer address is : %d\n", sizeof (&is));
  printf ("Address in is pointer is : 0x%08x\n", is);
  printf ("Data pointed to is[0] : %d\n", *(is));
  printf ("Data pointed to is[1] : %d\n", *(is+1));
  is = (struct number *) realloc (is, (sizeof (struct number) * 100));
  printf ("realloc of is *100\n");
  printf ("Sizeof is pointer is : %d\n", sizeof (is));
  printf ("Sizeof is pointer address is : %d\n", sizeof (&is));
  printf ("Data pointed to is[0] : %d\n", *(is));
  printf ("Data pointed to is[1] : %d\n", *(is+1));
  
  struct number **iss;
  iss = (struct number **)malloc (sizeof (struct number *));
  printf ("Address of iss pointer pointer is : 0x%08x\n", &iss);
  printf ("Address in iss pointer pointer is : 0x%08x\n", iss);
  printf ("Sizeof iss pointer is : %d\n", sizeof (iss));
  printf ("Sizeof iss pointer address is : %d\n", sizeof (&iss));
  printf ("Pointer in iss[0] is: 0x%08x\n", *iss); 
  iss = (struct number **) realloc (iss, sizeof (struct number *)*2);
  printf ("Realloc of iss *2\n");
  printf ("Address in iss pointer pointer is : 0x%08x\n", iss);
  printf ("Pointer in iss[0] is: 0x%08x\n", *(iss)); 
  printf ("Pointer in iss[1] is: 0x%08x\n", *(iss+1)); 
  *(iss) = is;
  printf ("*(iss) = is\n");
  printf ("Pointer in iss[0] is: 0x%08x\n", iss[0]); 
  printf ("Data in *(iss[0]) is: %d\n", *(iss[0])); 
  *(iss+1) = is+1;
  printf ("Data in *(iss[1]) is: %d\n", *(iss[1])); 
}
