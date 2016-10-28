/*
 * Log.h
 *
 *  Created on: 28 Oct 2016
 *      Author: rag
 */

#ifndef LOG_H_
#define LOG_H_

#include "Logging.h"

#ifdef LOG_ON

#define LOG(...) Log.Debug(...)

#else

#define LOG(...)

#endif


#endif /* LOG_H_ */
