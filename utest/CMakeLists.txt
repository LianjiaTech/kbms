include_directories("${CMAKE_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/curl-7.28.0/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/libevent-2.1.11/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/glog0.3.4/include")
include_directories("${CMAKE_SOURCE_DIR}/thirdparty/gtest-1.10.0/include")
include_directories("${CMAKE_SOURCE_DIR}/src/")

set(LIBRARIES kbms ${DEPENDENT_LIBRARIES})

link_directories(${PROJECT_BINARY_DIR}/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/glog0.3.4/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/jsoncpp-1.8.3/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/libevent-2.1.11/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/gtest-1.10.0/lib/
        ${PROJECT_SOURCE_DIR}/thirdparty/curl-7.28.0/lib/)

add_executable(gtest_server gtest_http_server.cc)
target_link_libraries(gtest_server gtest gtest_main ${LIBRARIES})


add_executable(gtest_task gtest_task.cc)
target_link_libraries(gtest_task gtest gtest_main ${LIBRARIES})

add_executable(gtest_thread_pool gtest_thread_pool.cc)
target_link_libraries(gtest_thread_pool gtest gtest_main ${LIBRARIES})
