/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-03-10 02:24:55 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-02 06:27:24 PM
 @Modified By: dongzhixiong
************************************************************************/
#include<sstream>

#include "utils/utils.h"
#include "args.h"

namespace kbms {
    Args::Args(const std::string &host, const std::string &port) : host_(host), port_(port) {

        //auxiliary setting
        http_prefix = "http:://";
        colon = ":";
        minus_sign = "-";
        slanting_bar = "/";

        //registry parans
        instance_id = "instanceId";
        app = "app";
        ip_addr = "ipAddr";
        status_page_url = "statusPageUrl";
        status_page_url_value = "info";

        health_check_url = "healthCheckUrl";
        health_check_url_value = "health";

        home_page_url = "homePageUrl";
        secure_vip_address = "secureVipAddress";
        vip_address = "vipAddress";

        at_class = "@class";
        at_class_value = "com.netflix.appinfo.InstanceInfo$DefaultDataCenterInfo";

        name = "name";
        name_value = "MyOwn";

        data_center_info = "dataCenterInfo";
        host_name = "hostName";
        sstatus = "status";
        renewal_interval_insecs = "renewalIntervalInSecs";
        duration_insecs = "durationInSecs";
        lease_info = "leaseInfo";
        dolar = "$";

        at_enabled = "@enabled";
        at_enabled_value = true;

        port_name = "port";
        instance = "instance";
        application = "application";
        //valid instance
        valid_status = "UP";

        //down seting
        down_tag = "status?value=DOWN";

        //set registry header
        registry_header = {
                {"Accept",          "*/*"},
                {"Accept-Encoding", "gzip, deflate"},
                {"Connection",      "keep-alive"},
                {"Content-Type",    "application/json"},
        };

        ser_header = {
                {"Accept", "application/json"},
        };
    }

    Args::~Args() {

    }

    std::string Args::GetUrl(std::string tail) const {
        std::string ret_str;
        std::vector<std::string> vs = {http_prefix, host_, colon, port_, slanting_bar, tail};
        Join(vs, ret_str);
        return ret_str;
    }

    std::string Args::GetUrl(const std::string ip, const std::string &port) const {
        std::string ret_str;
        std::vector<std::string> vs = {http_prefix, ip, colon, port};
        Join(vs, ret_str);
        return ret_str;
    }

}
