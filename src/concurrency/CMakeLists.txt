include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. CONCURRENCY)

add_library(concurrency STATIC ${CONCURRENCY})
