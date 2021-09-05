/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-03-16 05:25:28 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:07:54 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_UTILS_HELPERS_H_
#define KBMS_UTILS_HELPERS_H_

#include <string>
#include <cctype>
#include <algorithm>
#include <functional>

namespace kbms {

    namespace utils {

        typedef struct {
            const char *data;
            size_t length;
        } PutDataStruct;

// writedata callback function
        size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

// header callback function
        size_t HeaderCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

// read callback function
        size_t ReadCallback(void *ptr, size_t size, size_t nmemb, void *userdata);

// trim from start
        static inline std::string &ltrim(std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                            [](int c) { return !std::isspace(c); }));
            return s;
        }

// trim from end
        static inline std::string &rtrim(std::string &s) {
            s.erase(std::find_if(s.rbegin(), s.rend(),
                                 [](int c) { return !std::isspace(c); }).base(), s.end());
            return s;
        }

// trim from both ends
        static inline std::string &trim(std::string &s) {
            return ltrim(rtrim(s));
        }

    };

};

#endif  // KBMS_UTILS_HELPERS_H_
