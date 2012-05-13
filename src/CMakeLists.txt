ADD_SUBDIRECTORY(printers)
ADD_SUBDIRECTORY(util)

ADD_SOURCES(
    pte.h pte.c
    elements.h elements.c
    ../lib/json-parser/json.c
)
