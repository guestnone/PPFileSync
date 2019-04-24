//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSFileOps.h"

// Tu use the test code you need to select what test you want to use by
// leaving only one of the defines (all other are uncommented) and then recompiling the
// Test program.
// Crude and using a test framework is better idea but I(guest_none) am running out of time so here we go.
//#define TEST_HASHING
//#define TEST_COPY
#define TEST_BENCHMARK

int main()
{
	/// This code tests the hashing using the OpenSSL's MD5 implementation
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
	return 0;
#endif

	// This file benchmarks the file copying mechanism against files of various size and
	// copy type by measuring the time in which the code was called and returned.
	// Displays the results on the console
#if defined(TEST_BENCHMARK)
	time_t start, end;
	clock_t cpuStart, cpuStop;
	// Test files for copying.
	char* s10 = "bench/10mb.zip";
	char* d10 = "bench/10mb.zip.copy";
	char* s100 = "bench/100mb.zip";
	char* d100 = "bench/100mb.zip.copy";
	char* s1000 = "bench/1000mb.zip";
	char* d1000 = "bench/1000mb.zip.copy";
	char* s8000 = "bench/8000mb.zip";
	char* d8000 = "bench/8000mb.zip.copy";

	// 10 MB
	printf("10 MB\n");
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s10, d10, 10000);
	end = time(NULL);
	cpuStop = clock();
	printf("  Read-write: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d10);
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s10, d10, 5);
	end = time(NULL);
	cpuStop = clock();
	printf("  copy_file_range: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d10);

	// 100 MB
	printf("100 MB\n");
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s100, d100, 10000);
	end = time(NULL);
	cpuStop = clock();
	printf("  Read-write: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d100);
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s100, d100, 5);
	end = time(NULL);
	cpuStop = clock();
	printf("  copy_file_range: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d100);

	// 1000 MB
	printf("1000 MB\n");
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s1000, d1000, 10000);
	end = time(NULL);
	cpuStop = clock();
	printf("  Read-write: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d1000);
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s1000, d1000, 5);
	end = time(NULL);
	cpuStop = clock();
	printf("  copy_file_range: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d1000);

	// 8000 MB
	printf("8000 MB\n");
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s8000, d8000, 10000);
	end = time(NULL);
	cpuStop = clock();
	printf("  Read-write: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d8000);
	start = time(NULL);
	cpuStart = clock();
	copyDataFromPath(s8000, d8000, 5);
	end = time(NULL);
	cpuStop = clock();
	printf("  copy_file_range: Normal - %ld, CPU: %.2lf\n", end - start, (double) (cpuStop - cpuStart) / CLOCKS_PER_SEC);
	removeFile(d8000);

	return 0;

#endif
}