#include "log.h"
#include "find.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

//The structure contains basic file infomation
struct FileInfo{
    char fileType[10];      //file type
    unsigned long size;     //file size
    char* fileName;         //file name
};

//Once this app is run, the default direction is set to "."
char currentDirectory[1000] = ".";

//change a direct to a position according to input.
//if input ",", currentDirectory will not be changed
//if input "..", this function trys to change to up directory. On failing, currentDirectory will not be changed.
//if input begin with "/", this function will use abs path, else this function will use relative path. On failing, currentDirectory will not be changed.
void changeDirectory(){
    char newDir[1000];
    printf("\r\nInput Directory:");
    scanf("%s", newDir);
    if(strcmp(newDir, "") == 0){
        return;
    }else if(strcmp(newDir, ".") == 0){
        return;
    }else if(strcmp(newDir, "..") == 0
             || strcmp(newDir, "../") == 0){
        char* q = currentDirectory + strlen(currentDirectory);
        while(q > currentDirectory){
            if(*q == '/'){
                *q = 0;
                break;
            }
            q--;
        }
        printf("changed to up layer:%s\r\n", currentDirectory);
    }else{
        DIR *dir;
        if(newDir[0] == '/'){
            if ((dir = opendir (newDir)) != NULL) {
                strcpy(currentDirectory, newDir);
                printf("Directory is changed to: %s\r\n", currentDirectory);
                closedir (dir);
            }else{
                printf("%s cannot be accessed.", newDir);
            }
        }else{
            char tmpDir[1000];
            strcpy(tmpDir, currentDirectory);
            if(newDir[strlen(newDir) - 1] == '/'){
                newDir[strlen(newDir) - 1] = 0;
            }
            strcat(tmpDir, "/");
            strcat(tmpDir, newDir);
            if ((dir = opendir (tmpDir)) != NULL) {
                strcpy(currentDirectory, tmpDir);
                printf("Directory is changed to: %s\r\n", currentDirectory);
                closedir (dir);
            }else{
                printf("%s cannot be accessed.", tmpDir);
            }
        }
    }

    system("stty raw");
    printf("Press any key to continue...");
    getchar();
    getchar();
    system("stty cooked");
}


//Clean memory by free each node of the fileInfo list.
void freeFileInfo(struct FileInfo* fileInfoPtr,const int num){
    for(int i = 0; i < num; i++){
        free(fileInfoPtr[i].fileName);
    }
    free(fileInfoPtr);
}

//read file infomation to a list of FileInfo
struct FileInfo* getFileInfo( int* num)
{
    *num = 0;
    DIR * dir;
    struct dirent *ent;
    int fileNum = 0;
    dir = opendir (currentDirectory);
    if (dir != NULL) { // if open the directory
        while ((ent = readdir(dir)) != NULL) { // read file one by one and store the files at what ent points to
            fileNum++;
        }
        closedir (dir);
    }
    dir = opendir (currentDirectory);
    if (fileNum > 0)
    {
        struct FileInfo* fPtr = malloc(fileNum * sizeof(struct FileInfo));
        int i = 0;
        while ((ent = readdir(dir)) != NULL) {
            struct stat st;
            stat(ent->d_name, &st);
            fPtr[i].fileName = malloc(strlen(ent->d_name));
            strcpy(fPtr[i].fileName, ent->d_name);
            if(ent->d_type == 4){
                strcpy(fPtr[i].fileType, "Directory");
            }else if(ent->d_type == 8){
                strcpy(fPtr[i].fileType, "Regular");
            }else if(ent->d_type == 10){
                strcpy(fPtr[i].fileType, "Symlink");
            }else{
                strcpy(fPtr[i].fileType, "Unknown");
            }
            fPtr[i].size = st.st_size % 1024;

            //printf("%-20s%-20d%s\n", fPtr[i].fileType, fPtr[i].size, fPtr[i].fileName);
            i++;

        }

        closedir (dir);
        *num = fileNum;
        return fPtr;
    }
    else
    {
        perror ("error for open directory.");
        return NULL;
    }
}

