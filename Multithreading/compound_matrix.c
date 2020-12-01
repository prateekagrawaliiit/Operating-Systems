/*
* @Author: prateek
* @Date:   2020-12-01 15:00:33
* @Last Modified by:   prateek
* @Last Modified time: 2020-12-01 16:06:49
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>

// Number of Rows
#define M 3
// Number of Columns
#define N 3

// Original Matrix A
// Output 1 Matrix
// int A[M][N] = {{1, 4, 1}, {0, -1, 6}, {10, 1, 2}};

// Output 2 Matrix
// int A[M][N] = {{1, 1, 1}, {1, 1, 1}, {1,1, 1}};

// Output 3 Matrix
int A[M][N] = {{1, 0, 0}, {0, 1, 0}, {0,0, 1}};

// Compound Matrix C
int C[M][N];

// Structure to hold the row index and the column index.
struct v
{
    int i;//row index
    int j; //column index
};

void *runner(void *param) //runner Function
{
    // get hold of the row index and the column index from the parameters.
    struct v *data = param;
    // set initial row count and column count to 0
    int row_num = 0, col_num = 0;

    // Create a temporary matrix that will hold the minor
    int temp[M - 1][N - 1];

    for (int i = 0; i < M; i++)
    {
        // if the row index is same as the value passed in the row index just ignore this iteration of row.
        if (i == (data->i))
        {
            ; //ignore the entire row if i is same as the value passed
        }
        else
        {
            col_num = 0;
            for (int j = 0; j < N; j++)
            {
                // if the column index is same as the value passed in column index just ignore this iteration of column.

                if (j == (data->j))
                {
                    ; //ignore the entire column if j is same as the value passed
                }
                else
                {
                    temp[row_num][col_num] = A[i][j];
                    col_num++; //increment the column count so that the next value is assigned in the next column.
                }
            }
            row_num++; //increment the row count so that the next value is assigned in the next row.

        }
    }
    // assign the value at row index i and column index j in the final matrix as the determinant of the minor matrix
    C[data->i][data->j] = temp[0][0] * temp[1][1] - temp[0][1] * temp[1][0];

    pthread_exit(0); //exit the thread.
}


int main()
{
    // The following code of passing the indexes has been taken from the slides where we multiply two matrices.
    // define the row index and the column index
    // Declare pthreads
    pthread_t tid;
    pthread_attr_t attr;
    int i, j, count = 0;
    for (i = 0; i < M; i++)
    {   // row loop
        for (j = 0; j < N; j++)
        {
            // column loop
            // create a structure to hold the data(row and column index)
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, runner, data);
        }
    }

    // join all the pthreads createda
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            // Join the threads
            pthread_join(tid, NULL);

        }
    }

    // Printing the matrix
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Finished
    return 0;
}