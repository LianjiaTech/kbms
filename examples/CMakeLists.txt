include_directories("${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/curl-7.28.0/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/libevent-2.1.11/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/glog0.3.4/include")
include_directories("${CMAKE_SOURCE_DIR}/src/")

set(LIBRARIES kbms ${DEPENDENT_LIBRARIES})

link_directories(${PROJECT_BINARY_DIR}/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/glog0.3.4/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/libevent-2.1.11/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.28.0/lib/)

add_executable(example_http http/example_http.cc)
target_link_libraries(example_http ${LIBRARIES})


add_executable(example_echo http/example_echo.cc)
target_link_libraries(example_echo ${LIBRARIES})
