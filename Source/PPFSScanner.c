

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

                            int rmdirStatus =0;
                            rmdirStatus = rmdir(setElementPath(file->d_name,destination_path));
                             if(rmdirStatus==0)
                             {
                                time_t t = time(NULL);
                                struct tm tm = *localtime(&t);
                                LOGINFO("%d-%d-%d %d:%d:%d:directory doesnt exist in source folder. deleted directory succesfull: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);
                                //LOGINFO("abcdefghijk %d",11);
                             }
                             else
                             {
                                time_t t = time(NULL);
                                struct tm tm = *localtime(&t);
                                LOGINFO("%d-%d-%d %d:%d:%d: directory doesnt exist in source folder. couldnt delete directory: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);
                             }

                        }


                }

            }



        }
        else
        {

            if(!element_exist (setElementPath(file->d_name,source_path)) )
            {
                //delete file
                if(0/*deleted*/)
                {
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    LOGINFO("%d-%d-%d %d:%d:%d:file doesnt exist in source folder. succesfully deleted file: %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);
                }
                else
                {
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    LOGINFO("%d-%d-%d %d:%d:%d:file doesnt exist in source folder. couldnt delete file: %s \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);
                }
                printf("file does not exist: %s\n",file->d_name);
            }
            else
            {

                if(shouldIdeleteElement(getStatFile(setElementPath(file->d_name,source_path)),getStatFile(setElementPath(file->d_name,destination_path))))
                {
                    //delete file
                    if(0/*deleted*/)
                    {
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                         LOGINFO("%d-%d-%d %d:%d:%d: file is old. succesfully deleted file: %s  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);
                    }
                   else
                   {
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    LOGINFO("%d-%d-%d %d:%d:%d: file is old. couldnt delete file: %s  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);

                   }
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
                            int mkdirStatus = mkdir(setElementPath(file->d_name,destination_path),toGetModeT.st_mode);

                            if(mkdirStatus==0)
                            {
                                time_t t = time(NULL);
                                struct tm tm = *localtime(&t);
                                LOGINFO("%d-%d-%d %d:%d:%d: created directory: %s  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);

                            }
                            else
                            {
                                time_t t = time(NULL);
                                struct tm tm = *localtime(&t);
                                LOGINFO("%d-%d-%d %d:%d:%d: couldnt create directory: %s  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name);

                            }
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

                    if(0/*sucessful*/)
                    {
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        LOGINFO("%d-%d-%d %d:%d:%d: copied and pasted file: %s. size of file: %ld  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name,buf.st_size);

                    }
                    else
                    {
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        LOGINFO("%d-%d-%d %d:%d:%d: couldnt copy paste file: %s. size of file: %ld  \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,file->d_name,buf.st_size);

                    }
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
