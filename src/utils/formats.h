/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2019-12-24 10:49:56 AM
 @Created By : dongzhixiong
 @Description: define some data structure
 @Last modified: 2021-08-03 12:06:33 PM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_UTILS_FORMATS_H_
#define KBMS_UTILS_FORMATS_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "json/json.h"

namespace kbms {
    typedef Json::CharReaderBuilder ReaderBuilder;
    typedef Json::CharReader JReader;
    typedef Json::StreamWriterBuilder WriterBuilder;
    typedef Json::StreamWriter JWriter;
    typedef Json::Value JsonDoc;

    struct RegistryInfo {
        //if flag_of_mmserver_on=true,Required
        std::string url;
        std::string service_name;
        std::string version;

        // if flag_of_mmserver_on=true, not required
        //set header
        std::map<std::string, std::string> headers;

        // whether to access to micro services
        bool flag_of_mmserver_on;
        uint32_t timeout;
        uint32_t renewal_secs;
        uint32_t duration_sec;

        RegistryInfo() : flag_of_mmserver_on(true), timeout(100), renewal_secs(30), duration_sec(90) {}
    };

    struct ConfigInfo {
        // whether to access to config center
        bool flag_of_config_on;
        std::string url;
        //set header
        std::map<std::string, std::string> headers;
        uint32_t timeout;
        std::string username;
        std::string password;

        ConfigInfo() : timeout(50), flag_of_config_on(true) {}
    };

    //client request
    struct ReqBody {
        std::string service_name;                            //service name
        std::string tail_affix;                              //rul route
        char *request_body;                                  //body
        int32_t size;                                        //body size
        int32_t timeout;                                     //timeout(ms)
        struct {
            // authenticated user name
            std::string username;
            // authenticated user password
            std::string password;
        } basicAuth;
        std::map<std::string, std::string> req_headers;       // set request header
    };

    //client response
    struct ResBody {
        char *response;                                      //response
        int32_t size;                                        //body size
        std::map<std::string, std::string> res_headers;       //response header
        size_t code;                                         //status code
        std::string res_buf;
        std::string host;                                    //ip
    };

}

#endif // KBMS_UTILS_FORMATS_H_
