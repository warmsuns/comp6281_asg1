#include "stdio.h"
#ifndef LOG_H
#define LOG_H

static int logLength = 0;   //the real log size
static int capacity = 0;    //the capacity of log buffer
static char * logData;      //the log buffer pointer

//add msg to logData. Adjust the size by dynamic alloc is capacity is not enough.
void addLog(const char * logMessage);

//display the saved logData
void showLog();

//clean the log buffer
void freeLog();

//get current time to string buffer
void getTimeStr(char* buffer, int size);
#endif // LOG_H
