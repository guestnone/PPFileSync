

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

void deleteAdditionalItems(char* source_path, char* destination_path,int recurency)
{

    //  TO DO: checking modified date EDIT: DONE
    //DIR* src  = opendir(source_path);
    DIR* dest = opendir(destination_path);
    struct dirent *file;
    printf("source: %s\ndestination: %s\n",source_path,destination_path);
    while(file = readdir(dest))
    {
        if(file->d_type==DT_DIR)
        {
            if(recurency == 1)
            {
                if( !( strcmp( file->d_name, "." ) == 0 || strcmp( file->d_name, "..") == 0 ) )
                {


                        if(file_exist (setFilePath(file->d_name,source_path)))
                        {
                            if(shouldIdeleteElement(getStatFile(setFilePath(file->d_name,source_path)),getStatFile(setFilePath(file->d_name,destination_path))))
                            {
                                printf("dir: %s\n",file->d_name);
                                deleteAdditionalItems(setFilePath(file->d_name,source_path),setFilePath(file->d_name,destination_path),recurency);
                            }

                        }
                        else
                        {
                            //delete_dir
                        }


                }
            }



        }
        else
        {
            //if( !( strcmp( file->d_name, "." ) == 0 || strcmp( file->d_name, "..") == 0 ) )
            if(!file_exist (setFilePath(file->d_name,source_path)) )
            {
                //delete file
                printf("file does not exist: %s\n",file->d_name);
            }
            else
            {

                if(shouldIdeleteElement(getStatFile(setFilePath(file->d_name,source_path)),getStatFile(setFilePath(file->d_name,destination_path))))
                {
                    printf("file is old: %s\n",file->d_name);
                }
                else printf("file is good: %s\n",file->d_name);
            }
        }

    }
    closedir(dest);
}
