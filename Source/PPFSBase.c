
#include "PPFSBase.h"

#include "PPFSPrerequisites.h"

inline void startUpSysLog(void)
{
	openlog("PPFileSync", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
}

inline void shutDownSysLog(void)
{
	closelog();
}

pid_t forkProcess(void)
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		printf("ERROR: daemon couldn't be started!");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}
	return pid;
}

inline void closeStandardConsole(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

inline void resetToMainDirectory(void)
{
	if ((chdir("/")) < 0)
	{
		LOGFATAL("Couldn't change to root directory!")
		exit(EXIT_FAILURE);
	}
}

pid_t switchToNewSession(void)
{
	pid_t sid;
	sid = setsid();
	if (sid < 0)
	{
		LOGFATAL("Couldn't switch to new session!")
		exit(EXIT_FAILURE);
	}
	return sid;
}

