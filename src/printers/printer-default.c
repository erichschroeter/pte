#include <stdio.h>
#include <string.h>
#include "../../lib/json-parser/json.h"
#include "../pte.h"
#include "printer-default.h"

int print_default(int argc, char** argv)
{
    // can't do anything if we don't know which element(s) to print
    if (argc < 1)
    {
        return;
    }
    int i;
    for (i=0; i < argc; i++)
    {
        json_value* element = get_element(argv[i]);
        if (element == NULL)
        {
            return;
        }
        int j;
        for (j=0; j < element->u.object.length; j++)
        {
            char * key = element->u.object.values[j].name;
            json_value* value = element->u.object.values[j].value;

            if (!strcmp(key, "name"))
            {
                printf("name: '%s'\n", value->u.string.ptr);
            }
            else if (!strcmp(key, "symbol"))
            {
                printf("symbol: '%s'\n", value->u.string.ptr);
            }
            else if (!strcmp(key, "atomicNumber"))
            {
                long num = value->u.integer;
                printf("atomic number: '%ld'\n", num);
            }
        }
    }
    return 0;
}
