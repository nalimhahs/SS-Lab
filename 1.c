#include <stdio.h>
#define TIME_SLICE 1

int PROC_COUNT = 0;

typedef struct proc
{
    int id;
    int burst_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int priority;
} proc;

void swap(proc *xp, proc *yp)
{
    proc temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void sortByID(proc proc_list[], int start, int end)
{
    int i, j, small;

    for (i = start; i < end - 1; i++)
    {
        small = i;
        for (j = i + 1; j < end; j++)
            if (proc_list[j].id < proc_list[small].id)
                small = j;

        swap(&proc_list[small], &proc_list[i]);
    }
}

void sortByID_(proc proc_list[])
{
    sortByID(proc_list, 0, PROC_COUNT);
}

void sortByArrival(proc proc_list[], int start, int end)
{
    int i, j, small;

    for (i = start; i < end - 1; i++)
    {
        small = i;
        for (j = i + 1; j < end; j++)
            if (proc_list[j].arrival_time < proc_list[small].arrival_time)
                small = j;

        swap(&proc_list[small], &proc_list[i]);
    }
}

void sortByArrival_(proc proc_list[])
{
    sortByArrival(proc_list, 0, PROC_COUNT);
}

void sortByBurstTime(proc proc_list[], int start, int end)
{
    int i, j, small;

    for (i = start; i < end - 1; i++)
    {
        small = i;
        for (j = i + 1; j < end; j++)
            if (proc_list[j].burst_time < proc_list[small].burst_time)
                small = j;

        swap(&proc_list[small], &proc_list[i]);
    }
}

void sortByBurstTime_(proc proc_list[])
{
    sortByBurstTime(proc_list, 0, PROC_COUNT);
}

void sortByPriority(proc proc_list[], int start, int end)
{
    int i, j, small;

    for (i = start; i < end - 1; i++)
    {
        small = i;
        for (j = i + 1; j < end; j++)
            if (proc_list[j].priority < proc_list[small].priority)
                small = j;

        swap(&proc_list[small], &proc_list[i]);
    }
}

void sortByPriority_(proc proc_list[])
{
    sortByPriority(proc_list, 0, PROC_COUNT);
}

void resetList(proc proc_list[])
{
    sortByID_(proc_list);
    for (int i = 0; i < PROC_COUNT; i++)
    {
        proc_list[i].waiting_time = 0;
        proc_list[i].turnaround_time = 0;
        proc_list[i].completion_time = 0;
    }
}

void printList(proc proc_list[])
{

    for (int i = 0; i < PROC_COUNT; i++)
    {
        printf("\nID: %d, B: %d, A: %d, W: %d, T: %d", proc_list[i].id, proc_list[i].burst_time, proc_list[i].arrival_time, proc_list[i].waiting_time, proc_list[i].turnaround_time);
    }
}

void calcAverage(proc proc_list[])
{
    int avg_waiting = 0;
    int avg_turnaround = 0;

    for (int i = 0; i < PROC_COUNT; i++)
    {
        avg_waiting += proc_list[i].waiting_time;
        avg_turnaround += proc_list[i].turnaround_time;
    }

    printf("Average Waiting Time: %f\n", (float)avg_waiting / (float)PROC_COUNT);
    printf("Average Turnaround Time: %f\n", (float)avg_turnaround / (float)PROC_COUNT);
}

void calcResult(proc proc_list[])
{
    proc_list[0].turnaround_time = proc_list[0].burst_time - proc_list[0].arrival_time;
    proc_list[0].waiting_time = proc_list[0].arrival_time;

    for (int i = 1; i < PROC_COUNT; i++)
    {
        int cpu_idle_offset = proc_list[i].arrival_time - proc_list[i - 1].turnaround_time;

        if (cpu_idle_offset > 0)
        {
            // proc_list[i].turnaround_time = proc_list[i - 1].turnaround_time + cpu_idle_offset + proc_list[i].burst_time;
            // proc_list[i].waiting_time = proc_list[i].turnaround_time - proc_list[i].burst_time - 2 *cpu_idle_offset;
            printf("\n***CPU IDLE ERROR***\n");
            return;
        }
        else
        {
            int prev_completion_time = proc_list[i - 1].turnaround_time + proc_list[i - 1].arrival_time;
            proc_list[i].turnaround_time = prev_completion_time + proc_list[i].burst_time - proc_list[i].arrival_time;
            proc_list[i].waiting_time = proc_list[i].turnaround_time - proc_list[i].burst_time;
        }
    }

    printList(proc_list);
    printf("\n");
    calcAverage(proc_list);
}

void calcResultPreemptive(proc proc_list[], int count)
{
    proc proc_list_temp[20];
    for (int i = 0; i < PROC_COUNT; i++)
    {
        for (int j = count - 1; j >= 0; j--)
        {
            if (proc_list[j].id == i)
            {
                proc_list_temp[i] = proc_list[j];
                proc_list_temp[i].turnaround_time = proc_list[j].completion_time - proc_list_temp[i].arrival_time;
                proc_list_temp[i].waiting_time = proc_list_temp[i].turnaround_time - proc_list_temp[i].burst_time;
                break;
            }
        }
    }

    printList(proc_list_temp);
    printf("\n");
    calcAverage(proc_list_temp);
}

void calcFCFS(proc proc_list[])
{
    // Turn Around Time = Completion Time – Arrival Time
    resetList(proc_list);
    sortByArrival_(proc_list);

    calcResult(proc_list);
}

void calcSJF(proc proc_list[])
{
    resetList(proc_list);
    sortByBurstTime_(proc_list);
    sortByArrival_(proc_list);

    int time_elapsed = proc_list[0].arrival_time + proc_list[0].burst_time;
    int i = 1;

    while (i < PROC_COUNT)
    {
        int start = i;
        while (proc_list[i].arrival_time <= time_elapsed && i < PROC_COUNT)
            i++;
        sortByBurstTime(proc_list, start, i);
        if (start != i)
            time_elapsed += proc_list[start].burst_time;
        else
            time_elapsed += proc_list[start].arrival_time - proc_list[start - 1].burst_time;
        i = ++start;
    }

    calcResult(proc_list);
}

void calcPriority(proc proc_list[])
{
    resetList(proc_list);
    sortByPriority_(proc_list);
    sortByArrival_(proc_list);

    int time_elapsed = proc_list[0].arrival_time + proc_list[0].burst_time;
    int i = 1;

    while (i < PROC_COUNT)
    {
        int start = i;
        while (proc_list[i].arrival_time <= time_elapsed && i < PROC_COUNT)
            i++;
        sortByPriority(proc_list, start, i);
        if (start != i)
            time_elapsed += proc_list[start].burst_time;
        else
            time_elapsed += proc_list[start].arrival_time - proc_list[start - 1].burst_time;
        i = ++start;
    }

    calcResult(proc_list);
}

void calcRoundRobin(proc proc_list[])
{
    resetList(proc_list);
    sortByArrival_(proc_list);

    proc proc_list_temp[100];
    int time_elapsed = 0;
    int i = 0, iter_no = 1, j = 0;
    int flag = 0;
    proc_list_temp[0] = proc_list[0];
    proc_list_temp[0].completion_time = proc_list[0].burst_time - TIME_SLICE > 0 ? TIME_SLICE : proc_list[0].burst_time;

    while (flag < PROC_COUNT)
    {
        if (proc_list[i].arrival_time <= time_elapsed)
        {
            int remaining_burst = proc_list[i].burst_time - TIME_SLICE * (iter_no - 1);
            if ((remaining_burst <= 0 && proc_list[i].burst_time - TIME_SLICE * (iter_no - 2) > 0))
            {
                flag++;
                i++;
                continue;
            }
            if (remaining_burst < 0)
            {
                i++;
                continue;
            }
            proc_list_temp[j] = proc_list[i];
            proc_list_temp[j].completion_time = proc_list_temp[j - 1].completion_time + (remaining_burst - TIME_SLICE > 0 ? TIME_SLICE : remaining_burst);
            j++;
            time_elapsed += TIME_SLICE;
            i++;
        }
        else
        {
            i++;
        }
        if (i >= PROC_COUNT)
        {
            i = 0;
            iter_no++;
        }
    }

    calcResultPreemptive(proc_list_temp, j);
}

int main()
{
    proc proc_list[20];

    printf("Enter total number of processes: ");
    scanf("%d", &PROC_COUNT);

    for (int i = 0; i < PROC_COUNT; i++)
    {
        printf("Enter burst time, arrival time and priority of process %d: ", i);
        scanf("%d %d %d", &proc_list[i].burst_time, &proc_list[i].arrival_time, &proc_list[i].priority);
        proc_list[i].id = i;
    }

    calcFCFS(proc_list);
    calcSJF(proc_list);
    calcPriority(proc_list);
    calcRoundRobin(proc_list);
}