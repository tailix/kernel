#ifndef KERNELMQ_INCLUDED_LOGGER
#define KERNELMQ_INCLUDED_LOGGER 1

#define logger_info(...) logger_log(0, __VA_ARGS__)
#define logger_warn(...) logger_log(1, __VA_ARGS__)
#define logger_fail(...) logger_log(2, __VA_ARGS__)

void logger_log(unsigned char level, const char *format, ...);

#endif
