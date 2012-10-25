#include <stdio.h>
#include <string.h>
#include "../pte.h"
#include "../elements.h"
#include "../../lib/json-parser/json.h"
#include "printer-json.h"

extern const char* ELEMENTS_JSON;

int print_json_all()
{
    printf("%s\n", ELEMENTS_JSON);
    return 118;
}

int print_json(int argc, char** argv)
{
    int i;
    int num_of_elements = 0;
    json_value* elements[1024];
    // filter out all arguments that are not elements
    for (i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "*"))
        {
            print_json_all();
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
        // print the opening bracket to start a JSON object
        printf("{");
        int j;
        for (j=0; j < element->u.object.length; j++)
        {
            char * key = element->u.object.values[j].name;
            json_value* value = element->u.object.values[j].value;

            switch(value->type)
            {
            case json_integer:
                printf("\"%s\":\"%ld\"", key, value->u.integer);
                break;
            case json_double:
                printf("\"%s\":\"%f\"", key, value->u.dbl);
                break;
            case json_string:
                printf("\"%s\":\"%s\"", key, value->u.string.ptr);
                break;
            case json_boolean:
                printf("\"%s\":\"%d\"", key, value->u.boolean);
                break;
            case json_object:
            case json_array:
            case json_none:
            case json_null:
            default:
                break;
            }
            // if not the last object print a comma
            if (j != (element->u.object.length - 1))
            {
                printf(",");
            }
        }
        // print the closing bracket to end the JSON object
        printf("}");
        // if not the last object print a comma
        if (i < (num_of_elements - 1))
        {
            printf(",");
        }
    }
    printf("\n");
    return 0;
}
