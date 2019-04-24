//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#include "PPFSSynchronizer.h"
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSFileOps.h"

void setFilePath(char *fileName, char *folderPath, char* out)
{

	strcpy(out, folderPath);
	strcat(out, "/");
	strcat(out, fileName);

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
					char sPath[PATH_MAX];
					char dPath[PATH_MAX];
					setFilePath(file->d_name, source_path, sPath);
					setFilePath(file->d_name, destination_path, dPath);
					if (doesFileExist(sPath))
					{
						if (checkIfSameFile(sPath, dPath))
						{
							printf("dir: %s\n", file->d_name);
							performSynchronization(sPath, dPath, recursive, threshold);
						}
					}
					else
					{
						performSynchronization(sPath, dPath, recursive, threshold);
						int rmdirStatus = 0;
						rmdirStatus = rmdir(dPath);
						if (rmdirStatus == 0)
						{
							LOGINFO("directory doesnt exist in source folder. deleted directory succesfully: %s", dPath)
						}
						else
						{
							LOGFATAL("directory doesnt exist in source folder. couldn't delete directory: %s", dPath)
							exit(EXIT_FAILURE);
						}
					}

				}
			}
		}
		else
		{
			char sPath[PATH_MAX];
			char dPath[PATH_MAX];
			setFilePath(file->d_name, source_path, sPath);
			setFilePath(file->d_name, destination_path, dPath);
			if (!doesFileExist(sPath))
			{
				int ret = removeFile(dPath);
				if (ret == 0)
				{
					LOGINFO("file doesnt exist in source folder. succesfully deleted file: %s",
							dPath)
				}
				else
				{
					LOGFATAL("file doesnt exist in source folder. couldnt delete file: %s", dPath)
					exit(EXIT_FAILURE);
				}
				printf("file does not exist: %s\n", file->d_name);
			}
			else
			{

				if (checkIfNotInSource(sPath, dPath))
				{
					int ret = removeFile(dPath);
					if (ret == 0)
					{
						LOGINFO("file doesnt exist in source folder. successfuly deleted file: %s",dPath);
					}
					else
					{
						LOGFATAL("file doesnt exist in source folder. couldn't delete file: %s", dPath);
						exit(EXIT_FAILURE);
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
					char sPath[PATH_MAX];
					char dPath[PATH_MAX];
					setFilePath(file->d_name, source_path, sPath);
					setFilePath(file->d_name, destination_path, dPath);
					if (!doesFileExist(dPath))
					{
						printf("creating dir: %s\n", file->d_name);
						struct stat toGetModeT;
						stat(sPath, &toGetModeT);
						int mkdirStatus = mkdir(dPath, toGetModeT.st_mode);

						if (mkdirStatus == 0)
						{
							LOGINFO("Created directory: %s", dPath)
						}
						else
						{
							LOGFATAL("couldn't create directory: %s, Stopping...", dPath)
							exit(EXIT_FAILURE);
						}
						//create dir
						copyPasteElements(sPath, dPath, recursive, threshold);
					}
					else
					{
						
						printf("dir exist: %s\n", file->d_name);
						copyPasteElements(sPath,dPath,recursive,threshold);
					}
				}
			}
		}
		else
		{
			char sPath[PATH_MAX];
			char dPath[PATH_MAX];
			setFilePath(file->d_name, source_path, sPath);
			setFilePath(file->d_name, destination_path, dPath);
			if (!doesFileExist(dPath))
			{
				LOGINFO("Creating file in destination: %s", dPath)
				int sourceFd = open(sPath, O_RDONLY, openMode);
				int destFd = open(dPath, O_RDWR | O_EXCL | O_CREAT, openMode);
				if (destFd == -1) // File may exist, try again without creation flag
				{
					destFd = open(dPath, O_RDWR, openMode);
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
				if(!checkIfSameFile(sPath, dPath))
				{
					LOGINFO("Updating file: %s", dPath)
					int sourceFd = open(sPath, O_RDONLY, openMode);
					int destFd = open(dPath, O_RDWR | O_EXCL | O_CREAT, openMode);
					if (destFd == -1) // File may exist, try again without creation flag
					{
						destFd = open(dPath, O_RDWR, openMode);
						if (destFd == -1)
						{
							LOGFATAL("One of the files couldn't be open, stopping.")
							exit(EXIT_FAILURE);
						}
					}
					copyDataFromFileDesc(sourceFd, destFd, threshold);
					closeFileDesc(destFd);
					closeFileDesc(sourceFd);
					printf("file Changed: %s\n", file->d_name);

				}
				else
					printf("file is good: %s\n", file->d_name);
			};
		}

		file = readdir(src);
	}
	closedir(src);
}
