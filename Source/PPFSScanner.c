

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


                        if(element_exist (setElementPath(file->d_name,source_path)))
                        {
                            if(shouldIdeleteElement(getStatFile(setElementPath(file->d_name,source_path)),getStatFile(setElementPath(file->d_name,destination_path))))
                            {
                                printf("dir: %s\n",file->d_name);
                                deleteAdditionalItems(setElementPath(file->d_name,source_path),setElementPath(file->d_name,destination_path),recurency);
                            }

                        }
                        else
                        {

                            deleteAdditionalItems(setElementPath(file->d_name,source_path),setElementPath(file->d_name,destination_path),recurency);
                             rmdir(setElementPath(file->d_name,destination_path));

                        }


                }

            }



        }
        else
        {
            
            if(!element_exist (setElementPath(file->d_name,source_path)) )
            {
                //delete file
                printf("file does not exist: %s\n",file->d_name);
            }
            else
            {

                if(shouldIdeleteElement(getStatFile(setElementPath(file->d_name,source_path)),getStatFile(setElementPath(file->d_name,destination_path))))
                {
                    //delete file
                    printf("file is old: %s\n",file->d_name);
                }
                else printf("file is good: %s\n",file->d_name);
            }
        }

    }
    closedir(dest);
}


void copyPasteElements(char* source_path, char* destination_path,int recurency,int threshold)
{
    DIR* src = opendir(source_path);
    struct dirent *file;

     while(file = readdir(src))
    {
        if(file->d_type==DT_DIR)
        {
            if(recurency == 1)
            {
                if( !( strcmp( file->d_name, "." ) == 0 || strcmp( file->d_name, "..") == 0 ) )
                {
                        if(!element_exist (setElementPath(file->d_name,destination_path)))
                        {
                            printf("creating dir: %s\n",file->d_name);
                            struct stat toGetModeT;
                            fstat(setElementPath(file->d_name,source_path),&toGetModeT);
                            mkdir(setElementPath(file->d_name,destination_path),toGetModeT.st_mode);
                            //create dir
                            copyPasteElements(setElementPath(file->d_name,source_path),setElementPath(file->d_name,destination_path),recurency,threshold);
                        }
                        else{
                            printf("dir exist: %s\n",file->d_name);
                        }

                }
            }
        }
        else
        {
            //if( !( strcmp( file->d_name, "." ) == 0 || strcmp( file->d_name, "..") == 0 ) )
            if(!element_exist (setElementPath(file->d_name,destination_path)) )
            {

                struct stat buf;
                fstat(setElementPath(file->d_name,source_path),&buf);

                if(threshold < buf.st_size)
                {
                    //copypasteBigFile()
                }
                else
                {
                    //copypastesmallfile();
                }
                printf("copying file: %s\n",file->d_name);
            }
            else
            {
                printf("file is good: %s\n",file->d_name);
            }

        }

    }
    closedir(src);
}
