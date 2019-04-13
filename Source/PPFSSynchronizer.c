
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

	while (filesize > 0)
	{
		if (filesize < bytes)
		{
			bytes = filesize;
			filesize = 0;
		}
		else
		{
			filesize -= bytes;
		}
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
		memcpy(readMap, writeMap, bytes);

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
		offset = offset + bytes;
	}
}

void copySmallData(int inputFd, int outputFd)
{
	char buffer[1];
	int bytesRead;
	do
	{
		bytesRead = read(inputFd, buffer, sizeof(buffer));
		write(outputFd, buffer, sizeof(buffer);
	} while(bytesRead == sizeof(buffer));
}

int openFile(char* path, FileMode mode)
{ 
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
	int fd;
	switch (mode)
	{
	case ReadOnly:
		fd = open (path, O_WRONLY | O_EXCL | O_CREAT, mode);
		break;
	case WriteOnly:
		fd = open (path, O_RDONLY | O_EXCL | O_CREAT, mode);
		break;
	case ReadWrite:
		fd = open (path, O_RDWR | O_EXCL | O_CREAT, mode);
		break;
	}
	if (fd == -1) {
		LOGERR("Couldn't open file %s", path)
		return -1;
	}
	return fd;
}

inline int removeFile(char* path)
{
	return unlink(path);
}

inline int closeFileDesc(int fd)
{
	return close(fd);
}

int copyDataFromPath(char* sourcePath, char* destPath)
{
	int sourceFd = openFile(sourcePath, ReadOnly);
	int destFd = openFile(sourcePath, WriteOnly);
	copyDataFromFileDesc(sourceFd, destFd);
	closeFileDesc(sourceFd);
	closeFileDesc(destFd);
}

int copyDataFromFileDesc(int sourceFd, int destFd, unsigned int fileSizeThreshold)
{
	struct stat inputStats = {0};
	fstat(sourceFd, &inputStats);
	if (inputStats.st_size > fileSizeThreshold)
		copyLargeData(sourceFd, destFd);
	else
		copySmallData(sourceFd, destFd);
}