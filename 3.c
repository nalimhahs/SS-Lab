#include <stdio.h>

int main()
{

    int alloc_matrix[5][3] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    int max[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    int available[3] = {3, 3, 2};

    int no_of_proc = 5;
    int no_of_res = 3;

    int done[no_of_proc], ans[no_of_proc], ind = 0;

    // init done to zero
    for (int i = 0; i < no_of_proc; i++)
    {
        done[i] = 0;
    }

    // calc need matrix
    int need[no_of_proc][no_of_res];
    for (int i = 0; i < no_of_proc; i++)
    {
        for (int j = 0; j < no_of_res; j++)
            need[i][j] = max[i][j] - alloc_matrix[i][j];
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < no_of_proc; j++)
        {
            if (done[j] == 0)
            {

                int flag = 0;
                for (int k = 0; k < no_of_res; k++)
                {
                    if (need[j][k] > available[k])
                    {
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0)
                {
                    ans[ind++] = j;
                    for (int p = 0; p < no_of_res; p++)
                        available[p] += alloc_matrix[j][p];
                    done[j] = 1;
                }
            }
        }
    }

    printf("Safe Sequence is\n");
    for (int i = 0; i < no_of_proc - 1; i++)
        printf("P%d -> ", ans[i]);
    printf(" P%d", ans[no_of_proc - 1]);

}