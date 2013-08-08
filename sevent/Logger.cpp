/*
 * =====================================================================================
 *
 *       Filename:  VSLogger.cpp
 *
 *    Description:  Impl
 *
 *        Version:  1.0
 *        Created:  05/02/2012 11:50:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sam wang (), gotostudy@gmail.com
 *        Company:  www.taotaosou.com
 *
 * =====================================================================================
 */
#include <stdio.h>

#include "Logger.h"

LoggerPtr VSLogger::logger = Logger::getRootLogger();
bool VSLogger::isInited = false;


    int
VSLogger::Init ( const char* logfile, const char* logname )
{
    if ( logfile == NULL || logname == NULL ) {
        return -1;
    }
    string strfile = logfile;
    string strname = logname;
    return VSLogger::Init(strfile, strname);
}		/* -----  end of method VSLogger::Init  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  VSLogger
 *      Method:  VSLogger :: Init
 * Description:  initialization. This must be called during the init phase to set proper logger value.
 *--------------------------------------------------------------------------------------
 */
    int
VSLogger::Init (const string& logFile, const string& logName)
{
    if ((logFile.empty()) || (logName.empty())) {
        return -1;
    }
    PropertyConfigurator::configure (logFile);
    VSLogger::logger = Logger::getLogger (logName);
    //sad that we cannot get info if sth wrong when log4cxx loads config.
    VSLogger::isInited = true;
    return 0;
}		/* -----  end of method VSLogger::Init  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  VSLogger
 *      Method:  VSLogger :: getLogger
 * Description:  
 *--------------------------------------------------------------------------------------
 */
    LoggerPtr&
VSLogger::getLogger ( )
{
    if ( !isInited ) {
        fprintf(stderr, "FATAL error: Logger not properly initialized.\n");
    }
    return VSLogger::logger;
}		/* -----  end of method VSLogger::getLogger  ----- */

