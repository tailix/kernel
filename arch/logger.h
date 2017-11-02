#ifndef KERNELMQ_INCLUDED_LOGGER
#define KERNELMQ_INCLUDED_LOGGER 1

void logger_initialize();
void logger_info(const char *s);
void logger_warn(const char *s);
void logger_fail(const char *s);

#endif
