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

            switch(value->type)
            {
            case json_integer:
                printf("%s:%ld\n", key, value->u.integer);
                break;
            case json_double:
                printf("%s:%f\n", key, value->u.dbl);
                break;
            case json_string:
                printf("%s:%s\n", key, value->u.string.ptr);
                break;
            case json_boolean:
                printf("%s:%d\n", key, value->u.boolean);
                break;
            case json_object:
            case json_array:
            case json_none:
            case json_null:
            default:
                break;
            }
        }
    }
    return 0;
}