//output the FileInfo list.
void printFileInfo(const struct FileInfo* fileInfoPtr, const int num){
    printf ("%-20s%-20s%s\n", "File_Type", "Size (Kb)", "File_Name");
    for(int i = 0; i < num; i++){
        printf ("%-20s%-20d%s\n", fileInfoPtr[i].fileType, fileInfoPtr[i].size, fileInfoPtr[i].fileName);
    }
}

//The comparator of FileInfo(by name)
int sizeCompare(const struct FileInfo* f1Ptr, const struct FileInfo* f2Ptr)
{
    return (f1Ptr->size - f2Ptr->size);
}

//The comparator of Fileinfo(by size)
int nameCompare(const struct FileInfo* f1Ptr, const struct FileInfo* f2Ptr){
    return strcmp(f1Ptr->fileName, f2Ptr->fileName);
}

//list the file info by size
void listFileBysize(){
    struct FileInfo* fileInfoPtr = NULL;
    int num = 0;
    fileInfoPtr = getFileInfo(&num);
    qsort(fileInfoPtr, num, sizeof(struct FileInfo), sizeCompare);
    printFileInfo(fileInfoPtr, num);;
    freeFileInfo(fileInfoPtr, num);
}

//list the file info by name
void listFileByName(){
    struct FileInfo* fileInfoPtr = NULL;
    int num = 0;
    fileInfoPtr = getFileInfo(&num);
    qsort(fileInfoPtr, num, sizeof(struct FileInfo), nameCompare);
    printFileInfo(fileInfoPtr, num);
    freeFileInfo(fileInfoPtr, num);
}

//the entery of file explorer
void fileExplorer()
{
    char c;
    while(1){
        system("clear");

        printf("++++++++++++++++++++\n");
        printf("File Explorer\n");
        printf(" 1. File  List (sorted by name)\n");
        printf(" 2. File List (sorted by size)\n");
        printf(" 3. Change Directory\n");
        printf(" 4. Main Menu\n");
        printf("++++++++++++++++++++\n");

        printf("Selection:");

        system("stty raw");
        scanf("%c",&c);
        system("stty cooked");

        switch(c)
        {
        case '1':
            addLog("File  List (sorted by name) is selected.");
            listFileByName();
            printf("Press any key to continue...\r\n");
            system("stty raw");
            getchar();
            system("stty cooked");
            break;
        case '2':
            addLog("File List (sorted by size) is selected.");
            listFileBysize();
            printf("Press any key to continue...\r\n");
            system("stty raw");
            getchar();
            system("stty cooked");
            break;
        case '3':
            addLog("Change Directory is selected.");
            changeDirectory();
            break;
        case '4':
            return;
        default:
            printf("wrong input of selection. Do it again\n");
            addLog("wrong input of selection. Do it again\n");
            system("stty raw");
            getchar();
            system("stty cooked");
            break;
        }
    }
}

int main()
{
    realpath(".", currentDirectory);

    char c;
    while(1){
        system("clear");

        printf("++++++++++++++++++++\n");
        printf("Sys Sdmin Utility\n");
        printf(" 1. File  Explorer\n");
        printf(" 2. Find and Replace\n");
        printf(" 3. Show the Log\n");
        printf(" 4. Exit\n");
        printf("++++++++++++++++++++\n");

        printf("Selection:");

        system("stty raw");
        scanf("%c",&c);
        system("stty cooked");


        switch(c)
        {
        case '1':
            addLog("File Explorer is selected.");
            fileExplorer();
            break;
        case '2':
            addLog("Find and replace is selected.");
            runFindAndReplace();
            printf("Press any key to continue...\r\n");
            system("stty raw");
            getchar();
            getchar();
            system("stty cooked");
            break;
        case '3':
            addLog("Show the log is selected.");
            showLog();
            printf("Press any key to continue...\r\n");
            system("stty raw");
            getchar();
            system("stty cooked");
            break;
        case '4':
            addLog("Exit is selected.");
            printf("\r\n");
            return;
        default:
            printf("\r\nwrong input of selection. Do it again\n");
            addLog("wrong input of selection. Do it again.");
            printf("Press any key to continue...\r\n");
            system("stty raw");
            getchar();
            system("stty cooked");
        }
    }

}
