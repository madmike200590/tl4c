/**
 * logging.h - minimalistic logging framework
 *
 * @author Michael Langowski, e1426581@student.tuwien.ac.at
 * @date 10.01.2016
 *
 */

#ifndef LOGGING_H
#define LOGGING_H
#include <stdarg.h>

void set_loglevel(int level);

/**
 * Simple logging function for errors
 *
 * @param appname the application name - shows up at the beginning of a logged line
 * @param errmsg the error message to log
 *
 */
void print_err(const char* appname, char* errmsg);

void log_debug(const char* appname, char* msg, ...);

void log_info(const char* appname, char* msg, ...);

void log_warn(const char* appname, char* msg, ...);

void log_error(const char* appname, char* msg, ...);

void log_fatal(const char* appname, char* msg, ...);

void vlog_debug(const char* appname, char* msg, va_list msg_args);

void vlog_info(const char* appname, char* msg, va_list msg_args);

void vlog_warn(const char* appname, char* msg, va_list msg_args);

void vlog_error(const char* appname, char* msg, va_list msg_args);

void vlog_fatal(const char* appname, char* msg, va_list msg_args);

#endif /*LOGGING_H*/

