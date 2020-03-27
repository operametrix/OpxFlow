#include "log.h"

unsigned char log_run_level = LOG_LEVEL_DEBUG;

const char * log_level_strings [] = {
    "NONE    ",
	"CRITICAL",
    "ERROR   ",
	"WARNING ",
	"INFO    ",
	"DEBUG   ",
};
