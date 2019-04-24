//---------------------------------------------------------------------------
/*
	Copyright (C) 2019 Patrick Rećko, Paweł Krzywosz

	This source file is released under the MIT License.
	See LICENSE.md for full terms. This notice is not to be removed.
*/
//---------------------------------------------------------------------------
#pragma once
#include "PPFSPrerequisites.h"

/**
 * Macro wrappers around the syslog command to properly send the log to proper channel.
 */
#define LOGERR(...) syslog(LOG_ERR, __VA_ARGS__);
#define LOGINFO(...) syslog(LOG_INFO, __VA_ARGS__);
#define LOGNOTICE(...) syslog(LOG_NOTICE, __VA_ARGS__);
#define LOGFATAL(...) syslog(LOG_CRIT, __VA_ARGS__);
#define LOGDEBUG(...) syslog(LOG_DEBUG, __VA_ARGS__);

/**
 * Opens the system log for the program
 */
void startUpSysLog(void);

/**
 * Closes the system log for the program.
 */
void shutDownSysLog(void);

/**
 * Creates the child process from the master process.
 *
 * @note Calling this function will cause the master program to close.
 */
pid_t forkProcess(void);

/**
 * Closes the standard console
 */
void closeStandardConsole(void);

/**
 * Resets the main location where the program is running to the root directory
 */
void resetToMainDirectory(void);

/**
 * Switches the program to new session so that it wouldn't use the masters session.
 * @return ID of the new session
 */
pid_t switchToNewSession(void);

/**
 * Computes the MD5 hash of the file based on the given file
 * @param fileLocation File from which the hash should be computed.
 * @param out Where the final hash should be written.
 */
void computeMD5HashFromLoc(char* fileLocation, unsigned char* out);
/**
 * Converts the integer value to the string
 * @param value Value to convert
 * @param str String, where the output should be written.
 * @param base The numeric system that the string should represent the number.
 */
void itoa(int value, char* str, int base);