#!/bin/sh
#
# Converts the JSON to a C header file to be used as a resource file.

json_h() {
cat << EOF
#ifndef ELEMENTS_H
#define ELEMENTS_H

extern const char* ELEMENTS_JSON;

#endif
EOF
}


json_c() {
cat << EOF
#include "elements.h"

extern const char* ELEMENTS_JSON =
"$(cat elements.json | sed 's/"/\\"/g' | sed ':a;N;$!ba;s/\n/"\n"/g')";

EOF
}

json_h > ../src/elements.h
json_c > ../src/elements.c
exit 0
