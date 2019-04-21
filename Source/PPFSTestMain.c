#include "PPFSPrerequisites.h"
#include "PPFSBase.h"

int main()
{
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
}