#include <stdio.h>

const int MIN = 0;
const int MAX = 199;

int calcFCFS(int head, int req[], int n)
{
    int totalSeek = 0;
    for (int i = 0; i < n; i++)
    {
        totalSeek += (req[i] - head < 0 ? head - req[i] : req[i] - head);
        head = req[i];
    }
    return totalSeek;
}

int getNext(int current, int req[][2], int n, int direction)
{

    int head;
    for (int i = 0; i < n; i++)
    {
        if (direction == -1 && req[i][1] == 0 && req[i][0] < current)
        {
            head = req[i][0];
            break;
        }
        if (direction == 1 && req[i][1] == 0 && req[i][0] > current)
        {
            head = req[i][0];
            break;
        }
        
    }
    
    int index = 0;
    for (int i = 0; i < n; i++)
    {
        if (direction == -1)
        {
            if (req[i][1] == 0 && req[i][0] <= current && req[i][0] >= head)
            {
                head = req[i][0];
                index = i;
            }
        }
        else
        {
            if (req[i][1] == 0 && req[i][0] >= current && req[i][0] <= head)
            {
                head = req[i][0];
                index = i;
            }
        }
    }
    if (head == current) //no change
    {
        return -1;
    }
    else
    {
        req[index][1] = 1;
        return head;
    }
}

int calcSCAN(int head, int req[], int n)
{
    int totalSeek = 0;
    int direction = -1;

    int seq[10][2];
    for (int i = 0; i < n; i++)
    {
        seq[i][0] = req[i];
        seq[i][1] = 0;
    }

    int flag = 0;
    int i = 0;
    while (flag < 2)
    {
        int next = getNext(head, seq, n, direction);
        if (next == -1)
        {
            if (flag == 1)
                break;
            if (direction < 0)
            {
                totalSeek += (MIN - head < 0 ? head - MIN : MIN - head);
                head = MIN;
            }
            else
            {
                totalSeek += (MAX - head < 0 ? head - MAX : MAX - head);
                head = MAX;
            }
            flag++;
            direction *= -1;
            continue;
        }
        totalSeek += (next - head < 0 ? head - next : next - head);
        head = next;
    }

    return totalSeek;
}

int calcCSCAN(int head, int req[], int n)
{
    int totalSeek = 0;
    int direction = 1;

    int seq[10][2];
    for (int i = 0; i < n; i++)
    {
        seq[i][0] = req[i];
        seq[i][1] = 0;
    }

    int flag = 0;
    int i = 0;
    int rotate = 0;
    while (flag < 2)
    {
        if (rotate == 1)
        {
            rotate = 0;
            head = MIN;
        }
        
        int next = getNext(head, seq, n, direction);
        if (next == -1)
        {
            if (flag == 1)
                break;
            if (direction < 0)
            {
                totalSeek += (MIN - head < 0 ? head - MIN : MIN - head);
                head = MIN;
            }
            else
            {
                totalSeek += (MAX - head < 0 ? head - MAX : MAX - head);
                head = MAX;
            }
            rotate = 1;
            flag++;
            continue;
        }
        totalSeek += (next - head < 0 ? head - next : next - head);
        head = next;
    }

    return totalSeek;
}

int main()
{

    int arr[] = {176, 79, 34, 60, 92, 11, 41, 114};
    int head = 50;

    printf("%d\n", calcCSCAN(head, arr, 8));

    return 0;
}