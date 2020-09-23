#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dir
{
    int is_dir;
    char name[20];
    struct dir *sub_dir;
    struct dir *next;
} dir;

dir *root;
dir *cd;

dir *get_new_dir(char fname[20])
{
    dir *new;
    new = (dir *)malloc(sizeof(dir));
    strcpy(new->name, fname);
    new->next = NULL;
    new->sub_dir = NULL;
    return new;
}

dir *add_dir(dir *parent, char fname[20])
{
    dir *ptr = parent->sub_dir;
    while (ptr != NULL && ptr->next != NULL)
        ptr = ptr->next;
    dir *new = get_new_dir(fname);
    if (parent->sub_dir == NULL)
        parent->sub_dir = new;
    else
        ptr->next = new;
    new->is_dir = 1;
    return new;
}

void add_file(dir *parent, char fname[20])
{
    dir *ptr = parent->sub_dir;
    dir *prev = ptr;
    while (ptr != NULL && ptr->next != NULL)
        ptr = ptr->next;
    dir *new = get_new_dir(fname);
    if (parent->sub_dir == NULL)
        parent->sub_dir = new;
    else
        ptr->next = new;

    new->is_dir = 0;
}

void delete (dir *parent, char fname[20])
{
    dir *ptr = parent->sub_dir;
    dir *prev = NULL;
    int flag = 0;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, fname) == 0)
        {
            flag = 1;
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    if (flag)
    {
        if (prev != NULL)
        {
            prev->next = ptr->next;
        }
        else
        {
            parent->sub_dir = ptr->next;
        }
    }
    else
    {
        printf("\nFile/Directory not found!\n");
    }
}

void list_dir(dir *parent)
{
    dir *ptr = parent->sub_dir;
    printf("\n\nDir listing of %s:\n", parent->name);
    while (ptr != NULL)
    {
        if (ptr->is_dir)
        {
            printf("\n%s/", ptr->name);
        }
        else
        {
            printf("\n%s", ptr->name);
        }
        ptr = ptr->next;
    }
}

dir *change_dir(dir *parent, char fname[20])
{
    dir *ptr = parent->sub_dir;
    int flag = 0;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, fname) == 0)
        {
            flag = 1;
            break;
        }
        ptr = ptr->next;
    }
    if (flag)
    {
        return ptr;
    }
    else
    {
        printf("\nFile/Directory not found!\n");
        return NULL;
    }
}

int is_parent(dir *dir1, dir *dir2)
{
    dir *ptr = dir1->sub_dir;
    int flag = 0;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, dir2->name) == 0)
        {
            flag = 1;
            break;
        }
        ptr = ptr->next;
    }
    if (flag)
    {
        return 1;
    }
    else
    {
        printf("\nFile/Directory not found!\n");
        return 0;
    }
}

int is_unique(dir *parent, char fname[20])
{
    dir *ptr = parent->sub_dir;
    int flag = 0;
    while (ptr != NULL)
    {
        if (strcmp(ptr->name, fname) == 0)
        {
            flag = 1;
            break;
        }
        ptr = ptr->next;
    }
    if (flag)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void menu_single()
{
    int choice;
    char fname[20];
    while (1)
    {
        printf("\nMenu\n1. Add file\n2. Delete File\n3. List dir\n4. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter File Name: ");
            scanf("%s", fname);
            if (is_unique(root, fname))
                add_file(root, fname);
            else
                printf("File Already Exists");
            break;

        case 2:
            printf("Enter File Name: ");
            scanf("%s", fname);
            delete (root, fname);
            break;

        case 3:
            list_dir(root);
            break;

        case 4:
            return;

        default:
            printf("\nInvalid Choice!\n");
            break;
        }
    }
}

