#include <stdio.h>

int search(int arr[], int n, int e)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == e)
            return 1;
    }
    return 0;
}

int search2(int arr[][3], int n, int e)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i][0] == e)
            return 1;
    }
    return 0;
}

int fifo(int req_arr[], int req_size, int frames)
{
    int i;
    int next = 0;
    int frame_arr[10];
    int fault_count = 0;

    for (int i = 0; i < frames; i++)
    {
        frame_arr[i] = -1;
    }
    i = 0;

    while (i < req_size)
    {
        if (search(frame_arr, frames, req_arr[i]) == 0)
        {
            frame_arr[next] = req_arr[i];
            fault_count++;

            if (next < frames - 1)
                next++;
            else
                next = 0;
        }
        i++;
    }
    return fault_count;
}

int lfu(int req_arr[], int req_size, int frames)
{
    int i;
    int next = 0;
    int frame_arr[10][3];
    int fault_count = 0;

    for (int i = 0; i < frames; i++)
    {
        frame_arr[i][0] = -1; // value
        frame_arr[i][1] = 0;  // used count
        frame_arr[i][2] = 0;  // entry time
    }
    i = 0;

    while (i < req_size)
    {
        if (search2(frame_arr, frames, req_arr[i]) == 0)
        {
            next = 0;
            for (int j = 0; j < frames; j++)
            {
                if (frame_arr[j][1] == frame_arr[next][1] && frame_arr[j][2] < frame_arr[next][2])
                {
                    next = j;
                }
                if (frame_arr[j][1] < frame_arr[next][1])
                {
                    next = j;
                }
            }

            frame_arr[next][0] = req_arr[i];
            frame_arr[next][1] = 0;
            frame_arr[next][2] = i;
            fault_count++;
        }
        else
        {
            for (int j = 0; j < frames; j++)
            {
                if (frame_arr[j][0] == req_arr[i])
                {
                    frame_arr[j][1]++;
                }
            }
        }
        i++;
    }
    return fault_count;
}

int lru(int req_arr[], int req_size, int frames)
{
    int i;
    int next = 0;
    int frame_arr[10][3];
    int fault_count = 0;

    for (int i = 0; i < frames; i++)
    {
        frame_arr[i][0] = -1; // value
        frame_arr[i][1] = 0;  // used count
        frame_arr[i][2] = 0;  // entry time
    }
    i = 0;

    while (i < req_size)
    {
        if (search2(frame_arr, frames, req_arr[i]) == 0)
        {
            next = 0;
            for (int j = 0; j < frames; j++)
            {
                if (frame_arr[j][2] < frame_arr[next][2])
                {
                    next = j;
                }
            }

            frame_arr[next][0] = req_arr[i];
            frame_arr[next][1] = 0;
            frame_arr[next][2] = i;
            fault_count++;
        }
        else
        {
            for (int j = 0; j < frames; j++)
            {
                if (frame_arr[j][0] == req_arr[i])
                {
                    frame_arr[j][1]++;
                }
            }
        }
        i++;
    }
    return fault_count;
}

void main()
{
    int arr[] = {1, 3, 0, 3, 5, 6, 3};
    printf("Page faults: %d\n", fifo(arr, 7, 3));

    int arr2[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    printf("Page faults: %d\n", lru(arr2, 13, 4));

    int arr2[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2};
    printf("Page faults: %d\n", lfu(arr2, 13, 4));
}