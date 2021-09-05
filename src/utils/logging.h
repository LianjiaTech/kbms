/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-01-07 02:30:21 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:10:04 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_UTILS_LOGGING_H_
#define KBMS_UTILS_LOGGING_H_

#define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES


#ifdef GOOGLE_STRIP_LOG

#include "glog/logging.h"
#include "glog/raw_logging.h"

#if GOOGLE_STRIP_LOG == 0

#define LOGGER_TRACE LOG(INFO)
#define LOGGER_DEBUG LOG(INFO)
#define LOGGER_INFO  LOG(INFO)
#define LOGGER_ERROR  LOG(INFO)
#define DLOGGER_TRACE LOG(INFO) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else

#define LOGGER_TRACE if (false) LOG(INFO)
#define LOGGER_DEBUG if (false) LOG(INFO)
#define LOGGER_INFO  if (false) LOG(INFO)
#define LOGGER_ERROR  if (false) LOG(INFO)
#define DLOGGER_TRACE if (false) LOG(INFO)
#endif


#else

#include<iostream>

#define LOGGER_TRACE std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOGGER_DEBUG std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOGGER_INFO  std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOGGER_WARN  std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOGGER_ERROR std::cout << __FILE__ << ":" << __LINE__ << " "
#define LOGGER_FATAL std::cout << __FILE__ << ":" << __LINE__ << " "
#define CHECK_NOTnullptr(val) LOG_ERROR << "'" #val "' Must be non nullptr";
#endif

#endif // KBMS_UTILS_LOGGING_H_
