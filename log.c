#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
void addLog(const char * logMessage)
{
    char timeBuffer[80];

    getTimeStr(timeBuffer, sizeof(timeBuffer));

    int newCapacity = strlen(timeBuffer) + strlen(logMessage) + logLength;

    if(newCapacity >= capacity){

        if(capacity==0)
        {
            capacity =1;
        }

         while(newCapacity >= capacity){

            capacity *= 2;

            capacity++; // extra position for NULL

         }

        char * newData = malloc(capacity);

        if(logLength > 0){ // there exists log data

            strcpy(newData, logData); // copy the previous data to newData

            free(logData); // free the space logData pointed

        }

        logData = newData; // make logData points to what newData points, which is the updated log data

    }



    strcpy(logData + logLength, timeBuffer); // change pointer to the next position of the end of previous log, and add time tag to logData

    logLength += strlen(timeBuffer);

    strcpy(logData + logLength, logMessage);// change pointer to the next position of the end of timelog, and add log message to logData

    logLength += strlen(logMessage);

}



void showLog()
{
    system("clear");
    printf("%s\r\n", logData);
}

void freeLog()
{
    free(logData);
    capacity = 0;
    logLength = 0;
}

void getTimeStr(char * timeBuffer, int size)
{
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (timeBuffer,size,"\nAt %c: ",timeinfo);
}
