//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#include "PPFSScanner.h"
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSSynchronizer.h"

char *setFilePath(char *fileName, char *folderPath)
{
	int length = strlen(fileName) + strlen(folderPath) + 1;
	char *filePath = (char *) malloc(length);

	strcpy(filePath, folderPath);
	strcat(filePath, "/");
	strcat(filePath, fileName);

	return filePath;
}

struct stat getStatFile(char *filePath)
{
	struct stat fileinfo;
	int data = stat(filePath, &fileinfo);
	return fileinfo;
}

int doesFileExist(const char *filename)
{
	struct stat buffer;
	int exist = stat(filename, &buffer);
	if (exist == 0)
		return 1;
	else // -1
		return 0;
}

bool checkIfSameFile(char* source, char* destination)
{
	unsigned char sourceHash[MD5_DIGEST_LENGTH];
	unsigned char destHash[MD5_DIGEST_LENGTH];
	computeMD5HashFromLoc(source, sourceHash);
	computeMD5HashFromLoc(destination, destHash);
	if(memcmp(sourceHash, destHash, MD5_DIGEST_LENGTH) == 0)
		return true;
	return false;
}

bool checkIfNotInSource(char* source, char* destination)
{
	struct stat srcStat, destStat;
	if(stat(source, &srcStat) == -1 && stat(destination, &destStat) != -1)
		return true;
	return false;
}

void performSynchronization(char *source_path, char *destination_path, int recursive, int threshold)
{
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	DIR *dest = opendir(destination_path);
	struct dirent *file;
	printf("source: %s\ndestination: %s\n", source_path, destination_path);

	while (file = readdir(dest))
	{
		if (file->d_type == DT_DIR)
		{
			if (recursive == 1)
			{
				if (!(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0))
				{
					if (doesFileExist(setFilePath(file->d_name, source_path)))
					{
						if (checkIfSameFile(setFilePath(file->d_name, source_path),
												 setFilePath(file->d_name, destination_path)))
						{
							printf("dir: %s\n", file->d_name);
							performSynchronization(setFilePath(file->d_name, source_path),
												  setFilePath(file->d_name, destination_path), recursive, threshold);
						}
					}
					else
					{
						performSynchronization(setFilePath(file->d_name, source_path),
											  setFilePath(file->d_name, destination_path), recursive, threshold);
						int rmdirStatus = 0;
						rmdirStatus = rmdir(setFilePath(file->d_name, destination_path));
						if (rmdirStatus == 0)
						{
							
							LOGINFO("directory doesnt exist in source folder. deleted directory succesfull: %s\n",
									file->d_name)
							//LOGINFO("abcdefghijk %d",11);
						}
						else
						{
							LOGFATAL("directory doesnt exist in source folder. couldnt delete directory: %s\n",
									file->d_name)
						}
					}
				}
			}
		}
		else
		{
			if (!doesFileExist(setFilePath(file->d_name, source_path)))
			{
				int ret = removeFile(setFilePath(file->d_name, destination_path));
				if (ret == 0)
				{
					
					LOGINFO("file doesnt exist in source folder. succesfully deleted file: %s\n",
							file->d_name)
				}
				else
				{
					
					LOGFATAL("file doesnt exist in source folder. couldnt delete file: %s \n",
							file->d_name)
				}
				printf("file does not exist: %s\n", file->d_name);
			}
			else
			{

				if (checkIfNotInSource(setFilePath(file->d_name, source_path),
						               setFilePath(file->d_name, destination_path)))
				{
					int ret = removeFile(setFilePath(file->d_name, destination_path));
					if (ret == 0)
					{
						
						LOGINFO("file is old. succesfully deleted file: %s  \n",file->d_name);
					}
					else
					{
						
						LOGFATAL("file is old. couldnt delete file: %s  \n", file->d_name);

					}
					printf("file is old: %s\n", file->d_name);
				}
				else printf("file is good: %s\n", file->d_name);
			}
		}

	}
	closedir(dest);
}


void copyPasteElements(char *source_path, char *destination_path, int recursive, int threshold)
{
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	DIR *src = opendir(source_path);
	struct dirent *file;

	file = readdir(src);
	while (file)
	{
		if (file->d_type == DT_DIR)
		{
			if (recursive == 1)
			{
				if (!(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0))
				{
					if (!doesFileExist(setFilePath(file->d_name, destination_path)))
					{
						printf("creating dir: %s\n", file->d_name);
						struct stat toGetModeT;
						stat(setFilePath(file->d_name, source_path), &toGetModeT);
						int mkdirStatus = mkdir(setFilePath(file->d_name, destination_path), toGetModeT.st_mode);

						if (mkdirStatus == 0)
						{
							
							LOGINFO("created directory: %s  \n",  file->d_name)

						}
						else
						{
						
							LOGFATAL(" couldnt create directory: %s  \n", file->d_name)

						}
						//create dir
						copyPasteElements(setFilePath(file->d_name, source_path),
										  setFilePath(file->d_name, destination_path), recursive, threshold);
					}
					else
					{
						printf("dir exist: %s\n", file->d_name);
					}

				}
			}
		}
		else
		{
			if (!doesFileExist(setFilePath(file->d_name, destination_path)))
			{
				//int sourceFd = dirfd(src);
				int sourceFd = open(setFilePath(file->d_name, source_path), O_RDONLY, openMode);
				int destFd = open(setFilePath(file->d_name, destination_path), O_RDWR | O_EXCL | O_CREAT, openMode);
				if (destFd == -1) // File may exist, try again without creation flag
				{
					destFd = open(setFilePath(file->d_name, destination_path), O_RDWR, openMode);
					if (destFd == -1)
					{
						LOGFATAL("One of the files couldn't be open, stopping.")
						exit(EXIT_FAILURE);
					}
				}
				copyDataFromFileDesc(sourceFd, destFd, threshold);
				closeFileDesc(destFd);
				closeFileDesc(sourceFd);
				

				printf("copying file: %s\n", file->d_name);
			}
			else
			{
				printf("file is good: %s\n", file->d_name);
			}

		}

		file = readdir(src);
	}
	closedir(src);
}
