#ifndef LOGGING_H_
#define LOGGING_H_

/** 
 * log4cxx logger. If log4cxx is not installed, compilation will fail.
 *
 */


#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>


using namespace log4cxx;
using namespace log4cxx::helpers;

#define LOGGING_CONF_PATH "log4cxx.properties"
#define LOGGING_LOGGER_NAME "redelemeier-client-logger"



/// Each compilation unit including this file will have it's own instance of logger
static LoggerPtr logger(Logger::getLogger(LOGGING_LOGGER_NAME));






#endif
