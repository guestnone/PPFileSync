//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#include "PPFSFileOps.h"
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"

/**
 * Copies the files using the copy_file_range kernel function (copies directly in kernel).
 * @param inputFd Source file descriptor to copy the files.
 * @param outputFd To which file descriptor the source should be written.
 */
void copyLargeData(int inputFd, int outputFd)
{
	struct stat inputStats = {0};
	struct stat outputStats = {0};

	fstat(inputFd, &inputStats);
	fstat(outputFd, &outputStats);
	int filesize = inputStats.st_size;
	loff_t len, ret;

	len = inputStats.st_size;
	do
	{
		ret = copy_file_range(inputFd, NULL, outputFd, NULL, len, 0);
		if (ret == -1)
		{
			LOGFATAL("copy_file_range failed, stopping...")
			printf("copy_file_range failed, stopping...\n");
			exit(EXIT_FAILURE);
		}

		len -= ret;
	} while (len > 0);

}

/**
 * Copies the files using the read/write Linux API commands.
 * @param inputFd Source file descriptor to copy the files.
 * @param outputFd To which file descriptor the source should be written.tFd
 */
void copySmallData(int inputFd, int outputFd)
{
	char buffer[1024];
	memset(buffer, '0', sizeof(buffer));
	int bytesRead;
	do
	{
		bytesRead = read(inputFd, buffer, sizeof(buffer));
		write(outputFd, buffer, bytesRead);
	}
	while (bytesRead == sizeof(buffer));
}

int copyDataFromPath(char *sourcePath, char *destPath, uint64_t fileSizeThreshold)
{
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int sourceFd = open(sourcePath, O_RDONLY, openMode);
	int destFd = open(destPath, O_RDWR | O_EXCL | O_CREAT, openMode);
	if (destFd == -1) // File may exist, try again without creation flag
	{
		destFd = open(destPath, O_RDWR, openMode);
		if (destFd == -1)
		{
			LOGFATAL("Destination file %s couldn't be open, stopping.", sourcePath)
			printf("Destination file %s couldn't be open, stopping.", sourcePath);
			exit(EXIT_FAILURE);
		}
	}
	copyDataFromFileDesc(sourceFd, destFd, fileSizeThreshold);
	closeFileDesc(sourceFd);
	closeFileDesc(destFd);
	return 0;
}

void copyDataFromFileDesc(int sourceFd, int destFd, uint64_t fileSizeThreshold)
{
	struct stat inputStats = {0};
	char filePathSource[PATH_MAX], filePathDest[PATH_MAX];
	char tmpFdPathSrc[PATH_MAX], tmpFdPathDest[PATH_MAX];
	char tmpFdAsStrSrc[2], tmpFdAsStrDest[2];
	itoa(sourceFd, tmpFdAsStrSrc, 10);
	itoa(destFd, tmpFdAsStrDest, 10);
	strcpy(tmpFdPathSrc, "/proc/self/fd/");
	strcpy(tmpFdPathDest, "/proc/self/fd/");
	strcat(tmpFdPathSrc, tmpFdAsStrSrc);
	strcat(tmpFdPathDest, tmpFdAsStrDest);
	fstat(sourceFd, &inputStats);

	readlink(tmpFdPathSrc, filePathSource, PATH_MAX);
	readlink(tmpFdPathDest, filePathDest, PATH_MAX);
	if (inputStats.st_size > fileSizeThreshold*1000000)
	{
		LOGINFO("Copying file %s to %s using copy_file_range...", filePathSource, filePathDest)
		copyLargeData(sourceFd, destFd);
	}
	else
	{
		LOGINFO("Copying file %s to %s using read/write...", filePathSource, filePathDest)
		copySmallData(sourceFd, destFd);
	}
}

int removeFile(char *path)
{
	return unlink(path);
}

int closeFileDesc(int fd)
{
	return close(fd);
}


