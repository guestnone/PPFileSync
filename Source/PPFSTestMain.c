#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSSynchronizer.h"

//#define TEST_HASHING
#define TEST_COPY

int main()
{
#if defined(TEST_HASHING)
	char s[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/ping.png";
	char d[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/ping2.png";
	unsigned char sourceHash[MD5_DIGEST_LENGTH];
	unsigned char destHash[MD5_DIGEST_LENGTH];
	computeMD5HashFromLoc(s, sourceHash);
	computeMD5HashFromLoc(d, destHash);
	int result = memcmp(sourceHash, destHash, MD5_DIGEST_LENGTH);
	if(result == 0)
	{
		printf("OK!!!");
		return 0;
	}
	printf("NOK");
	return -1;
#endif

#if defined(TEST_COPY)
	char s[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/movo.mkv";
	char d[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/movietestcopy.mkv";
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	startUpSysLog();
	int sFd = open (s, O_RDONLY, openMode);
	int dFd = open (d, O_RDWR | O_EXCL | O_CREAT, openMode);
	copyDataFromFileDesc(sFd, dFd, 1000);
	closeFileDesc(sFd);
	closeFileDesc(dFd);
	shutDownSysLog();
#endif
}