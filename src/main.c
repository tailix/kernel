#include "logger.h"

void main()
{
    logger_initialize();
    logger_info("Kernel initialization started.");
    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}
