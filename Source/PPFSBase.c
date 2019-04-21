
#include "PPFSBase.h"

#include "PPFSPrerequisites.h"

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

void startUpSysLog(void)
{
	openlog("PPFileSync", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
}

void shutDownSysLog(void)
{
	closelog();
}

void closeStandardConsole(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

void resetToMainDirectory(void)
{
	if ((chdir("/")) < 0)
	{
		LOGFATAL("Couldn't change to root directory!")
		exit(EXIT_FAILURE);
	}
}

void computeMD5HashFromLoc(char* fileLocation, unsigned char* out)
{
	MD5_CTX c;
	char buf[512];
	ssize_t bytes;
	int fd = open(fileLocation, O_RDONLY);
	//unsigned char out[MD5_DIGEST_LENGTH];

	MD5_Init(&c);
	bytes=read(fd, buf, 512);

	while(bytes > 0)
	{
		MD5_Update(&c, buf, bytes);
		bytes = read(fd, buf, 512);
	}
	MD5_Final(out, &c);
	close(fd);
}