include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. AMS_SRC)

add_library(ams STATIC ${AMS_SRC})
