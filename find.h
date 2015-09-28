#ifndef FIND
#define FIND

//LineNode is the structure contains a paragraph
//It also saves the key words' occurence and their positions
struct LineNode
{
    int matchPosition[100]; //key words' positions. Only support first 100 of each line.
    int matchCount;         //the key word counter
    char * lineText;        //the content of a line
    struct LineNode * next; //point to next line
};

//The entry of find and replace
void runFindAndReplace();

//read a text file to memory and return the point of the text block
char* readFile(char* fileName);

//analyze the content of the text block, and split it to lines
struct LineNode* extractFileContent(char* fPtr);

//for comparing the first n bytes of to strings
int equals(char* text1, char* text2, int length);

//delete the node of each line
void freeMemory(struct LineNode* lPtr);

//find the occurence of each key words and fill them in each node
void findAndPrint(struct LineNode* lineNode, char* originWord);

//according to key word match positions, replace with another words
void replaceAndPrint(struct LineNode* lineNode, char* originWord, char* replaceWord);

#endif // FIND