void menu_two_level()
{
    int choice;
    dir *temp = root->sub_dir;
    char fname[20];

    while (1)
    {
        printf("\nMenu\n1. Add User\n2. Delete User\n3. List Users\n4. Select User\n5. Add file\n6. Delete File\n7. List dir\n8. Exit\nEnter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("Enter User Name: ");
            scanf("%s", fname);
            if (is_unique(root, fname))
                add_file(root, fname);
            else
                printf("User Already Exists");
            break;

        case 2:
            printf("Enter User Name: ");
            scanf("%s", fname);
            delete (root, fname);
            break;

        case 3:
            list_dir(root);
            break;

        case 4:
            printf("Enter User Name: ");
            scanf("%s", fname);
            temp = change_dir(root, fname);
            break;

        case 5:
            if (temp == NULL)
            {
                printf("\nNo user selected!\n");
                break;
            }
            printf("Enter File Name: ");
            scanf("%s", fname);
            if (is_unique(temp, fname))
                add_file(temp, fname);
            else
                printf("File Already Exists");
            break;

        case 6:
            if (temp == NULL)
            {
                printf("\nNo user selected!\n");
                break;
            }
            printf("Enter File Name: ");
            scanf("%s", fname);
            delete (temp, fname);
            break;

        case 7:
            if (temp == NULL)
            {
                printf("\nNo user selected!\n");
                break;
            }
            list_dir(temp);
            break;

        case 8:
            return;

        default:
            printf("\nInvalid Choice!\n");
            break;
        }
    }
}

void main()
{
    root = (dir *)malloc(sizeof(dir));
    strcpy(root->name, "root");
    dir *temp;
    int choice;
    printf("\nMenu\n1. Single\n2. Two Level\n3. Heirarchial\n4. Exit\nEnter Method to use: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        menu_single();
        break;

    case 2:
        menu_two_level();
        break;

    case 3:
        add_file(root, "hello");
        add_file(root, "world");
        add_dir(root, "dir1");
        add_dir(root, "dir2");
        add_file(root, "myfile");
        list_dir(root);
        delete (root, "world");
        list_dir(root);
        temp = change_dir(root, "dir1");
        add_file(temp, "subfile");
        add_file(temp, "asdsda");
        add_dir(temp, "susus");
        list_dir(temp);
        delete (temp, "asdsda");
        list_dir(temp);
        temp = change_dir(temp, "susus");
        list_dir(temp);
        break;

    case 4:
        return;

    default:
        printf("\nInvalid Choice!\n");
        break;
    }
}


/*
*******************************************************
Output
*******************************************************

Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 1
Enter File Name: aaa

Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 1
Enter File Name: bbb

Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 1
Enter File Name: ccc

Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 3


Dir listing of root:

aaa
bbb
ccc
Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 2
Enter File Name: ccc

Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 3


Dir listing of root:

aaa
bbb
Menu
1. Add file
2. Delete File
3. List dir
4. Exit
Enter choice: 4

*************************************************************

Menu
1. Single
2. Two Level
3. Heirarchial
4. Exit
Enter Method to use: 2

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 1
Enter User Name: milan

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 4
Enter User Name: milan

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 1
Enter User Name: aaa

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 1
Enter User Name: aaa
User Already Exists
Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 7


Dir listing of milan:

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 3


Dir listing of root:

milan
aaa
Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 5
Enter File Name: bbb

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 5
Enter File Name: bbb
File Already Exists
Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 7


Dir listing of milan:

bbb
Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 5
Enter File Name: ccc

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 6
Enter File Name: bbb

Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 7


Dir listing of milan:

ccc
Menu
1. Add User
2. Delete User
3. List Users
4. Select User
5. Add file
6. Delete File
7. List dir
8. Exit
Enter choice: 8

************************************************************


Menu
1. Single
2. Two Level
3. Heirarchial
4. Exit
Enter Method to use: 3


Dir listing of root:

hello
world
dir1/
dir2/
myfile

Dir listing of root:

hello
dir1/
dir2/
myfile

Dir listing of dir1:

subfile
asdsda
susus/

Dir listing of dir1:

subfile
susus/

Dir listing of susus:
*/