include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. CONFIG_SRC)

add_library(config STATIC ${CONFIG_SRC})
