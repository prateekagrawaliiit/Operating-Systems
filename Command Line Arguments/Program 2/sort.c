#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[])
{
  int array[100], n, c, d, swap;
  int inc = 0;
  n = atoi(argv[1]);
  inc = argv[2];

  
  for (c = 0; c < n; c++)
    array[c] = atoi(argv[c + 3]);

  if (inc == 1)
  {
    for (c = 0 ; c < n - 1; c++)
    {
      for (d = 0 ; d < n - c - 1; d++)
      {
        if (array[d] > array[d + 1]) /* For decreasing order use < */
        {
          swap       = array[d];
          array[d]   = array[d + 1];
          array[d + 1] = swap;
        }
      }
    }    printf("Sorted list in ascending order:\n");    for (c = 0; c < n; c++)
    printf("%d\n", array[c]);
  }
  else
  {
    for (c = 0 ; c < n - 1; c++)
    {
      for (d = 0 ; d < n - c - 1; d++)
      {
        if (array[d] < array[d + 1]) /* For decreasing order use < */
        {
          swap       = array[d];
          array[d]   = array[d + 1];
          array[d + 1] = swap;
        }
      }
    }    printf("Sorted list in decreasing order:\n");    for (c = 0; c < n; c++)
    printf("%d\n", array[c]);
  }  return 0;
}