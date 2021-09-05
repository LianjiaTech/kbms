/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : Sat 13 Jul 2019 04:59:41 PM CST
 @Created By : cuiming
 @Description:
 @Last modified: 2021-08-03 12:11:01 PM
 @Modified By: dongzhixiong
 ************************************************************************/
#ifndef KBMS_UTILS_UTILS_H_
#define KBMS_UTILS_UTILS_H_

#include <string>

#include "formats.h"

#define IP_SIZE     16
namespace kbms {
    bool StrToJson(const std::string &str, JsonDoc &json);

    bool JsonToStr(const JsonDoc &json, std::string &str);

    void GetLocalIp(char *ip);

    void Join(const std::vector<std::string> &vec, const std::string &delim, std::string &ret_str);

    void Join(const std::vector<std::string> &vec, std::string &ret_str);
}

#endif // KBMS_UTILS_UTILS_H_
