/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2020-01-15 02:49:21 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:06:07 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_UTILS_FUNC_EXCEPTION_H_
#define KBMS_UTILS_FUNC_EXCEPTION_H_

#include<exception>
#include<iostream>

namespace kbms {
    class FuncException : public std::exception {
    public:
        const char *what() const throw() {
            return "callback function is null";
        }
    };

}
#endif // KBMS_UTILS_FUNC_EXCEPTION_H_
