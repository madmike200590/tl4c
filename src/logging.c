#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "logging.h"

// max length of a logged line
#define MAX_LOG_LEN 200

#define STR_DEBUG "DEBUG"
#define STR_INFO "INFO"
#define STR_WARN "WARN"
#define STR_ERROR "ERROR"
#define STR_FATAL "FATAL"

#define APPNAME_LOGGING "logging"

//default value for level
int logging_level = 4;

static char* alloc_line(void);

static void print_log_line(const char* appname, int level, char* msg_format,
        va_list msg_args);

static char* alloc_line(void) {
    char* ret_val = NULL;
    if ((ret_val = malloc(sizeof(char) * MAX_LOG_LEN)) == NULL) {
        print_err("LOGGING-INTERNAL",
                "Internal logging-error, couldn't allocate string for output");
    }
    return ret_val;
}

void set_loglevel(int level) {
    if (level < TL4C_LEVEL_DEBUG) {
        logging_level = TL4C_LEVEL_DEBUG;
    } else if (level > TL4C_LEVEL_FATAL) {
        logging_level = TL4C_LEVEL_FATAL;
    } else {
        logging_level = level;
    }
}

void log_debug(const char* appname, char* msg, ...) {
    va_list msg_args;
    va_start(msg_args, msg);
    print_log_line(appname, TL4C_LEVEL_DEBUG, msg, msg_args);
    va_end(msg_args);
}

void log_info(const char* appname, char* msg, ...) {
    va_list msg_args;
    va_start(msg_args, msg);
    print_log_line(appname, TL4C_LEVEL_INFO, msg, msg_args);
    va_end(msg_args);
}

void log_warn(const char* appname, char* msg, ...) {
    va_list msg_args;
    va_start(msg_args, msg);
    print_log_line(appname, TL4C_LEVEL_WARN, msg, msg_args);
    va_end(msg_args);
}

void log_error(const char* appname, char* msg, ...) {
    va_list msg_args;
    va_start(msg_args, msg);
    print_log_line(appname, TL4C_LEVEL_ERROR, msg, msg_args);
    va_end(msg_args);
}

void log_fatal(const char* appname, char* msg, ...) {
    va_list msg_args;
    va_start(msg_args, msg);
    print_log_line(appname, TL4C_LEVEL_FATAL, msg, msg_args);
    va_end(msg_args);
}

void vlog_debug(const char* appname, char* msg, va_list msg_args) {
    print_log_line(appname, TL4C_LEVEL_DEBUG, msg, msg_args);
}

void vlog_info(const char* appname, char* msg, va_list msg_args) {
    print_log_line(appname, TL4C_LEVEL_INFO, msg, msg_args);
}

void vlog_warn(const char* appname, char* msg, va_list msg_args) {
    print_log_line(appname, TL4C_LEVEL_WARN, msg, msg_args);
}

void vlog_error(const char* appname, char* msg, va_list msg_args) {
    print_log_line(appname, TL4C_LEVEL_ERROR, msg, msg_args);
}

void vlog_fatal(const char* appname, char* msg, va_list msg_args) {
    print_log_line(appname, TL4C_LEVEL_FATAL, msg, msg_args);
}

static void print_log_line(const char* appname, int level, char* msg_format,
        va_list msg_args) {
    if (level < logging_level) {
        // don't print anything if log level is set higher than passed one
        return;
    }
    char* level_str;
    switch (level) {
        case TL4C_LEVEL_DEBUG:
            level_str = STR_DEBUG;
            break;
        case TL4C_LEVEL_INFO:
            level_str = STR_INFO;
            break;
        case TL4C_LEVEL_WARN:
            level_str = STR_WARN;
            break;
        case TL4C_LEVEL_ERROR:
            level_str = STR_ERROR;
            break;
        case TL4C_LEVEL_FATAL:
            level_str = STR_FATAL;
            break;
        default:
            log_error(APPNAME_LOGGING, "Log-level %d unknown", level);
    }
    char* line;
    if ((line = alloc_line()) == NULL) {
        log_error(APPNAME_LOGGING,
                "Could not allocate memory for log line string!");
        return;
    }
    // write user-provided message into buffer before final formatting and output
    int charsWritten = vsnprintf(line, MAX_LOG_LEN, msg_format, msg_args);
    if (errno != 0 && (level >= TL4C_LEVEL_ERROR)) {
        // if errno is set and level is according, include some error info into log output
        char* stderrmsg = strerror(errno);
        (void) fprintf(stderr, "%s %s: %s (errno=%d: %s)\n", level_str, appname,
                line, errno, stderrmsg);
    } else {
        (void) fprintf(stderr, "%s %s: %s\n", level_str, appname, line);
    }
    // release message buffer
    // TODO reuse same buffer!
    free(line);
    if (charsWritten > MAX_LOG_LEN) {
        log_warn(APPNAME_LOGGING, "WARN", "Log line was truncated by %d chars",
                (charsWritten - MAX_LOG_LEN));
    }
}

/**
 * Simple logging function for errors
 *
 * @param appname the application name - shows up at the beginning of a logged line
 * @param errmsg the error message to log
 *
 */
void print_err(const char* appname, char* errmsg) {
    char* stderrmsg = strerror(errno);
    (void) fprintf(stderr, "%s: ERROR %s (errno=%d: %s)\n", appname, errmsg,
            errno, stderrmsg);
}
