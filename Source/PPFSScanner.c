

#include "PPFSScanner.h"

#include "PPFSPrerequisites.h"

char* setFilePath(char *fileName,char* folderPath)
{

	int lenght =strlen(fileName) + strlen(folderPath)+1;

	char *filePath = (char*) malloc(lenght);

	strcpy(filePath,folderPath);
    strcat(filePath,"/");
	strcat(filePath,fileName);

	return filePath;
}
struct stat getStatFile(char* filePath)
{
	char buf[200];
	struct stat fileinfo;
	int data = stat(filePath,&fileinfo);
	return fileinfo;
}

int doesFileExist(const char* filename){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else // -1
        return 0;
}
