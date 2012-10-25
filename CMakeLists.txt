cmake_minimum_required(VERSION 2.8)
project(pte)

add_subdirectory(src)
add_subdirectory(lib)

set(SRCS
	src/pte.c
	src/elements.c
)
add_executable(pte ${SRCS})
target_link_libraries(pte printers utils json)
