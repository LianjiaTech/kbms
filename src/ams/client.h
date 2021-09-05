/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-02-25 05:32:02 PM
 @Created By : dongzhixiong
 @Description: request other micro service
 @Last modified: 2021-08-03 11:19:11 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_AMS_CLIENT_H_
#define KBMS_AMS_CLIENT_H_

#include "utils/formats.h"

namespace kbms {
    class Client {
    public:
        Client();

        virtual ~Client();

        //request other micro service(type=Post or Get)
        virtual void Request(const std::string &type, const ReqBody &req_body, ResBody &res_body) const;

        //default post
        virtual void Request(const ReqBody &req_body, ResBody &res_body) const;

    private:

        class Impl;

        Impl *impl_;
    };

}
#endif // KBMS_AMS_CLIENT_H_
