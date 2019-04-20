
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

bool checkIfSameFile(struct stat sourceStat, struct stat destinationStat)
{

	return true;
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
						if (checkIfSameFile(getStatFile(setFilePath(file->d_name, source_path)),
												 getStatFile(setFilePath(file->d_name, destination_path))))
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
							time_t t = time(NULL);
							struct tm tm = *localtime(&t);
							LOGINFO("%d-%d-%d %d:%d:%d:directory doesnt exist in source folder. deleted directory succesfull: %s\n",
									tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
									file->d_name)
							//LOGINFO("abcdefghijk %d",11);
						}
						else
						{
							time_t t = time(NULL);
							struct tm tm = *localtime(&t);
							LOGINFO("%d-%d-%d %d:%d:%d: directory doesnt exist in source folder. couldnt delete directory: %s\n",
									tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
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
					time_t t = time(NULL);
					struct tm tm = *localtime(&t);
					LOGINFO("%d-%d-%d %d:%d:%d:file doesnt exist in source folder. succesfully deleted file: %s\n",
							tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
							file->d_name)
				}
				else
				{
					time_t t = time(NULL);
					struct tm tm = *localtime(&t);
					LOGINFO("%d-%d-%d %d:%d:%d:file doesnt exist in source folder. couldnt delete file: %s \n",
							tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
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
						time_t t = time(NULL);
						struct tm tm = *localtime(&t);
						LOGINFO("%d-%d-%d %d:%d:%d: file is old. succesfully deleted file: %s  \n", tm.tm_year + 1900,
								tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, file->d_name);
					}
					else
					{
						time_t t = time(NULL);
						struct tm tm = *localtime(&t);
						LOGINFO("%d-%d-%d %d:%d:%d: file is old. couldnt delete file: %s  \n", tm.tm_year + 1900,
								tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, file->d_name);

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
							time_t t = time(NULL);
							struct tm tm = *localtime(&t);
							LOGINFO("%d-%d-%d %d:%d:%d: created directory: %s  \n", tm.tm_year + 1900, tm.tm_mon + 1,
									tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, file->d_name)

						}
						else
						{
							time_t t = time(NULL);
							struct tm tm = *localtime(&t);
							LOGINFO("%d-%d-%d %d:%d:%d: couldnt create directory: %s  \n", tm.tm_year + 1900,
									tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, file->d_name)

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
				int sourceFd = dirfd(src);
				int destFd = openFile(setFilePath(file->d_name, destination_path), WriteOnly);
				copyDataFromFileDesc(sourceFd, destFd, threshold);
				closeFileDesc(destFd);
				/*
				struct stat buf;
				fstat(setFilePath(file->d_name, source_path), &buf);

				if (threshold < buf.st_size)
				{
					//copypasteBigFile()
					//sucessful
					if (0)
					{
						time_t t = time(NULL);
						struct tm tm = *localtime(&t);
						LOGINFO("%d-%d-%d %d:%d:%d: copied and pasted file: %s. size of file: %ld  \n",
								tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
								file->d_name, buf.st_size);

					}
					else
					{
						time_t t = time(NULL);
						struct tm tm = *localtime(&t);
						LOGINFO("%d-%d-%d %d:%d:%d: couldnt copy paste file: %s. size of file: %ld  \n",
								tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
								file->d_name, buf.st_size);

					}
				}
				else
				{
					//copypastesmallfile();
				}
				*/

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
