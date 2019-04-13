//
// Created by guest_none on 03.04.19.
//

#pragma once

#define LOGERR(text) syslog(LOG_ERR, text);
#define LOGINFO(text) syslog(LOG_INFO, text);
#define LOGNOTICE(text) syslog(LOG_NOTICE, text);
#define LOGFATAL(text) syslog(LOG_CRIT, text);

inline void startUpSysLog(void);

inline void shutDownSysLog(void);

int daemonize();

pid_t forkProcess(void);

inline void closeStandardConsole(void);

inline void resetToMainDirectory(void);

pid_t switchToNewSession(void);