cmake_minimum_required(VERSION 2.8)
project(pte)

set(MAJOR_VERSION "0")
set(MINOR_VERSION "1")
set(PATCH_VERSION "0")

add_subdirectory(src)
add_subdirectory(lib)

set(SRCS
	src/pte.c
	src/elements.c
)
add_executable(pte ${SRCS})
target_link_libraries(pte printers utils json)

install(TARGETS pte
	RUNTIME DESTINATION bin
)
install(FILES pte_bash_completion
	DESTINATION /etc/bash_completion.d
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
	RENAME pte
)
install(FILES doc/manpage.1
	DESTINATION share/man/man1
	RENAME pte.1.gz
)

set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set(CPACK_PACKAGE_CONTACT "Erich Schroeter erichschroeter@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "retrieve element information")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A CLI for retrieving element information from the periodic table")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")

# Debian specific options
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

include(CPack)
