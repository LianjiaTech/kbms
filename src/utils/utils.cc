/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : Sat 13 Jul 2019 05:01:48 PM CST
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:12:06 PM
 @Modified By: dongzhixiong
 ************************************************************************/
#include <arpa/inet.h>
#include <errno.h>
#include <linux/sockios.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "utils.h"

namespace kbms {
    bool StrToJson(const std::string &str, JsonDoc &json) {
        if (str.empty())
            return false;
        ReaderBuilder builder;
        std::unique_ptr<JReader> parser(builder.newCharReader());
        std::string msg;
        return parser->parse(str.data(),
                             str.data() + str.length(),
                             &json, &msg);
    }

    bool JsonToStr(const JsonDoc &json, std::string &str) {

        std::ostringstream os;
        Json::StreamWriterBuilder builder;
        builder.settings_["indentation"] = "";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(json, &os);
        str = os.str();

        return true;
    }

    void Join(const std::vector<std::string> &vec, const std::string &delim, std::string &ret_str) {
        std::vector<std::string>::const_iterator itlast = vec.end() - 1;
        for (auto iter = vec.begin(); iter != vec.end(); iter++) {
            ret_str += *iter;
            if (iter != itlast) {
                ret_str += delim;
            }
        }
    }

    void Join(const std::vector<std::string> &vec, std::string &ret_str) {
        Join(vec, "", ret_str);
    }

    void GetLocalIp(char *ip) {

        int s;
        struct ifconf conf;
        struct ifreq *ifr;
        struct sockaddr_in sin;
        char buff[BUFSIZ];
        int num;
        int i;

        s = socket(PF_INET, SOCK_DGRAM, 0);
        conf.ifc_len = BUFSIZ;
        conf.ifc_buf = buff;

        ioctl(s, SIOCGIFCONF, &conf);
        num = conf.ifc_len / sizeof(struct ifreq);
        ifr = conf.ifc_req;

        for (i = 0; i < num; i++) {
            struct sockaddr_in *sin = (struct sockaddr_in *) (&ifr->ifr_addr);
            ioctl(s, SIOCGIFFLAGS, ifr);
            if (((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP)) {
                char *aa = inet_ntoa(sin->sin_addr);
                strcpy(ip, aa);
                return;
            }
            ifr++;
        }

    }
}
