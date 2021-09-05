include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. MONITOR)

add_library(monitor STATIC ${MONITOR})
