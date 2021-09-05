/*************************************************************************
 @Copyright(c) 2017 Lianjia, Inc. All Rights Reserved
 @Created Time : 2021-03-16 05:25:28 PM
 @Created By : dongzhixiong
 @Description:
 @Last modified: 2021-08-03 12:08:15 PM
 @Modified By: dongzhixiong
************************************************************************/
#include <cstring>

#include "http_curl.h"
#include "helpers.h"

size_t kbms::utils::WriteCallback(void *data, size_t size,
                                  size_t nmemb, void *userdata) {
    Response *r;
    r = reinterpret_cast<Response *>(userdata);
    r->body.append(reinterpret_cast<char *>(data), size * nmemb);

    return (size * nmemb);
}

size_t kbms::utils::HeaderCallback(void *data, size_t size,
                                   size_t nmemb, void *userdata) {
    Response *r;
    r = reinterpret_cast<Response *>(userdata);
    std::string header(reinterpret_cast<char *>(data), size * nmemb);
    size_t seperator = header.find_first_of(':');
    if (std::string::npos == seperator) {
        trim(header);
        if (0 == header.length()) {
            return (size * nmemb);
        }
        r->headers[header] = "present";
    } else {
        std::string key = header.substr(0, seperator);
        trim(key);
        std::string value = header.substr(seperator + 1);
        trim(value);
        r->headers[key] = value;
    }

    return (size * nmemb);
}

size_t kbms::utils::ReadCallback(void *data, size_t size,
                                 size_t nmemb, void *userdata) {
    // get upload struct
    kbms::utils::PutDataStruct *u;
    u = reinterpret_cast<kbms::utils::PutDataStruct *>(userdata);
    // set correct sizes
    size_t curl_size = size * nmemb;
    size_t copy_size = (u->length < curl_size) ? u->length : curl_size;
    // copy data to buffer
    std::memcpy(data, u->data, copy_size);
    // decrement length and increment data pointer
    u->length -= copy_size;
    u->data += copy_size;
    // return copied size
    return copy_size;
}
