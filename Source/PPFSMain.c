#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSScanner.h"
#include "PPFSSynchronizer.h"

volatile bool gProgramRunning = true;
volatile bool gPerformScan = true;

void exitHandler(int signum)
{
	gProgramRunning = false;
}

void forceHandler(int signum)
{
	gPerformScan = true;
}

void initDaemon(bool withPathReset)
{
	pid_t pid, sid;
	pid = forkProcess();
	umask(0);
	startUpSysLog();
	sid = switchToNewSession();
	if (withPathReset)
		resetToMainDirectory();
	LOGNOTICE("Daemon Started on pid=%d and sid=%d", pid, sid)
}

int main(int argc, char *argv[])
{
	char *source = NULL;
	char *destination = NULL;
	int sleepTime = 5 * 60;
	int recursive = 0;
	int threshold = 100;
	int argument = 0;
	while ((argument = getopt(argc, argv, "s:d:w:t:R")) != -1)
	{
		//source destination sleep(wait) recursive threshold
		switch (argument)
		{
			case 's':
				source = optarg;
//printf("source: %s\n",optarg);

				break;

			case 'd':
				destination = optarg;
//printf("destination: %s\n",optarg);
				break;

			case 'w':
				sleepTime = atoi(optarg);
//printf("sleep time: %s\n",optarg);
				break;

			case 'R':
				recursive = 1;
				printf("recursive\n");
				break;

			case 't':
				threshold = atoi(optarg);
//printf("threshold: %s\n",optarg);
				break;

		}

	}
	//return 0;
	printf("     -----BEGIN OF PROGRAM-----\n");
	while (gProgramRunning == true)
	{
		performSynchronization(source, destination, recursive);
		sleep(sleepTime);
	}

}
