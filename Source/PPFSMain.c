#include "PPFSPrerequisites.h"
#include "PPFSBase.h"
#include "PPFSScanner.h"
#include "PPFSSynchronizer.h"

volatile bool gProgramRunning = true;
volatile bool gPerformScan = true;
volatile bool gDaemonize = true;

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
	LOGNOTICE("Daemon Started on pid=%d", sid)
}

void printErrorHelp()
{
	printf("Usage: PPFileSync [OPTIONS] -s [SOURCEDIR] -d [DESTDIR]\n");
}

void printFullHelp()
{
	printErrorHelp();
	printf("\nOptions:\n"
		"  -h                  Prints this help text and exit.\n"
        "  -s SOURCEDIR        Uses this source directory for synchronization.\n"
		"  -d DESTDIR          Uses this destination directory for synchronization.\n"
        "  -R                  All directories on the source will be synchronized instead of only files on root directory.\n"
		"  -t FILETHRESHOLD    Threshold in bytes where the files will use more faster, mmap-based copy interface.\n"
        "  -w TIME             Wait time (in minutes) between every synchronization.\n"
		"  -v                  verbose mode (don't daemonize)\n");
}

int main(int argc, char *argv[])
{
	char *source = NULL;
	char *destination = NULL;
	int sleepTime = 5 * 60;
	int recursive = 0;
	int threshold = 100;
	int argument = 0;

	if(argc < 2)
	{
		printErrorHelp();
		printf("Type PPFileSync -h to see a list of all options.\n");
		exit(EXIT_FAILURE);
	}

	while ((argument = getopt(argc, argv, "s:d:w:t:Rhv")) != -1)
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
			case 'v':
				gDaemonize = false;
				break;
			case 'h':
				printFullHelp();
				exit(EXIT_SUCCESS);
			default:
				printErrorHelp();
				printf("Type PPFileSync -h to see a list of all options.\n");
				exit(EXIT_FAILURE);
		}

	}

	if(destination == NULL && source == NULL)
	{
		printErrorHelp();
		printf("\nYou must provide both source and destination.\n");
		printf("Type PPFileSync -h to see a list of all options.\n");
		exit(EXIT_FAILURE);
	}
	if(source == NULL)
	{
		printErrorHelp();
		printf("\nYou must provide source directory with -s option\n");
		printf("Type PPFileSync -h to see a list of all options.\n");
		exit(EXIT_FAILURE);
	}
	if(destination == NULL)
	{
		printErrorHelp();
		printf("\nYou must provide destination directory with -d option\n");
		printf("Type PPFileSync -h to see a list of all options.\n");
		exit(EXIT_FAILURE);
	}


	signal(SIGTERM, exitHandler);
	signal(SIGQUIT, exitHandler);
	signal(SIGUSR1, forceHandler);
	if(gDaemonize)
		initDaemon(false);

	while (gProgramRunning)
	{
		int currSleepTime = sleepTime;
		if(gPerformScan)
		{
			LOGNOTICE("Performing Synchronization!")
			performSynchronization(source, destination, recursive, threshold);
			gPerformScan = false;
			LOGNOTICE("Synchronization Done! Sleeping...")
		}
		while (!gPerformScan)
		{
			sleep(1);
			--currSleepTime;
			if (currSleepTime < 0)
				gPerformScan = true;
			if (!gProgramRunning)
				break;
		}
	}
	LOGNOTICE("Program Ended!");
	shutDownSysLog();
	return 0;
}
