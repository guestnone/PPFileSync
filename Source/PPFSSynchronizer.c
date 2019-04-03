

#include "PPFSSynchronizer.h"

#include "PPFSPrerequisites.h"

#define MAX_SIZE 1048576

void copyLargeData(int input, int output)
{
	struct stat inputStats = {0};
	struct stat outputStats = {0};
	char *readMap, *writeMap;
	int offset = 0;

	fstat(input, &inputStats);
	fstat(output, &outputStats);
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
		if ((readMap = mmap((caddr_t) 0, bytes, PROT_READ, MAP_SHARED, input, offset)) == MAP_FAILED)
		{
			printf("mmap error : fps\n");
			exit(1);
		}
		if ((writeMap = mmap((caddr_t) 0, bytes, PROT_READ | PROT_WRITE, MAP_SHARED, output, offset)) == MAP_FAILED)
		{
			printf("mmap error : fpd\n");
			exit(1);
		}
		memcpy(readMap, writeMap, bytes);

		if ((munmap(readMap, bytes)) == -1)
		{
			printf("munmap error : src\n");
			exit(1);
		}
		if ((munmap(writeMap, bytes) == -1))
		{
			printf("munmap error : dst\n");
			exit(1);
		}
		offset = offset + bytes;
	}
}