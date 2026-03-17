#ifndef _RVM_LOGGER_H
#define _RVM_LOGGER_H

enum rvm_logger_level {
	RVM_LOGGER_INFO,
	RVM_LOGGER_DEBUG,
	RVM_LOGGER_WARN,
	RVM_LOGGER_ERROR,
	RVM_LOGGER_FATAL,
	RVM_LOGGER_TRACE
};

void rvm_log(enum rvm_logger_level level, const char *file, int line, const char *fmt, ...);

#ifndef NLOG
	#define rvm_info(...) rvm_log(RVM_LOGGER_INFO, __FILE__, __LINE__, __VA_ARGS__)
	#define rvm_debug(...) rvm_log(RVM_LOGGER_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
	#define rvm_warn(...) rvm_log(RVM_LOGGER_WARN, __FILE__, __LINE__, __VA_ARGS__)
	#define rvm_error(...) rvm_log(RVM_LOGGER_ERROR, __FILE__, __LINE__, __VA_ARGS__)
	#define rvm_fatal(...) rvm_log(RVM_LOGGER_FATAL, __FILE__, __LINE__, __VA_ARGS__)
	#define rvm_trace(...) rvm_log(RVM_LOGGER_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#else
	#define rvm_info(...)
	#define rvm_debug(...)
	#define rvm_warn(...)
	#define rvm_error(...)
	#define rvm_fatal(...)
	#define rvm_trace(...)
#endif

#endif
