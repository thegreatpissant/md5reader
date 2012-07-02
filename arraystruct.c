#include <stdlib.h>
int main ()
{

  struct number {
    int n;
  };

  struct number numbers[10];

  struct number * i = numbers;

  struct number **is;
  *is = (struct number *) malloc (sizeof (struct number
}
