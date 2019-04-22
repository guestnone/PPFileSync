
#include "PPFSSynchronizer.h"
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"

#define MAX_SIZE 1048576

void copyLargeData(int inputFd, int outputFd)
{
	struct stat inputStats = {0};
	struct stat outputStats = {0};
	char *readMap, *writeMap;
	int offset = 0;

	fstat(inputFd, &inputStats);
	fstat(outputFd, &outputStats);
	int filesize = inputStats.st_size;
	int bytes = MAX_SIZE;
	ftruncate(outputFd, filesize);
	lseek(outputFd, filesize - 1, SEEK_SET);


	if ((readMap = mmap((caddr_t) 0, bytes, PROT_READ, MAP_SHARED, inputFd, offset)) == MAP_FAILED)
	{
		LOGFATAL("mmap error : fps\n")
		exit(1);
	}
	if ((writeMap = mmap((caddr_t) 0, bytes, PROT_READ | PROT_WRITE, MAP_SHARED, outputFd, offset)) == MAP_FAILED)
	{
		LOGFATAL("mmap error : fpd\n")
		exit(1);
	}
	memcpy(readMap, writeMap, filesize);

	if ((munmap(readMap, bytes)) == -1)
	{
		LOGFATAL("munmap error : src\n")
		exit(1);
	}
	if ((munmap(writeMap, bytes) == -1))
	{
		LOGFATAL("munmap error : dst\n")
		exit(1);
	}
}

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

int openFile(char *path, FileMode mode)
{
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd;
	switch (mode)
	{
		case ReadOnly:
			fd = open(path, O_RDONLY, openMode);
			break;
		case WriteOnly:
			fd = open(path, O_WRONLY, openMode);
			break;
		case ReadWrite:
			fd = open(path, O_RDWR, openMode);
			break;
		case CreateWrite :
			fd = open(path, O_WRONLY | O_EXCL | O_CREAT, openMode);
			break;
	}
	if (fd == -1)
	{
		LOGERR("Couldn't open file %s", path)

		return -1;
	}
	return fd;
}

int copyDataFromPath(char *sourcePath, char *destPath, unsigned int fileSizeThreshold)
{
	int sourceFd = openFile(sourcePath, ReadOnly);
	int destFd = openFile(destPath, WriteOnly);
	copyDataFromFileDesc(sourceFd, destFd, fileSizeThreshold);
	closeFileDesc(sourceFd);
	closeFileDesc(destFd);
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


