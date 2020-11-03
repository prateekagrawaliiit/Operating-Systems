/*
* @Author: prateek
* @Date:   2020-10-31 23:48:36
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-01 00:04:02
*/

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


#define N 3
using namespace std;

void *col_sum_check(void *params);
void *row_sum_check(void *params);
void *diag_sum_check(void *params);


int col_flag = 1;
int diag_flag = 1;
int row_flag = 1;

int mat[][N] = {{ 8, 3, 4 },
    { 1, 5, 9 },
    { 6, 7, 2 }
};

int main()
{


    pthread_t col_tid, row_tid, diag_tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&col_tid, &attr, col_sum_check, NULL);
    pthread_create(&row_tid, &attr, row_sum_check, NULL);
    pthread_create(&diag_tid, &attr, diag_sum_check, NULL);
    pthread_join(col_tid, NULL);
    pthread_join(row_tid, NULL);
    pthread_join(diag_tid, NULL);

    if((col_flag==1)&&(diag_flag==1)&&(row_flag==1))
        printf("The given matrix is a square matrix\n");
    else
        printf("The given matrix is a not square matrix\n");




}


void *col_sum_check(void *params)
{
    int sum = 0;
    int flag = 1;
    for (int i = 0; i < N; i++) {

        int rowSum = 0;
        for (int j = 0; j < N; j++)
            rowSum += mat[i][j];

        // check if every row sum is
        // equal to prime diagonal sum
        if (i == 0)
            sum = rowSum;
        else
        {
            if (rowSum != sum) {

                row_flag = 0;
                pthread_exit(0);

            }
        }


    }

    return NULL;
}
void *row_sum_check(void *params)
{
    int sum = 0;
    int flag = 1;
    for (int i = 0; i < N; i++)
    {

        int colSum = 0;
        for (int j = 0; j < N; j++)
            colSum += mat[j][i];

        // check if every column sum is
        // equal to prime diagonal sum
        if (i == 0)
            sum = colSum;
        else
        {
            if (colSum != sum)
            {
                col_flag = 0;
                pthread_exit(0);
            }

        }
    }

    return NULL;
}

void *diag_sum_check(void *params)
{
    int sum = 0, sum2 = 0;
    for (int i = 0; i < N; i++)
        sum = sum + mat[i][i];
    // the secondary diagonal
    for (int i = 0; i < N; i++)
        sum2 = sum2 + mat[i][N - 1 - i];

    if (sum != sum2)
        diag_flag=0;
    
return NULL;
}
