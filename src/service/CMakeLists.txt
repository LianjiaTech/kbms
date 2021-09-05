include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. SERVICE)

add_library(service STATIC ${SERVICE})
