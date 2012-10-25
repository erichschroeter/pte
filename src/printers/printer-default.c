#include <stdio.h>
#include <string.h>
#include "../../lib/json-parser/json.h"
#include "../pte.h"
#include "printer-default.h"

int print_default_all()
{
    int i;
    for (i = 1; i <= 118; i++)
    {
        char atomic_num[3];
        sprintf(atomic_num, "%d", i);
        json_value* element = get_element(atomic_num);
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
        json_value_free(element);
    }
    return 118;
}

int print_default(int argc, char** argv)
{
    int i;
    int num_of_elements = 0;
    json_value* elements[1024];
    // filter out all arguments that are not elements
    for (i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "*"))
        {
            print_default_all();
            return 0;
        }
        json_value* element = get_element(argv[i]);
        if (element == NULL)
        {
            continue;
        }
        elements[num_of_elements] = element;
        num_of_elements++;
    }
    for (i=0; i < num_of_elements; i++)
    {
        json_value* element = elements[i];
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
