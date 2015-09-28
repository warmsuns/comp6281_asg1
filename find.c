#include "find.h"
#include "stdio.h"

extern char currentDirectory[1000];
void runFindAndReplace()
{
    system("clear");
    char fileName[80];
    char textToFind[50];
    char textToReplace[50];
    char * filePtr;
    struct LineNode * LNHead;
    printf("File Name: ");
    scanf("%s",fileName);

    printf("\n Enter text to find: ");
    scanf("%s",textToFind);

    printf("\n Replace text with: ");
    scanf("%s",textToReplace);

    filePtr = readFile(fileName);
    if(filePtr){
        LNHead = extractFileContent(filePtr);
        findAndPrint(LNHead,textToFind);
        replaceAndPrint(LNHead,textToFind,textToReplace);
        freeMemory(LNHead);
    }
}

char* readFile(char* fileName)
{
    char * fPtr = NULL;
    char absFileName[2000];
    strcpy(absFileName, currentDirectory);
    strcat(absFileName, "/");
    strcat(absFileName, fileName);
    printf("\r\ntry to open:%s\r\n", absFileName);
    FILE* file = fopen(absFileName, "r");
    if(file == NULL)
    {
        perror("File doesn't exists\n");

    }else //get file contents and store them to the space pointed by buffer
    {
        fseek (file , 0 , SEEK_END);
        size_t fSize = ftell(file);// get the size of the file
        rewind (file);
        fPtr = (char*) calloc (fSize + 1, sizeof(char));
        if (fPtr == NULL)
        {
            perror ("error distributing space for file.");
        }else
        {
            size_t realSize = fread (fPtr, 1, fSize, file);
            if (realSize != fSize) // the size file points to not equal to file size;
            {
                free (fPtr);
                fPtr = NULL;
                perror ("error to get th file size.");
            }
            fPtr[fSize] = 0;
        }
        fclose (file);
    }
    return fPtr;

}

struct LineNode* extractFileContent(char* pText){
    struct LineNode* firstNode = calloc(1, sizeof(struct LineNode));
    struct LineNode* currentNode = firstNode;
    if(strlen(pText) > 0){
        char* lineStart, * lineEnd;
        lineStart = lineEnd = pText;
        while(*lineEnd){
            if(*lineEnd == '\n'){
                currentNode->next = calloc(1, sizeof(struct LineNode));
                currentNode = currentNode->next;
                currentNode->next = NULL;

                currentNode->lineText = calloc((lineEnd - lineStart) + 2, sizeof(char));
                memcpy(currentNode->lineText, lineStart, lineEnd - lineStart + 1);
                lineStart = lineEnd + 1;
            }
            ++lineEnd;
        }
    }
    return firstNode;
}


int equals(char* text1, char* text2, int text1Len)
{
    for(int i = 0; i < text1Len; i++){
        if(((*(text1 + i))!=(*(text2 + i)))){
            return 0;
        }
        return 1;
    }
}

void freeMemory(struct LineNode* lPtr){
    struct LineNode* currentLPtr = lPtr;
    while(currentLPtr)
    {
        if(currentLPtr->lineText)
            free(currentLPtr->lineText);

        currentLPtr = currentLPtr->next;
        free(lPtr);
        lPtr = currentLPtr;
    }
}

void findAndPrint(struct LineNode* lineNode, char* originWord){
    printf("OLD FILE:\n");
    while(lineNode){
        int hitPositionIndex = 0;
        int isThisLinePrinted = 0;
        if(lineNode->lineText){
            int offset = strlen(lineNode->lineText) - strlen(originWord);
            if(offset > 0){
                for(unsigned i = 0; i <= offset;){
                    if(equals(lineNode->lineText + i, originWord, strlen(originWord))){
                        lineNode->matchPosition[hitPositionIndex++] = i;
                        lineNode->matchCount++;
                        if(!isThisLinePrinted){
                            printf("%s", lineNode->lineText);
                            isThisLinePrinted = 1;
                        }
                        i += strlen(originWord);
                    }else{
                        i++;
                    }
                }
            }
        }

        lineNode = lineNode->next;
    }
}


void replaceAndPrint(struct LineNode* lineNode, char* originWord, char* replaceWord){
    printf("NEW FILE:\n");
    while(lineNode){
        if(lineNode->lineText){
            int outputTextStart, outputTextEnd;
            outputTextStart = outputTextEnd = 0;
            for(int i = 0; i < lineNode->matchCount; i++){
                outputTextEnd = lineNode->matchPosition[i];
                for(int j = outputTextStart; j < outputTextEnd; j++){
                    putchar(lineNode->lineText[j]);
                }

                printf("**%s**", replaceWord);
                outputTextStart = outputTextEnd + strlen(originWord);
            }
            printf("%s", lineNode->lineText + outputTextStart);
        }

        lineNode = lineNode->next;
    }
}
