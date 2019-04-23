//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSSynchronizer.h"

// Tu use the test code you need to select what test you want to use by
// leaving only one of the defines (all other are uncommented) and then recompiling the
// Test program.
// Crude and using a test framework is better idea but I(guest_none) am running out of time so here we go.
//#define TEST_HASHING
#define TEST_COPY

int main()
{
	/// This Test program tests the hashing using the OpenSSL's MD5 implementation
	/// by comparing two files.
	/// Prints OK if two files are matching, NOK if they're not.
#if defined(TEST_HASHING)
	// First file to compute hash.
	char s[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/ping.png";
	// Second file to compute hash.
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

	// This code tests the file copying mechanisms. It creates the file on the
#if defined(TEST_COPY)
	// Source file to duplicate
	char s[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/movo.mkv";
	// Destination file where the file should be copied.
	char d[] = "/home/guest_none/DevelopM/Private/guest_none/school/os/task_4/test/a/movietestcopy.mkv";
	mode_t openMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	startUpSysLog();
	copyDataFromPath(s, d, 100);
	shutDownSysLog();
#endif
}