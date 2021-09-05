/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2019-12-24 10:49:56 AM
 @Created By : dongzhixiong
 @Description: some params about accessing to netflix 
 @Last modified: 2021-08-03 11:17:05 AM
 @Modified By: dongzhixiong
************************************************************************/
#ifndef KBMS_AMS_ARGS_H_
#define KBMS_AMS_ARGS_H_

#include <unordered_map>

#include "utils/formats.h"

// define status code
#define  RC_SUCCESS 204
#define  RC_FAILURE 500
#define  SC_SUCCESS 200
#define  ST_FAILURE 404

namespace kbms {

    class Args {

    public:
        Args(const std::string &host, const std::string &port);

        virtual ~Args();

        //auxiliary setting
        std::string http_prefix;
        std::string colon;
        std::string minus_sign;
        std::string slanting_bar;

        //registry agreement
        std::string instance_id;
        std::string app;
        std::string ip_addr;
        std::string status_page_url;
        std::string status_page_url_value;

        std::string health_check_url;
        std::string health_check_url_value;

        std::string home_page_url;
        std::string secure_vip_address;
        std::string vip_address;

        std::string at_class;
        std::string at_class_value;

        std::string name;
        std::string name_value;

        std::string data_center_info;
        std::string host_name;
        std::string sstatus;
        std::string renewal_interval_insecs;
        std::string duration_insecs;
        std::string lease_info;
        std::string dolar;

        std::string at_enabled;
        bool at_enabled_value;

        std::string port_name;
        std::string instance;
        std::string application;
        //valid instance
        std::string valid_status;
        //down seting
        std::string down_tag;
        //registry header
        std::unordered_map<std::string, std::string> registry_header;
        //header of registry service list
        std::unordered_map<std::string, std::string> ser_header;
    public:
        std::string GetUrl(std::string tail = "") const;

        std::string GetUrl(const std::string ip, const std::string &port) const;

    private:
        std::string host_;
        std::string port_;

    };

}

#endif // KBMS_AMS_ARGS_H_
