#ifndef KERNELMQ_INCLUDED_LOGGER
#define KERNELMQ_INCLUDED_LOGGER 1

#define logger_info_from(source, ...) logger_log(0, source, __VA_ARGS__)
#define logger_warn_from(source, ...) logger_log(1, source, __VA_ARGS__)
#define logger_fail_from(source, ...) logger_log(2, source, __VA_ARGS__)

#define logger_info(...) logger_info_from(0, __VA_ARGS__)
#define logger_warn(...) logger_warn_from(0, __VA_ARGS__)
#define logger_fail(...) logger_fail_from(0, __VA_ARGS__)

void logger_log(unsigned char level, const char *source, const char *format, ...);

#endif
