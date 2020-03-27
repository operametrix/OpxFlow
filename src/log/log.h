#ifndef __LOG_H__
#define __LOG_H__
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
enum {
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_CRITICAL = 1,
	LOG_LEVEL_ERROR = 2,
	LOG_LEVEL_WARNING = 3,
	LOG_LEVEL_INFO = 4,
	LOG_LEVEL_DEBUG = 5,
};
 
extern uint8_t log_run_level;
extern const char * log_level_strings[];
 
#ifndef LOG_FP
#ifdef stdout
#define LOG_FP stdout
#endif
#endif
 
#define LOG_SHOULD_I( level ) ( level <= log_run_level )

#define LOG(level, fmt, arg...) do {	\
	if ( LOG_SHOULD_I(level) ) { \
        fprintf(LOG_FP, "%s " fmt "\n", log_level_strings[level], ##arg); \
		fflush( LOG_FP ); \
	} \
} while(0)
 
#define LOG_DEBUG( fmt, arg... ) LOG( LOG_LEVEL_DEBUG, fmt, ##arg )
#define LOG_INFO( fmt,arg... ) LOG( LOG_LEVEL_INFO, fmt, ##arg )
#define LOG_WARNING( fmt, arg... ) LOG( LOG_LEVEL_WARNING, fmt, ##arg )
#define LOG_ERROR( fmt, arg... ) LOG( LOG_LEVEL_ERROR, fmt, ##arg )
#define LOG_CRITICAL( fmt, arg... ) LOG( LOG_LEVEL_CRITICAL, fmt, ##arg )

#endif
