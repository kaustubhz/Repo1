#define MAXINODE 50
#define READ 1
#define WRITE 2
#define MAXFILESIZE 1024
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct superblock
{
    int TotalInodes;
    int FreeInodes;
} SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FIleSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int Permission;
    struct inode *next;
} INODE, *PINODE, **PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE ptrinode;
} FILETABLE, *PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
} UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

void InitialiseSuperBlock()
{
    int i = 0;
    while (i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }
    SUPERBLOCKobj.FreeInodes = SUPERBLOCKobj.TotalInodes = MAXINODE;
}

void CreateDILB()
{
    PINODE newn = NULL;
    PINODE temp = head;
    int i = 1;

    while (i <= MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));
        newn->LinkCount = newn->ReferenceCount = 0;
        newn->Buffer = NULL;
        newn->next = NULL;
        newn->InodeNumber = i;
        if (temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
        i++;
    }
}

void ls_file()
{
    PINODE temp = head;
    if (SUPERBLOCKobj.FreeInodes == MAXINODE)
    {
        printf("ERROR: There are no files\n");
        return;
    }

    printf("\nFile name\tInode number\tFile size\tLink count\n");
    printf("-------------------------------------------------------------------------------------\n");
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\n", temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
        }
        temp = temp->next;
    }
    printf("-------------------------------------------------------------------------------------\n");
}

int main()
{
    char *ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];
    InitialiseSuperBlock();
    CreateDILB();

    while (1)
    {
        fflush(stdin);
        strcpy(str, "");
        printf("\nMarvellous VFS: >");
        fgets(str, 80, stdin);

        count = sscanf(str, "%s%s%s%s", command[0], command[1], command[2], command[3]);

        if (count == 1)
        {
            if (strcasecmp(command[0], "ls") == 0)
            {
                ls_file();
            }
            else if(strcasecmp(command[0],"cls")==0)
            {
                system("clear");
                continue;
            }
        }
    }
    return 0;
}