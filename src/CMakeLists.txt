#include dir
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/curl-7.28.0/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/libevent-2.1.11/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/glog0.3.4/include")
include_directories("${CMAKE_SOURCE_DIR}/src/")
include_directories("${CMAKE_SOURCE_DIR}/src/utils")
include_directories("${CMAKE_SOURCE_DIR}/src/concurrency")
include_directories("${CMAKE_SOURCE_DIR}/src/ams")
include_directories("${CMAKE_SOURCE_DIR}/src/config")
include_directories("${CMAKE_SOURCE_DIR}/src/monitor")
include_directories("${CMAKE_SOURCE_DIR}/src/network")
include_directories("${CMAKE_SOURCE_DIR}/src/service")

add_subdirectory(concurrency)
add_subdirectory(utils)
add_subdirectory(ams)
add_subdirectory(config)
add_subdirectory(monitor)
add_subdirectory(network)
add_subdirectory(service)

set(UTILS_LIBS ${UTILS_LIBS} utils)
set(AMS_LIBS ${AMS_LIBS} ams)
set(CONCURRENCY_LIBS ${CONCURRENCY_LIBS} concurrency)
set(CONFIG_LIBS ${CONFIG_LIBS} config)
set(MONITOR_LIBS ${MONITOR_LIBS} monitor)
set(NETWORK_LIBS ${NETWORK_LIBS} network)
set(SERVICE_LIBS ${SERVICE_LIBS} service)

#lib search dir
link_directories("${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/lib")
link_directories("${CMAKE_SOURCE_DIR}/thirdparty/curl-7.28.0/lib")
link_directories("${CMAKE_SOURCE_DIR}/thirdparty/libevent-2.1.11/lib")
link_directories("${CMAKE_SOURCE_DIR}/thirdparty/glog0.3.4/lib")

file(GLOB HEADER_FILES "*.cc" "*.cpp" "ams/*.cc" "concurrency/*.cc" "utils/*.cc" "config/*.cc" "monitor/*.cc" "network/*.cc" "service/*.cc")
file(GLOB SOURCE_FILES "*.h" "ams/*.h" "concurrency/*.h" "utils/*.h" "config/*.h" "config/*.hpp" "monitor/*.h" "network/*.h" "service/*.h")
#package file
add_library(kbms STATIC ${SOURCE_FILES} ${HEADER_FILES})


message(STATUS ${SERVING_SRC})

#link lib
target_link_libraries(kbms
        ${CMAKE_THREAD_LIBS_INIT}
        ${CONCURRENCY_LIBS}
        ${AMS_LIBS}
        ${CONFIG_LIBS}
        ${UTILS_LIBS}
        ${MONITOR_LIBS}
        ${NETWORK_LIBS}
        ${SERVICE_LIBS}
        libglog.a
        libjsoncpp.a
        libevent.a
        libcurl.a)
