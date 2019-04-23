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

#define MAX_SIZE 1048576
/**
 * Copies the files using the memory mapping (mmap).
 * @param inputFd Source file descriptor to copy the files.
 * @param outputFd To which file descriptor the source should be written.
 *
 * @note On 32-bit version this only supports 4GB files, This is due of us mapping the entire file and not the
 *       parts of it, which should be a better idea. L1imit AFAIK on 64-bit ones in our use is not noticeable
 *       so I'm leaving as it is.
 */
void copyLargeData(int inputFd, int outputFd)
{
	struct stat inputStats = {0};
	struct stat outputStats = {0};
	char *readMap, *writeMap;

	fstat(inputFd, &inputStats);
	fstat(outputFd, &outputStats);
	int filesize = inputStats.st_size;
	int bytes = MAX_SIZE;
	ftruncate(outputFd, filesize);
	lseek(outputFd, filesize - 1, SEEK_SET);


	if ((readMap = mmap((caddr_t) 0, filesize, PROT_READ, MAP_SHARED, inputFd, 0)) == MAP_FAILED)
	{
		LOGFATAL("Couldn't mmap source descriptor, stopping.\n")
		exit(EXIT_FAILURE);
	}
	if ((writeMap = mmap((caddr_t) 0, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, outputFd, 0)) == MAP_FAILED)
	{
		LOGFATAL("Couldn't mmap destination descriptor, stopping.\n")
		exit(EXIT_FAILURE);
	}
	memcpy(writeMap, readMap, filesize);

	if ((munmap(readMap, filesize)) == -1)
	{
		LOGFATAL("Couldn't unmap source descriptor from memory, stopping.\n")
		exit(EXIT_FAILURE);
	}
	if ((munmap(writeMap, filesize) == -1))
	{
		LOGFATAL("Couldn't unmap destination descriptor from memory, stopping.\n")
		exit(EXIT_FAILURE);
	}
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

int copyDataFromPath(char *sourcePath, char *destPath, unsigned int fileSizeThreshold)
{
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int sourceFd = open(sourcePath, O_RDONLY, openMode);
	int destFd = open(destPath, O_RDWR | O_EXCL | O_CREAT, openMode);
	if (destFd == -1) // File may exist, try again without creation flag
	{
		destFd = open(destPath, O_RDWR, openMode);
		if (destFd == -1)
		{
			LOGFATAL("Destination file couldn't be open, stopping.")
			exit(EXIT_FAILURE);
		}
	}
	copyDataFromFileDesc(sourceFd, destFd, fileSizeThreshold);
	closeFileDesc(sourceFd);
	closeFileDesc(destFd);
	return 0;
}

void copyDataFromFileDesc(int sourceFd, int destFd, unsigned int fileSizeThreshold)
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
		LOGINFO("Copying file %s to %s using mmap...", filePathSource, filePathDest)
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


