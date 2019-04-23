//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
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

void computeMD5HashFromLoc(char *fileLocation, unsigned char *out)
{
	MD5_CTX c;
	char buf[512];
	ssize_t bytes;
	int fd = open(fileLocation, O_RDONLY);

	MD5_Init(&c);
	bytes = read(fd, buf, 512);

	while (bytes > 0)
	{
		MD5_Update(&c, buf, bytes);
		bytes = read(fd, buf, 512);
	}
	MD5_Final(out, &c);
	close(fd);
}

void strreverse(char *begin, char *end)
{
	char aux;
	while (end > begin)
		aux = *end, *end-- = *begin, *begin++ = aux;
}

void itoa(int value, char *str, int base)
{
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *wstr = str;
	int sign;
	// Validate base
	if (base < 2 || base > 35)
	{
		*wstr = '\0';
		return;
	}
	// Take care of sign
	if ((sign = value) < 0)
		value = -value;
	// Conversion. Number is reversed.
	do *wstr++ = num[value % base]; while (value /= base);
	if (sign < 0)
		*wstr++ = '-';
	*wstr = '\0';
	// Reverse string
	strreverse(str, wstr - 1);
}
