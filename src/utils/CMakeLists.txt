include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. UTILS_SRC)

add_library(utils STATIC ${UTILS_SRC})
