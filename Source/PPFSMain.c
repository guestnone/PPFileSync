#include "PPFSPrerequisites.h"

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

int main(void)
{
	printf("IT WORKS!!!");
	return 0;
}