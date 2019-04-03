//
// Created by guest_none on 03.04.19.
//

#pragma once

#define LOGERR(text) syslog(LOG_ERR, text);
#define LOGINFO(text) syslog(LOG_INFO, text);
#define LOGNOTICE(text) syslog(LOG_NOTICE, text);g
#define LOGFATAL(text) syslog(LOG_CRIT, text);

void startUpSysLog(void);
