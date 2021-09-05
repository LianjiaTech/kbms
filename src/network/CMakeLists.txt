include_directories("${CMAKE_SOURCE_DIR}/src")

aux_source_directory(. NETWORK)

add_library(network STATIC ${NETWORK})
