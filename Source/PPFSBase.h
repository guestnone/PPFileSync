//
// Created by guest_none on 03.04.19.
//

#pragma once
#include "PPFSPrerequisites.h"

#define LOGERR(...) syslog(LOG_ERR, __VA_ARGS__);
#define LOGINFO(...) syslog(LOG_INFO, __VA_ARGS__);
#define LOGNOTICE(...) syslog(LOG_NOTICE, __VA_ARGS__);
#define LOGFATAL(...) syslog(LOG_CRIT, __VA_ARGS__);

void startUpSysLog(void);

void shutDownSysLog(void);

pid_t forkProcess(void);

void closeStandardConsole(void);

void resetToMainDirectory(void);

pid_t switchToNewSession(void);