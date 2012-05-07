#!/bin/sh
#
# Converts the JSON to a C header file to be used as a resource file.

json() {
cat << EOF
#ifndef ELEMENTS_H
#define ELEMENTS_H

char* ELEMENTS_JSON =
"$(cat elements.json | sed 's/"/\\"/g' | sed ':a;N;$!ba;s/\n/"\n"/g')";

#endif
EOF
}

json > ../src/elements.h
exit 0
