#include<stdio.h>
#include<math.h>
#include<pthread.h>
float a[25][25], k, fac[25][25];

/*For calculating Determinant of the Matrix */
float determinant(float a[25][25], float k)
{
  float s = 1, det = 0, b[25][25];
  int i, j, m, n, c;
  if (k == 1)
  {
    return (a[0][0]);
  }
  else
  {
    det = 0;
    for (c = 0; c < k; c++)
    {
      m = 0;
      n = 0;
      for (i = 0; i < k; i++)
      {
        for (j = 0 ; j < k; j++)
        {
          b[i][j] = 0;
          if (i != 0 && j != c)
          {
            b[m][n] = a[i][j];
            if (n < (k - 2))
              n++;
            else
            {
              n = 0;
              m++;
            }
          }
        }
      }
      det = det + s * (a[0][c] * determinant(b, k - 1));
      s = -1 * s;
    }
  }

  return (det);
}
/*Finding transpose of matrix*/
void *transpose(void *nums)
{
  int i, j; float r = k;
  float b[25][25], inverse[25][25], d;

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < r; j++)
    {
      b[i][j] = fac[j][i];
    }
  }
  d = determinant(a, r);
  for (i = 0; i < r; i++)
  {
    for (j = 0; j < r; j++)
    {
      inverse[i][j] = b[i][j] / d;
    }
  }
  printf("\n\n\nThe inverse of matrix is : \n");

  for (i = 0; i < r; i++)
  {
    for (j = 0; j < r; j++)
    {
      printf("\t%f", inverse[i][j]);
    }
    printf("\n");
  }
  pthread_exit(0);
}

void *cofactor(void *nums)
{

  float b[25][25], f = k;
  int p, q, m, n, i, j;
  for (q = 0; q < f; q++)
  {
    for (p = 0; p < f; p++)
    {
      m = 0;
      n = 0;
      for (i = 0; i < f; i++)
      {
        for (j = 0; j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = a[i][j];
            if (n < (f - 2))
              n++;
            else
            {
              n = 0;
              m++;
            }
          }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  pthread_exit(0);
}

int main()
{
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  float d;
  int i, j;
  printf("Enter the order of the Matrix : ");
  scanf("%f", &k);
  printf("Enter the elements of %.0fX%.0f Matrix : \n", k, k);
  for (i = 0; i < k; i++)
  {
    for (j = 0; j < k; j++)
    {
      scanf("%f", &a[i][j]);
    }
  }
  d = determinant(a, k);
  if (d == 0)
    printf("\nInverse of Entered Matrix is not possible\n");
  else
  {
    pthread_attr_init(&attr);
    pthread_create(&tid1, NULL, cofactor, NULL);
    pthread_join(tid1, NULL);

    pthread_attr_init(&attr);
    pthread_create(&tid2, NULL, transpose, NULL);
    pthread_join(tid2, NULL);
  }
  return 0;
}
